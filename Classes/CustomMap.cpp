#include "CustomMap.h"
USING_NS_CC;

namespace
{
const char C_LAYER_NAME[] = "tiles";
const char C_OBSTACLES_LAYER_NAME[] = "obstacles";
const char C_OBSTACLES_LAYER_NAME_FOR_ENEMY[] = "obstaclesForEnemy";
const char C_OBSTACLES_LAYER_NAME_FOR_ON_GROUND[] = "onGround";
const char C_UNITS_LAYER_NAME[] = "units";
const char C_PASSABILITY_PROP[] = "pass";
const char C_HERO_TYPE_KEY[] = "hero";
const char C_NPC_TYPE_KEY[] = "npc";


void DumpObjectGroup(TMXObjectGroup *group)
{
    if (!group)
    {
        CCLOG("-- objects group empty --");
        return;
    }
    CCLOG("-- objects group `%s` --", group->getGroupName().c_str());
    for (const Value &v : group->getObjects())
    {
        CCLOG("%s", v.getDescription().c_str());
    }
    CCLOG("-- objects group end --");
}


class CLayerBoolean
{
public:
    CLayerBoolean(TMXTiledMap & map, TMXLayer & layer, const std::string &name)
        : m_map(map)
        , m_layer(layer)
        , m_name(name)
    {
    }
    bool GetProperty(unsigned x, unsigned y)
    {
        uint32_t groupId = m_layer.getTileGIDAt(Vec2(x, y));

        auto cachedValueIt = m_cache.find(groupId);
        if (cachedValueIt != m_cache.end())
        {
            return cachedValueIt->second;
        }
        bool canPass = HasGroupBoolProperty(groupId);
        m_cache[groupId] = canPass;
        return canPass;
    }
private:
    bool HasGroupBoolProperty(uint32_t groupId)
    {
		Value value = m_map.getPropertiesForGID(int(groupId));
		if (value.isNull())
		{
			return false;
		}
        ValueMap groupProps = value.asValueMap();
        return groupProps[m_name].asBool();
    }

    TMXTiledMap & m_map;
    TMXLayer & m_layer;
    const std::string m_name;
    std::unordered_map<uint32_t, bool> m_cache;
};

struct SLine
{
    Vec2 from;
    Vec2 to;

    SLine() = default;
    SLine(const Vec2 &from, const Vec2 &to)
        : from(from)
        , to(to)
    {
    }

    bool Intersects(const SLine &other)const;
    bool Intersects(const Rect &other)const;
};

bool IsBetween(float x, float minX, float maxX)
{
    return (minX <= x) && (x <= maxX);
}

bool SLine::Intersects(const SLine &other) const
{
    const Vec2 deltaThis = to - from;
    const Vec2 deltaOther = other.to - other.from;
    const Vec2 deltaFrom = from - other.from;
    const float denom = deltaOther.y * deltaThis.x - deltaOther.x * deltaThis.y;
    const float epsilon = std::numeric_limits<float>::epsilon();
    // Если знаменатель равен нулю, то прямые параллельны или совпадают
    if (IsBetween(denom, -epsilon, +epsilon))
    {
        return false;
    }
    const float numThis = deltaOther.x * deltaFrom.y - deltaOther.y * deltaFrom.x;
    const float numOther = deltaThis.x * deltaFrom.y - deltaThis.y * deltaFrom.x;
    const float factorThis = numThis / denom;
    const float factorOther = numOther / denom;

    return IsBetween(factorThis, 0, 1) && IsBetween(factorOther, 0, 1);
}

bool SLine::Intersects(const Rect &other) const
{
    Vec2 topLeft(other.getMinX(), other.getMinY());
    Vec2 topRight(other.getMaxX(), other.getMinY());
    Vec2 bottomLeft(other.getMinX(), other.getMaxY());
    Vec2 bottomRight(other.getMaxX(), other.getMaxY());

    return Intersects(SLine(topLeft, topRight))
            || Intersects(SLine(topLeft, bottomLeft))
            || Intersects(SLine(topRight, bottomRight))
            || Intersects(SLine(bottomLeft, bottomRight))
            || other.containsPoint(from);
}

}

bool CCustomMap::CanWalkDirectly(const Vec2 &from, const Vec2 &to) const
{
    SLine path(TMXTiledMap::convertToNodeSpace(from),
               TMXTiledMap::convertToNodeSpace(to));
    return std::none_of(m_obstacles.begin(), m_obstacles.end(), [=](const Rect &rect) {
        return path.Intersects(rect);
    });
}


bool CCustomMap::CanStandOn(const Vec2 &worldPosition) const
{
    Vec2 localPosition = TMXTiledMap::convertToNodeSpace(worldPosition);
    bool isOnObstacle = std::any_of(m_obstacles.begin(), m_obstacles.end(), [=](const Rect &rect) {
        return rect.containsPoint(localPosition);
    });
    Rect bounds(Vec2(0, 0), getContentSize());
    bool isOnMap = bounds.containsPoint(localPosition);

    return isOnMap && !isOnObstacle;
}

cocos2d::Rect CCustomMap::GetHeroWorldPosition() const
{
    return m_heroPosition;
}

std::vector<cocos2d::Rect> CCustomMap::GetObcaslePosition()const
{
	return m_obstacles;
}

std::vector<cocos2d::Rect> CCustomMap::GetObcasleOnGround()const
{
	return m_obstaclesOnGround;
}

std::vector<Vec2> CCustomMap::GetEnemyWorldPositions() const
{
    std::vector<Vec2> positions(m_enemyPositions);
    for (Vec2 & pos : positions)
    {
        pos = convertToWorldSpace(pos);
    }
    return positions;
}

bool CCustomMap::init(const std::string &tmxFile)
{
    return initWithTMXFile(tmxFile) &&
		InitPassabilityMap() && 
		LoadObstacles() &&
		LoadUnits() &&
		LoadOnGround() && 
		LoadBonusJump() && 
		LoadBonusHP() && 
		LoadObstaclesKey() && 
		LoadCoogwheel();
}

bool CCustomMap::InitPassabilityMap()
{
    TMXLayer *layer = TMXTiledMap::getLayer(C_LAYER_NAME);
	auto s = layer->getTileSet();
    CLayerBoolean property(*this, *layer, C_PASSABILITY_PROP);
    const Size size = layer->getLayerSize();
    const unsigned width = static_cast<unsigned>(size.width);
    const unsigned height = static_cast<unsigned>(size.height);

    m_passability.resize(width * height);
    for (unsigned x = 0; x < width; ++x)
    {
        for (unsigned y = 0; y < height; ++y)
        {
            unsigned index = x + y * width;
            m_passability[index] = property.GetProperty(x, y);
        }
    }
    return true;
}

bool CCustomMap::LoadObstacles()
{
    TMXObjectGroup *group = TMXTiledMap::getObjectGroup(C_OBSTACLES_LAYER_NAME);
    try
    {
        std::vector<cocos2d::Rect> obstacles;
        for (Value object : group->getObjects())
        {
            obstacles.emplace_back(AsRect(object.asValueMap()));
        }
        std::swap(obstacles, m_obstacles);
    }
    catch (const std::exception &)
    {
        return false;
    }
    return true;
}



bool CCustomMap::LoadUnits()
{
    TMXObjectGroup *group = TMXTiledMap::getObjectGroup(C_UNITS_LAYER_NAME);
    try
    {
        for (Value object : group->getObjects())
        {
            ValueMap map = object.asValueMap();
            Rect rect = AsRect(map);
            if (C_NPC_TYPE_KEY == map.at("type").asString())
            {
                m_enemyPositions.push_back(rect.origin);
            }
            if (C_HERO_TYPE_KEY == map.at("type").asString())
            {
				m_heroPosition = rect;
			}
        }
    }
    catch (const std::exception &)
    {
        return false;
    }
    return true;
}


bool CCustomMap::LoadObstaclesKey()
{
	TMXObjectGroup *group = TMXTiledMap::getObjectGroup("key");
	try
	{
		std::vector<cocos2d::Rect> obstacles;
		for (Value object : group->getObjects())
		{
			obstacles.emplace_back(AsRect(object.asValueMap()));
		}
		std::swap(obstacles, m_obstaclesKey);
	}
	catch (const std::exception &)
	{
		return false;
	}
	return true;
}


bool CCustomMap::LoadOnGround()
{
	TMXObjectGroup *group = TMXTiledMap::getObjectGroup(C_OBSTACLES_LAYER_NAME_FOR_ON_GROUND);
	try
	{
		std::vector<cocos2d::Rect> obstacles;
		for (auto object : group->getObjects())
		{
			obstacles.emplace_back(AsRect(object.asValueMap()));
		}
		std::swap(obstacles, m_obstaclesOnGround);
	}
	catch (const std::exception &)
	{
		return false;
	}
	return true;
}

bool CCustomMap::LoadCoogwheel()
{
	TMXObjectGroup *group = TMXTiledMap::getObjectGroup("coogwheel");
	try
	{
		std::vector<cocos2d::Rect> obstacles;
		for (auto object : group->getObjects())
		{
			obstacles.emplace_back(AsRect(object.asValueMap()));
		}
		std::swap(obstacles, m_obstaclesCoogwheel);
	}
	catch (const std::exception &)
	{
		return false;
	}
	return true;
}


Rect CCustomMap::AsRect(const ValueMap &properties) const
{
	Rect rect;
	rect.origin.x = properties.at("x").asFloat();
	rect.origin.y = properties.at("y").asFloat();
	rect.size.width = properties.at("width").asFloat();
	rect.size.height = properties.at("height").asFloat();
    return rect;
}


bool CCustomMap::LoadBonusJump()
{
	TMXObjectGroup *objectGroup = TMXTiledMap::getObjectGroup("bonusJump");
	try
	{
		std::vector<cocos2d::Rect> obstacles;
		for (auto object : objectGroup->getObjects())
		{
			obstacles.emplace_back(AsRect(object.asValueMap()));
			//auto bon = CBonuce::create(Vec2(bonuce.getMidX(), bonuce.getMidY()), CBonuce::Quest);
			//this->addChild(bon);
		}
		std::swap(obstacles, m_obstaclesBounusJump);
	}
	catch (const std::exception &)
	{
		return false;
	}
	return true;
}


bool CCustomMap::LoadBonusHP()
{
	TMXObjectGroup *objectGroup = TMXTiledMap::getObjectGroup("bonusHP");
	try
	{
		std::vector<cocos2d::Rect> obstacles;
		for (auto object : objectGroup->getObjects())
		{
			obstacles.emplace_back(AsRect(object.asValueMap()));
			//auto bon = CBonuce::create(Vec2(bonuce.getMidX(), bonuce.getMidY()), CBonuce::Quest);
			//this->addChild(bon);
		}
		std::swap(obstacles, m_obstaclesBounusHP);
	}
	catch (const std::exception &)
	{
		return false;
	}
	return true;
}


std::vector<cocos2d::Rect> CCustomMap::GetObcasleBonusJumpPosition() const
{
	return m_obstaclesBounusJump;
}

std::vector<cocos2d::Rect> CCustomMap::GetObcasleBonusHPPosition() const
{
	return m_obstaclesBounusHP;
}
std::vector<cocos2d::Rect> CCustomMap::GetObcasleKey() const
{
	return m_obstaclesKey;
}

std::vector<cocos2d::Rect> CCustomMap::GetObcasleCoogwheel() const
{
	return m_obstaclesCoogwheel;
}
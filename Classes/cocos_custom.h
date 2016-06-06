#pragma once

#include "cocos2d.h"
#include <memory>
#include <functional>

#ifndef _WIN32
namespace std
{
template <class T, class ...TArgs>
std::unique_ptr<T> make_unique(TArgs&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<TArgs>(args)...));
}
}
#endif

template <class T, class ...TArgs>
cocos2d::RefPtr<T> make_node(TArgs&&... args)
{
    cocos2d::RefPtr<T> ret(new (std::nothrow) T);
    if (ret && ret->init(std::forward<TArgs>(args)...)) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_RELEASE(ret);
    return nullptr;
}

template <class T, class ...TArgs>
T *make_cc(TArgs&&... args)
{
	T *ret = new (std::nothrow) T;
	if (ret && ret->init(std::forward<TArgs>(args)...)) {
		ret->autorelease();
		return ret;
	}
	CC_SAFE_RELEASE(ret);
	return nullptr;
}
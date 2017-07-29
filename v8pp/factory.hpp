//
// Copyright (c) 2013-2016 Pavel Medvedev. All rights reserved.
//
// This file is part of v8pp (https://github.com/pmed/v8pp) project.
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
#ifndef V8PP_FACTORY_HPP_INCLUDED
#define V8PP_FACTORY_HPP_INCLUDED

#include <memory>
#include <utility>

#include <v8.h>

namespace v8pp {

// Factory that creates new C++ objects of type T
template<typename T, bool use_shared_ptr = false>
struct factory
{
	static size_t const object_size = sizeof(T);

	template<typename ...Args>
	static T* create(v8::Isolate* isolate, Args... args)
	{
		T* object = new T(std::forward<Args>(args)...);
		isolate->AdjustAmountOfExternalAllocatedMemory(
			static_cast<int64_t>(object_size));
		return object;
	}

	static void destroy(v8::Isolate* isolate, T* object)
	{
		delete object;
		isolate->AdjustAmountOfExternalAllocatedMemory(
			-static_cast<int64_t>(object_size));
	}
};

// Factory that creates new std::shared_ptr<T> C++ objects of type T
template<typename T>
struct factory<T, true>
{
	template<typename ...Args>
	static std::shared_ptr<T> create(v8::Isolate*, Args... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	static void destroy(v8::Isolate*, std::shared_ptr<T> const&)
	{
		// do nothing with reference-counted object
	}
};

} //namespace v8pp

#endif // V8PP_FACTORY_HPP_INCLUDED

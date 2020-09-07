#pragma once

#include <cstdarg>
#include <iostream>
#include <typeindex>
#include <vector>

#include <cppmicroservices/ServiceInterface.h>

struct US_ABI_EXPORT Service
{
	using type = std::type_index;

	virtual ~Service() {}

	// define Evaluate in the most generic terms possible
	virtual void* Evaluate(va_list args) = 0;
	// allow a way to ensure the correct signature is used
	virtual bool Validate(const std::vector<type>& as, type r) = 0;
};

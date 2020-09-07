#pragma once

#include <stdexcept>
#include <cstdarg>

#include <cppmicroservices/BundleContext.h>
#include <cppmicroservices/GetBundleContext.h>

#include <service/Service.cpp>

namespace us = cppmicroservices;

struct Consumer
{
	// define a Callable to interact with Evaluate more naturally
	// manages the `va_list' and `void*' dereferencing
	template<class T>
	struct invoke;

	// the user specifies the expected type
	// this MUST match the actual signature/usage of the service
	template<class Out, class... Ins>
	struct invoke<Out(Ins...)>
	{
		using ServiceRef = us::ServiceReference<Service>;
	
		// the Callable is constructed with a reference to the service
		invoke(std::string name)
		{
			auto svcs = us::GetBundleContext().GetServiceReferences<Service>();
			service = *std::find_if(svcs.begin(), svcs.end(), [&](auto s) {
				return (s.GetBundle().GetSymbolicName() == name);
			});
		}
	
		// the call operator takes a parameter pack of the arguments to pass
		Out operator ()(Ins... args)
		{
			// make sure the service is good first
			// FIXME: `svc' could be pass to evaluate, if I knew the type...
			auto ctx = us::GetBundleContext();
			auto svc = ctx.GetService(service);
			if (!svc)
				throw std::runtime_error("service does not exist");

			// attempt to validate this call against the expected
			std::vector<Service::type> as = { typeid(Ins)... };
			Service::type r = typeid(Out);
			if (!svc->Validate(as, r))
				throw std::runtime_error("bad call to service Evaluate");

			// pass it to a helper function
			// `va_list' requires a concrete argument before the varargs
			void* res = evaluate(0, args...);
			// get the actual value from the `void*'
			return *(Out*)res;
		}
	
		// actually call Evaluate as it is defined
		void* evaluate(int dummy, ...)
		{
			va_list args;
			va_start(args, dummy);
	
			// FIXME: see above; repetition could be removed
			auto ctx = us::GetBundleContext();
			auto svc = ctx.GetService(service);

			return svc->Evaluate(args);
		}
	
		ServiceRef service;
	};
};

[+ AutoGen5 template cpp json +]
[+ CASE (suffix) +]

[+ == cpp +]
#include <cstdarg>
#include <iostream>
#include <csignal>
#include <cppmicroservices/BundleActivator.h>
extern "C" {
#include <wiringPi.h>
}

#include <service/Service.cpp>

namespace us = cppmicroservices;

// the service will always be named `Service<ServiceName>'
class Service[+ (get "name") +] : public Service
{
	// define static storage since the return value must return a `void*'
	// type+name gathered from the DDL `<OutputType>' and `<OutputName>'
	// FIXME: not thread-safe in its current form
	[+ IF (exist? "output.name") +]
	[+ (get "output.type") +] [+ (get "output.name") +] = {};
	[+ ENDIF +]

	// use a `va_list' to get known parameters dynamically
	// the end result should allow Evaluate to be used as `auto Evaluate(...)'
	void* Evaluate(va_list args) override
	{
		// each input is gathered from the DDL `<InputType>'
		[+ FOR inputs +]
		auto [+ (get "name") +] = va_arg(args, [+ (get "type") +]);
		[+ ENDFOR +]
		va_end(args);

		// codegen will be more advanced/fine-grained, eventually
		// for now, the whole function body is specified in the DDL `<Source>'
		[+ FOR source +]
		[+ (get "source") +]
		[+ ENDFOR +]
		
		// cast the static storage to a generic return type
		[+ IF (exist? "output.name") +]
		return (void*)&[+ (get "output.name") +];
		[+ ELSE +]
		return NULL;
		[+ ENDIF +]
	}

	// confirm parameter/result types match before calling
	bool Validate(const std::vector<type>& as, type r) override
	{
		// check the parameter count is the same
		if (as.size() != [+ (count "inputs") +])
			return false;
		// check the return type
		if (r != typeid([+ (get "output.type") +]))
			return false;

		// check each argument type
		[+ FOR inputs +]
		if (as[[+ (for-index) +]] != typeid([+ (get "type") +]))
			return false;
		[+ ENDFOR +]

		return true;
	}
};

// instantiate and register the service
class ServiceActivator : public us::BundleActivator
{
	void Start(us::BundleContext ctx)
	{
		auto service = std::make_shared<Service[+ (get "name") +]>();
		ctx.RegisterService<Service>(service);
	}

	void Stop(us::BundleContext) {}
};

CPPMICROSERVICES_EXPORT_BUNDLE_ACTIVATOR(ServiceActivator);

[+ == json +]
{
	"bundle.symbolic_name": "[+ (get "name") +]",
	"bundle.activator":     true,
	"bundle.name"         : "Service [+ (get "name") +]",
	"bundle.description"  : "This bundle is service type"
}

[+ ESAC +]

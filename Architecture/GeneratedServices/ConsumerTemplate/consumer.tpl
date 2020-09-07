[+ AutoGen5 template cpp json +]
[+ CASE (suffix) +]

[+ == cpp +]

#include <consumer/Consumer.cpp>

#include <iostream>

#include <cppmicroservices/Bundle.h>
#include <cppmicroservices/BundleActivator.h>
#include <cppmicroservices/BundleContext.h>


#include <cstdarg>
#include <cppmicroservices/GetBundleContext.h>
#include <service/Service.cpp>

namespace us = cppmicroservices;

class Consumer[+ (get "name") +]Activator : public us::BundleActivator, Consumer
{
	using ServiceRef = us::ServiceReference<Service>;

	void Start(us::BundleContext ctx)
	{
		Do();
	}

	void Stop(us::BundleContext) {}

	void Do() const
	{
		using Log = long(const char*);
		std::cout << std::endl;
		(void)invoke<Log>("Log")("hello, world"); //< (most vexing parse)
	}
};

CPPMICROSERVICES_INITIALIZE_BUNDLE;
CPPMICROSERVICES_EXPORT_BUNDLE_ACTIVATOR(Consumer[+ (get "name") +]Activator);


[+ == json +]
{
	"bundle.symbolic_name": "consumer[+ (get "name") +]",
	"bundle.activator":     true,
	"bundle.name"         : "Consumer [+ (get "name") +]",
	"bundle.description"  : "This bundle is consumer type"
}

[+ ESAC +]

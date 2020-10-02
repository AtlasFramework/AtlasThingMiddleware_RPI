/*
* FileName: API_Generator.cpp
* Description: Generates API for each service offered by the thing to thing mates, parses and runs received API call.
* Note: Supported datatypes for the current version of Atlas-IoT_thing are void, int, bool. This can be extended to 
* include other types.
*/

#include "API_Generator.h"
#include "System_Functions.h"
#include "IoTDDL_Parser.h"
#include "Identity_Parser.h"
#include <fstream>
#include <sys/types.h>
#include <iostream>
#include <sys/stat.h>
#include "GeneratedServices/service/Service.cpp"
#include <cppmicroservices/FrameworkFactory.h>
#include <cppmicroservices/Framework.h>
#include <cppmicroservices/BundleContext.h>
#include <cppmicroservices/Bundle.h>
#include <cppmicroservices/BundleImport.h>
#include <iostream>
#include <fstream>
#include <dlfcn.h>
#include <stdlib.h>
using namespace std;
namespace us = cppmicroservices;


class service_name_exception : public std::exception {};
class service_arg_exception  : public std::exception {};

template<class T>
struct invoke_result { void* value; operator T() const { return *(T*)value; } };

template<>
struct invoke_result<void> { void* value; };

template<class T>
struct invoke;

// the user specifies the expected type
// this MUST match the actual signature/usage of the service
template<class Out, class... Ins>

struct invoke<Out(Ins...)>{
	using ServiceRef = us::ServiceReference<Service>;

	// the Callable is constructed with a reference to the service
	invoke(us::BundleContext c, std::string name): ctx{c} {
		auto svcs = ctx.GetServiceReferences<Service>();
		service = *std::find_if(svcs.begin(), svcs.end(), [&](auto s) {
			return (s.GetBundle().GetSymbolicName() == name);
		});
	}

	// the call operator takes a parameter pack of the arguments to pass
	invoke_result<Out> operator ()(Ins... args){
		// make sure the service is good first
		// FIXME: `svc' could be pass to evaluate, if I knew the type...
		auto svc = ctx.GetService(service);
		if (!svc){
			throw service_name_exception();
			//throw std::runtime_error("service does not exist");
		}

		// attempt to validate this call against the expected
		std::vector<Service::type> as = { typeid(Ins)... };
		Service::type r = typeid(Out);
		if (!svc->Validate(as, r)){
			throw service_arg_exception();
			//throw std::runtime_error("bad call to service Evaluate");
		}

		// pass it to a helper function
		// `va_list' requires a concrete argument before the varargs
		return invoke_result<Out>{evaluate(0, args...)};
	}

	// actually call Evaluate as it is defined
	void* evaluate(int dummy, ...){
		va_list args;
		va_start(args, dummy);

		// FIXME: see above; repetition could be removed
		auto svc = ctx.GetService(service);
		return svc->Evaluate(args);
	}

	us::BundleContext ctx;
	ServiceRef service;
};


//---------------------------------------------------------------------

API_Generator::API_Generator(){
    	APIs.clear();
}

void API_Generator::Generate_ServicesBundles(){

	IoTDDL_Parser DDLM;

    	int num_Entities = 1;
    	string num_of_Entities = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Thing","Structural_Metadata","Number_Entities");
    	num_Entities =  atoi(num_of_Entities.c_str());

	for(int j=1;j<=num_Entities;j++){

		int num_services = 1; 
	    	string num_of_services = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"Services","Number_Services");
	    	num_services =  atoi(num_of_services.c_str());
	    
	    	for(int i=1;i<=num_services;i++){

			int Service_Number = i;
			cout<<"Parsing Information of Service #"<<i<<" under entity #"<<j<<endl;
			string Service_Name              = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"Services","Service_"+std::to_string(i),"Name");

			//Inputs
			string Service_InputTypes        = "NULL";
			string Service_InputDescription  = "NULL";
			string Service_InputRange        = "NULL";
		    	int num_Inputs = 0;
		    	string num_of_Inputs = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"Services","Service_"+std::to_string(i),"Service_NumberInputs");
		    	num_Inputs =  atoi(num_of_Inputs.c_str());

			if(num_Inputs > 0){
			      Service_InputTypes        = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"Services","Service_"+std::to_string(i),"Service_InputTypes");
			      Service_InputDescription  = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"Services","Service_"+std::to_string(i),"Service_InputDescriptions");
			      //Service_InputRange        = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"Services","Service_"+std::to_string(i),"Service_InputRange");
			}

			//Output
			string Service_OutputTypes       = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"Services","Service_"+std::to_string(i),"OutputType");

			//Code
			string Service_Code   		= DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"Services","Service_"+std::to_string(i),"Service_Formula");
		
			cout<<"Generating service bundle for "<<Service_Name<<"under entity#"<<j<<endl;
		
			auto dir = "Architecture/GeneratedServices/service/" + Service_Name + "/";

			mkdir(dir.c_str(), S_IRWXU | S_IRWXG);

		
			// Create service.def per service that will be used to generate the corresponding microservice -------------
			auto fflags = std::ofstream::out | std::ofstream::trunc;        // Create the file (overwrite if it already exists)
			std::ofstream def(dir + "service.def", fflags);
			if (!def.is_open()) return;
			def << "autogen definitions service;" << std::endl;             // the AutoGen header
	

			//set the library of the RaspberryPI GPIO
			//def << "library = " << "#include <wiringPi.h>" << ";" << std::endl;           // Add the service name

			def << "name = " << Service_Name << ";" << std::endl;           // Add the service name


			// Translate the Formula's statments into C code 
			string instruction = "";
			std::vector<std::string> Service_Formula;
			std::string::size_type pos = 0;
		    	std::string::size_type prev = 0;
			while ((pos = Service_Code.find("\n", prev)) != std::string::npos){
				Service_Formula.push_back(Service_Code.substr(prev, pos - prev));
				prev = pos + 1;
			}
			Service_Formula.push_back(Service_Code.substr(prev));

			for (auto& line : Service_Formula) {
				//these instructions work on RaspberryPi only

				if(line.length() == 0)	continue;
								
				std::size_t found=0;
	      			found = line.find("//");
				if (found!=std::string::npos) continue;
	      			found = line.find("return");
				if (found!=std::string::npos) continue;
	      			found = line.find("cout");
				if (found!=std::string::npos) continue;
	      			found = line.find("![");
				if (found!=std::string::npos) continue;
	      			found = line.find("]]");
				if (found!=std::string::npos) continue;

				//cout<<"Formula #"<<line<<endl;

	      			found = line.find("DigitalWrite_High(");
				if (found!=std::string::npos){
	  				std::size_t foundx = line.find("(");
	  				std::size_t foundy = line.find(")");
	  				if (foundx!=std::string::npos){
						std::string pin_id = line.substr(foundx+1, foundy-foundx-1);
						def << "source = \"" << "wiringPiSetupGpio();" << "\";" << std::endl;
						instruction = "pinMode(" + pin_id +", OUTPUT);";
						def << "source = \"" << instruction << "\";" << std::endl;
						instruction = "digitalWrite(" + pin_id +", HIGH);";
						def << "source = \"" << instruction << "\";" << std::endl;
					}
					continue;
				}

	      			found = line.find("DigitalWrite_Low(");
				if (found!=std::string::npos){
	  				std::size_t foundx = line.find("(");
	  				std::size_t foundy = line.find(")");
	  				if (foundx!=std::string::npos){
						std::string pin_id = line.substr(foundx+1, foundy-foundx-1);
						def << "source = \"" << "wiringPiSetupGpio();" << "\";" << std::endl;
						instruction = "pinMode(" + pin_id +", OUTPUT);";
						def << "source = \"" << instruction << "\";" << std::endl;
						instruction = "digitalWrite(" + pin_id +", LOW);";
						def << "source = \"" << instruction << "\";" << std::endl; 
					}
					continue;
				}

	      			found = line.find("Delay(");
				if (found!=std::string::npos){
	  				std::size_t foundx = line.find("(");
	  				std::size_t foundy = line.find(")");
	  				if (foundx!=std::string::npos){
						std::string time = line.substr(foundx+1, foundy-foundx-1);
						instruction = "delay(" + time +");";
						def << "source = \"" << instruction << "\";" << std::endl;
					}
					continue;
				}

	      			found = line.find("DigitalWrite_Blink(");
				if (found!=std::string::npos){
	  				std::size_t foundx = line.find("(");
	  				std::size_t foundy = line.find(",");
	  				std::size_t foundz = line.find(")");
	  				if (foundx!=std::string::npos){
						std::string pin_id = line.substr (foundx+1, foundy-foundx-1);
						std::string time   = line.substr (foundy+1, foundz-foundy-1);
						def << "source = \"" << "wiringPiSetupGpio();" << "\";" << std::endl;
						instruction = "pinMode(" + pin_id +", OUTPUT);";
						def << "source = \"" << instruction << "\";" << std::endl;
						instruction = "digitalWrite(" + pin_id +", HIGH);";
						def << "source = \"" << instruction << "\";" << std::endl; 
						instruction = "delay(" + time +");";
						def << "source = \"" << instruction << "\";" << std::endl;
						instruction = "digitalWrite(" + pin_id +", LOW);";
						def << "source = \"" << instruction << "\";" << std::endl; 
					}
					continue;
				}

				found = line.find("print_value(");
				if (found!=std::string::npos){
	  				std::size_t foundx = line.find("(");
	  				std::size_t foundy = line.find(")");
	  				if (foundx!=std::string::npos){
						std::string value = line.substr(foundx+1, foundy-foundx-1);
						instruction = "std::cout << " + value + " << std::endl;";
						def << "source = \"" << instruction << "\";" << std::endl;
					}
					continue;
				}

				found = line.find("print_string(");
				if (found!=std::string::npos){
	  				std::size_t foundx = line.find("(");
	  				std::size_t foundy = line.find(")");
	  				if (foundx!=std::string::npos){
						std::string value = line.substr(foundx+1, foundy-foundx-1);
						instruction = "std::cout << " + value + " << std::endl;";
						def << "source = \"" << instruction << "\";" << std::endl;
					}
					continue;
				}

				//////not tested >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
				//////not tested >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

				found = line.find("DigitalRead(");
				if (found!=std::string::npos){
	  				std::size_t foundx = line.find("(");
	  				std::size_t foundy = line.find(")");
	  				std::size_t foundz = line.find("=");
	  				if (foundx!=std::string::npos){
						std::string pin_id = line.substr(foundx+1, foundy-foundx-1);
						std::string returned = line.substr(0,foundz-1);
						def << "source = \"" << "wiringPiSetupGpio();" << "\";" << std::endl;
						instruction = "pinMode(" + pin_id +", INPUT);";
						def << "source = \"" << instruction << "\";" << std::endl;
						instruction = returned + " = "+ " digitalRead(" + pin_id +");";
						def << "source = \"" << instruction << "\";" << std::endl; 
					}
					continue;
				}



	      			found = line.find("AnalogWrite(");
				if (found!=std::string::npos){
	  				std::size_t foundx = line.find("(");
	  				std::size_t foundy = line.find(", ");
	  				if (foundx!=std::string::npos){
						std::string pin_id = line.substr (foundx+5,foundy-foundx-5);
						std::string value   = line.substr (foundy+8,line.size()-foundy-9);
						def << "source = \"" << "wiringPiSetupGpio();" << "\";" << std::endl;
						instruction = "pinMode(" + pin_id +", INPUT);";
						def << "source = \"" << instruction << "\";" << std::endl;
						instruction = "analogWrite(" + pin_id +", " + value +");";
						def << "source = \"" << instruction << "\";" << std::endl; 
					}
					continue;
				}

	      			
				

	      			found = line.find("AnalogRead(");
				if (found!=std::string::npos){
	  				std::size_t foundx = line.find("(");
	  				std::size_t foundy = line.find("=");
	  				if (foundx!=std::string::npos){
						std::string pin_id = line.substr(foundx+1,line.size()-foundx-2);
						std::string returned = line.substr(0,foundy-1);
						def << "source = \"" << "wiringPiSetupGpio();" << "\";" << std::endl;
						instruction = "pinMode(" + pin_id +", INPUT);";
						def << "source = \"" << instruction << "\";" << std::endl;
						instruction = returned + " = "+ " analogRead(" + pin_id +");";
						def << "source = \"" << instruction << "\";" << std::endl; 
					}
					continue;
				}



				found = line.find("}");
				if (found!=std::string::npos){
	  				instruction = line;
					def << "source = \"" << instruction << "\";" << std::endl;
					continue;
				}
				found = line.find("{");
				if (found!=std::string::npos){
	  				instruction = line;
					def << "source = \"" << instruction << "\";" << std::endl;
					continue;
				}
				found = line.find("while(");
				if (found!=std::string::npos){
	  				instruction = line;
					def << "source = \"" << instruction << "\";" << std::endl;
					continue;
				}
				found = line.find("for(");
				if (found!=std::string::npos){
	  				instruction = line;
					def << "source = \"" << instruction << "\";" << std::endl;
					continue;
				}
				found = line.find("if(");
				if (found!=std::string::npos){
	  				instruction = line;
					def << "source = \"" << instruction << "\";" << std::endl;
					continue;
				}
				found = line.find("else if(");
				if (found!=std::string::npos){
	  				instruction = line;
					def << "source = \"" << instruction << "\";" << std::endl;
					continue;
				}
				found = line.find("else");
				if (found!=std::string::npos){
	  				instruction = line;
					def << "source = \"" << instruction << "\";" << std::endl;
					continue;
				}

				instruction = line;
				def << "source = \"" << instruction << "\";" << std::endl;
			}



			if(Service_InputTypes.compare("NULL") !=0){
		      		vector<string> types;
		   	   	vector<string> description;
		   	   	vector<string> Range;
		  	    	string s;
		 	     	istringstream f(Service_InputTypes);
		      		istringstream d(Service_InputDescription);
		 	     	istringstream r(Service_InputRange);
		  	   	while(getline(f,s,','))  types.push_back(s);
		 	    	while(getline(d,s,','))  description.push_back(s);
		   	  	while(getline(r,s,','))  Range.push_back(s);
				int number_Inputs=description.size();

				for(int i=0;i<number_Inputs;i++)
					def << "inputs = { name = "<<description[i]<<"; type = \""<<types[i]<<"\"; };" << std::endl;

			}


			if(Service_OutputTypes == "void") {
				def << "output = { type = void; };" << std::endl;
			} else {
				string Service_OutputDescription       = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"Services","Service_"+std::to_string(i),"OutputDescription");
				//string Service_OutputRange       = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entity_"+std::to_string(num_Entities),"Resource_Service","Service_"+std::to_string(i),"OutputRange");
				def << "output = { name = "<<Service_OutputDescription<<"; type = \""<<Service_OutputTypes<<"\"; };" << std::endl;
			}
			def.close();

			// This system call instructs the Makefile under '.../Architecture/GeneratedServices/service/Makefile'
			// to generate the directory of the service
			system(("make -CArchitecture/GeneratedServices/service " + Service_Name).c_str());
			cout<<"Done Parsing and generating Service #"<<Service_Number<<endl;
	    }
	}
}



void API_Generator::Generate_ServicesAPIs(){

    	IoTDDL_Parser DDLM;

    	int num_Entities = 1;
    	string num_of_Entities = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Thing","Structural_Metadata","Number_Entities");
    	num_Entities =  atoi(num_of_Entities.c_str());

	for(int j=1;j<=num_Entities;j++){

		int num_services = 1;      
		string num_of_services = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"Services","Number_Services");
		num_services =  atoi(num_of_services.c_str());
	    
		//string Service_InputTypes        = "NULL"; //DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"Services","Service_"+std::to_string(i),"InputTypes");

	    	for(int i=1;i<=num_services;i++){
			int Service_Number = i;
			string Service_Name              = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"Services","Service_"+std::to_string(i),"Name");

			string Service_InputTypes        = "NULL";
			string Service_InputDescription  = "NULL";
			string Service_InputRange        = "NULL";
		    	int num_Inputs = 0;
		    	string num_of_Inputs = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"Services","Service_"+std::to_string(i),"Service_NumberInputs");
		    	num_Inputs =  atoi(num_of_Inputs.c_str());

			if(num_Inputs > 0){
			      Service_InputTypes        = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"Services","Service_"+std::to_string(i),"Service_InputTypes");
			      Service_InputDescription  = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"Services","Service_"+std::to_string(i),"Service_InputDescriptions");
			      //Service_InputRange      = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"Services","Service_"+std::to_string(i),"Service_InputRange");
			}
		
			string Service_OutputTypes       = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"Services","Service_"+std::to_string(i),"OutputType");
			string API_Input = "NULL";
			string API_Output = "NULL";

			if(Service_InputTypes.compare("NULL") !=0){
			      	vector<string> types;
			      	vector<string> description;
			      	//vector<string> Range;
			      	string s;
			      	istringstream f(Service_InputTypes);
			      	istringstream d(Service_InputDescription);
			      	//istringstream r(Service_InputRange);
			     	while(getline(f,s,','))  types.push_back(s);
			     	while(getline(d,s,','))  description.push_back(s);
			     	//while(getline(r,s,','))  Range.push_back(s);
				int number_Inputs=description.size();
				API_Input = description[0]+","+types[0]+", NULL"; //+Range[0];

				for(int i=1;i<description.size();i++)
					API_Input = API_Input +"|" +description[i]+","+types[i]+", NULL"; //+Range[i];
			}

			if(Service_OutputTypes.compare("void") !=0) {
				string Service_OutputDescription = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"Services","Service_"+std::to_string(i),"OutputDescription");
				//string Service_OutputRange     = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entity_"+std::to_string(num_Entities),"Resource_Service","Service_"+std::to_string(i),"OutputRange");
							
				API_Output = Service_OutputDescription+","+Service_OutputTypes+", NULL"; //+Service_OutputRange;
			}
	
			string Generated_API = Service_Name+":["+API_Input+"]:("+API_Output+")";
			APIs.push_back(Generated_API);
	    	}
	}
}





string API_Generator::Handle_ServiceCall(string APICall){
	
	auto fw = us::FrameworkFactory().NewFramework();
	fw.Init();
	auto ctx = fw.GetBundleContext();
	if (!ctx) { std::cout << "invalid fw context" << std::endl; return ""; }
	
	IoTDDL_Parser DDLM;
     	string TID       = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Thing","Descriptive_Metadata","Thing_ATID");
     	string SSID      = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Thing","Descriptive_Metadata","Thing_SSID");
	
        char json[4048];
	strcpy(json, APICall.c_str());
	Document document;  
	char buffer[4048];
	memcpy(buffer, json, sizeof(json));

	if (document.ParseInsitu(buffer).HasParseError())  
		return "";

	if((TID  != document["Thing ID"].GetString()) || (SSID != document["Space ID"].GetString())){
  	        cout<<"this service call is missing the correct thing id or smart space id"<<endl;
		string x1 = "\"Tweet Type\" : \"Service call reply\"";
		string x2 = "\"Thing ID\" : \""+TID+"\"";
		string x3 = "\"Space ID\" : \""+SSID+"\"";
		string x4 = "\"Status\" : \"Unsuccessful\"";
		string x5 = "\"Status Description\" : \"incorrect thing id or smart space id\"";
		return " { " + x1 + "," + x2 + ","+ x3 + ","+ x4 + ","+ x5 + " }";
	}

        std::cout << "API call with valid ATID and SSID" << std::endl;



    	
    	int num_Entities = 1;
    	string num_of_Entities = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Thing","Structural_Metadata","Number_Entities");
    	num_Entities =  atoi(num_of_Entities.c_str());


	for(int j=1;j<=num_Entities;j++){

	    	string num_of_services 	= DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"Services","Number_Services");
	    	int num_services 	= atoi(num_of_services.c_str());
	    
	    	for(int i=1;i<=num_services;i++){
			int Service_Number = i;
			string Service_Name              = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"Services","Service_"+std::to_string(i),"Name");
		
			if(Service_Name == document["Service Name"].GetString()){
		                std::cout << "found a service match for the corresponding API call"<< std::endl;
				ctx.InstallBundles("Architecture/GeneratedServices/service/" + Service_Name + "/lib" + Service_Name + ".so");
				fw.Start();
				auto bundles = ctx.GetBundles();
				for (auto& b : bundles)  
					b.Start();

		                //  need to check the type and range of the input
		                //****** now if it is one input, we don't have the [], this should be handled

				
				string Service_Inputs = document["Service Inputs"].GetString();
				

				//case one: no inputs -------------------------------------------------------------------
				
   				if((Service_Inputs.compare("()") == 0)||
					(Service_Inputs.compare("(null)") == 0)||
					(Service_Inputs.compare("(NULL)") == 0)||
					(Service_Inputs.compare("(void)") == 0)||
					(Service_Inputs.compare("(VOID)") == 0)){

					cout<<"calling "<<Service_Name<<" with no inputs"<<endl;

				    	string Service_OutputTypes       = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"Services","Service_"+std::to_string(i),"OutputType");

					if(Service_OutputTypes.compare("void") == 0) {
						using f = void(void);
						try{
						 	invoke<f>(ctx, Service_Name)();	
				  	                cout<<"no ouptput"<<endl;
						}catch(service_name_exception ex){
						  	cout<<"Unknown service"<<endl;
							string x1 = "\"Tweet Type\" : \"Service call reply\"";
							string x2 = "\"Thing ID\" : \""+TID+"\"";
							string x3 = "\"Space ID\" : \""+SSID+"\"";
							string x4 = "\"Status\" : \"Unsuccessful\"";
							string x5 = "\"Status Description\" : \"Unknown Service\"";
							return " { " + x1 + "," + x2 + ","+ x3 + ","+ x4 + ","+ x5 + " }";
						}catch(service_arg_exception ex){
						  	cout<<"Incorrect service arguments"<<endl;
							string x1 = "\"Tweet Type\" : \"Service call reply\"";
							string x2 = "\"Thing ID\" : \""+TID+"\"";
							string x3 = "\"Space ID\" : \""+SSID+"\"";
							string x4 = "\"Status\" : \"Unsuccessful\"";
							string x5 = "\"Status Description\" : \"Incorrect service arguments\"";
							return " { " + x1 + "," + x2 + ","+ x3 + ","+ x4 + ","+ x5 + " }";
						}
						string x1 = "\"Tweet Type\" : \"Service call reply\"";
						string x2 = "\"Thing ID\" : \""+TID+"\"";
						string x3 = "\"Space ID\" : \""+SSID+"\"";
						string x4 = "\"Service Name\" : \""+Service_Name+"\"";
						string x5 = "\"Status\" : \"Successful\"";
						string x6 = "\"Status Description\" : \"Successful Service Call\"";
						string x7 = "\"Service Result\" : \"No Output\"";
						return " { " + x1 + "," + x2 + ","+ x3 + ","+ x4 + ","+ x5 + ","+ x6 + ","+ x7 + " }";
					}


					if(Service_OutputTypes.compare("int") == 0) {
					
						using f = int(void);
						int result;
						try{
						 	result = invoke<f>(ctx, Service_Name)();	
				  	                cout<<"results: "<<result<<endl;
						}catch(service_name_exception ex){
						  	cout<<"Unknown service"<<endl;
							string x1 = "\"Tweet Type\" : \"Service call reply\"";
							string x2 = "\"Thing ID\" : \""+TID+"\"";
							string x3 = "\"Space ID\" : \""+SSID+"\"";
							string x4 = "\"Status\" : \"Unsuccessful\"";
							string x5 = "\"Status Description\" : \"Unknown Service\"";
							return " { " + x1 + "," + x2 + ","+ x3 + ","+ x4 + ","+ x5 + " }";
						}catch(service_arg_exception ex){
							cout<<"Incorrect service arguments"<<endl;
							string x1 = "\"Tweet Type\" : \"Service call reply\"";
							string x2 = "\"Thing ID\" : \""+TID+"\"";
							string x3 = "\"Space ID\" : \""+SSID+"\"";
							string x4 = "\"Status\" : \"Unsuccessful\"";
							string x5 = "\"Status Description\" : \"Incorrect service arguments\"";
							return " { " + x1 + "," + x2 + ","+ x3 + ","+ x4 + ","+ x5 + " }";
						}

						string x1 = "\"Tweet Type\" : \"Service call reply\"";
						string x2 = "\"Thing ID\" : \""+TID+"\"";
						string x3 = "\"Space ID\" : \""+SSID+"\"";
						string x4 = "\"Service Name\" : \""+Service_Name+"\"";
						string x5 = "\"Status\" : \"Successful\"";
						string x6 = "\"Status Description\" : \"Successful Service Call\"";
						string x7 = "\"Service Result\" : \""+std::to_string(result)+"\"";
						return " { " + x1 + "," + x2 + ","+ x3 + ","+ x4 + ","+ x5 + ","+ x6 + ","+ x7 + " }";
					}
				}
      				
				//------------------------------------------------------------------------
				//------------------------------------------------------------------------


				//case two: inputs (assume either one or two, and all are integers)  ---------------------------

				Service_Inputs.erase(std::remove(Service_Inputs.begin(),Service_Inputs.end(), '('),Service_Inputs.end());
				Service_Inputs.erase(std::remove(Service_Inputs.begin(),Service_Inputs.end(), ')'),Service_Inputs.end());

				vector<string> inputsx;
			      	string s;
			      	istringstream f(Service_Inputs);
			     	while(getline(f,s,','))  inputsx.push_back(s);

				if(inputsx.size() == 1){ //single input

					int sInput = stol(inputsx[0]);
	  	                	cout<<"calling "<<Service_Name<<" with input "<<sInput<<endl;

					string Service_OutputTypes       = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"Services","Service_"+std::to_string(i),"OutputType");

					if(Service_OutputTypes.compare("void") == 0) {
						using f = void(int);
						try{
						 	invoke<f>(ctx, Service_Name)(sInput);	
				  	        }catch(service_name_exception ex){
						  	cout<<"Unknown service"<<endl;
							string x1 = "\"Tweet Type\" : \"Service call reply\"";
							string x2 = "\"Thing ID\" : \""+TID+"\"";
							string x3 = "\"Space ID\" : \""+SSID+"\"";
							string x4 = "\"Status\" : \"Unsuccessful\"";
							string x5 = "\"Status Description\" : \"Unknown Service\"";
							return " { " + x1 + "," + x2 + ","+ x3 + ","+ x4 + ","+ x5 + " }";
						}catch(service_arg_exception ex){
						  	cout<<"Incorrect service arguments"<<endl;
							string x1 = "\"Tweet Type\" : \"Service call reply\"";
							string x2 = "\"Thing ID\" : \""+TID+"\"";
							string x3 = "\"Space ID\" : \""+SSID+"\"";
							string x4 = "\"Status\" : \"Unsuccessful\"";
							string x5 = "\"Status Description\" : \"Incorrect service arguments\"";
							return " { " + x1 + "," + x2 + ","+ x3 + ","+ x4 + ","+ x5 + " }";
						}

						string x1 = "\"Tweet Type\" : \"Service call reply\"";
						string x2 = "\"Thing ID\" : \""+TID+"\"";
						string x3 = "\"Space ID\" : \""+SSID+"\"";
						string x4 = "\"Service Name\" : \""+Service_Name+"\"";
						string x5 = "\"Status\" : \"Successful\"";
						string x6 = "\"Status Description\" : \"Successful Service Call\"";
						string x7 = "\"Service Result\" : \"No Output\"";
						return " { " + x1 + "," + x2 + ","+ x3 + ","+ x4 + ","+ x5 + ","+ x6 + ","+ x7 + " }";

					}
					if(Service_OutputTypes.compare("int") == 0) {
						using f = int(int);
						int result;
						try{
						 	result = invoke<f>(ctx, Service_Name)(sInput);	
				  	                cout<<"results: "<<result<<endl;
						}catch(service_name_exception ex){
						  	cout<<"Unknown service"<<endl;
							string x1 = "\"Tweet Type\" : \"Service call reply\"";
							string x2 = "\"Thing ID\" : \""+TID+"\"";
							string x3 = "\"Space ID\" : \""+SSID+"\"";
							string x4 = "\"Status\" : \"Unsuccessful\"";
							string x5 = "\"Status Description\" : \"Unknown Service\"";
							return " { " + x1 + "," + x2 + ","+ x3 + ","+ x4 + ","+ x5 + " }";
						}catch(service_arg_exception ex){
						  	cout<<"Incorrect service arguments"<<endl;
							string x1 = "\"Tweet Type\" : \"Service call reply\"";
							string x2 = "\"Thing ID\" : \""+TID+"\"";
							string x3 = "\"Space ID\" : \""+SSID+"\"";
							string x4 = "\"Status\" : \"Unsuccessful\"";
							string x5 = "\"Status Description\" : \"Incorrect service arguments\"";
							return " { " + x1 + "," + x2 + ","+ x3 + ","+ x4 + ","+ x5 + " }";
						}

						string x1 = "\"Tweet Type\" : \"Service call reply\"";
						string x2 = "\"Thing ID\" : \""+TID+"\"";
						string x3 = "\"Space ID\" : \""+SSID+"\"";
						string x4 = "\"Service Name\" : \""+Service_Name+"\"";
						string x5 = "\"Status\" : \"Successful\"";
						string x6 = "\"Status Description\" : \"Successful Service Call\"";
						string x7 = "\"Service Result\" : \""+std::to_string(result)+"\"";
						return " { " + x1 + "," + x2 + ","+ x3 + ","+ x4 + ","+ x5 + ","+ x6 + ","+ x7 + " }";
					}	
					


				}else if(inputsx.size() == 2){//two inputs


					int sInput1 = stol(inputsx[0]);
					int sInput2 = stol(inputsx[1]);
	  	                	cout<<"calling "<<Service_Name<<" with inputs "<<sInput1<<" and "<<sInput2<<endl;

					string Service_OutputTypes       = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"Services","Service_"+std::to_string(i),"OutputType");


					if(Service_OutputTypes.compare("void") == 0) {
						using f = void(int, int);
						try{
						 	invoke<f>(ctx, Service_Name)(sInput1,sInput2);	
				  	        }catch(service_name_exception ex){
						  	cout<<"Unknown service"<<endl;
							string x1 = "\"Tweet Type\" : \"Service call reply\"";
							string x2 = "\"Thing ID\" : \""+TID+"\"";
							string x3 = "\"Space ID\" : \""+SSID+"\"";
							string x4 = "\"Status\" : \"Unsuccessful\"";
							string x5 = "\"Status Description\" : \"Unknown Service\"";
							return " { " + x1 + "," + x2 + ","+ x3 + ","+ x4 + ","+ x5 + " }";
						}catch(service_arg_exception ex){
						  	cout<<"Incorrect service arguments"<<endl;
							string x1 = "\"Tweet Type\" : \"Service call reply\"";
							string x2 = "\"Thing ID\" : \""+TID+"\"";
							string x3 = "\"Space ID\" : \""+SSID+"\"";
							string x4 = "\"Status\" : \"Unsuccessful\"";
							string x5 = "\"Status Description\" : \"Incorrect service arguments\"";
							return " { " + x1 + "," + x2 + ","+ x3 + ","+ x4 + ","+ x5 + " }";
						}

						string x1 = "\"Tweet Type\" : \"Service call reply\"";
						string x2 = "\"Thing ID\" : \""+TID+"\"";
						string x3 = "\"Space ID\" : \""+SSID+"\"";
						string x4 = "\"Service Name\" : \""+Service_Name+"\"";
						string x5 = "\"Status\" : \"Successful\"";
						string x6 = "\"Status Description\" : \"Successful Service Call\"";
						string x7 = "\"Service Result\" : \"No Output\"";
						return " { " + x1 + "," + x2 + ","+ x3 + ","+ x4 + ","+ x5 + ","+ x6 + ","+ x7 + " }";

					}
					if(Service_OutputTypes.compare("int") == 0) {
						using f = int(int, int);
						int result;
						try{
						 	result = invoke<f>(ctx, Service_Name)(sInput1,sInput2);	
				  	                cout<<"results: "<<result<<endl;
						}catch(service_name_exception ex){
						  	cout<<"Unknown service"<<endl;
							string x1 = "\"Tweet Type\" : \"Service call reply\"";
							string x2 = "\"Thing ID\" : \""+TID+"\"";
							string x3 = "\"Space ID\" : \""+SSID+"\"";
							string x4 = "\"Status\" : \"Unsuccessful\"";
							string x5 = "\"Status Description\" : \"Unknown Service\"";
							return " { " + x1 + "," + x2 + ","+ x3 + ","+ x4 + ","+ x5 + " }";
						}catch(service_arg_exception ex){
						  	cout<<"Incorrect service arguments"<<endl;
							string x1 = "\"Tweet Type\" : \"Service call reply\"";
							string x2 = "\"Thing ID\" : \""+TID+"\"";
							string x3 = "\"Space ID\" : \""+SSID+"\"";
							string x4 = "\"Status\" : \"Unsuccessful\"";
							string x5 = "\"Status Description\" : \"Incorrect service arguments\"";
							return " { " + x1 + "," + x2 + ","+ x3 + ","+ x4 + ","+ x5 + " }";
						}

						string x1 = "\"Tweet Type\" : \"Service call reply\"";
						string x2 = "\"Thing ID\" : \""+TID+"\"";
						string x3 = "\"Space ID\" : \""+SSID+"\"";
						string x4 = "\"Service Name\" : \""+Service_Name+"\"";
						string x5 = "\"Status\" : \"Successful\"";
						string x6 = "\"Status Description\" : \"Successful Service Call\"";
						string x7 = "\"Service Result\" : \""+std::to_string(result)+"\"";
						return " { " + x1 + "," + x2 + ","+ x3 + ","+ x4 + ","+ x5 + ","+ x6 + ","+ x7 + " }";
					}
				}
			}
	    	}
	}

  	cout<<"call for unknown service"<<endl;
	string x1 = "\"Tweet Type\" : \"Service call reply\"";
	string x2 = "\"Thing ID\" : \""+TID+"\"";
	string x3 = "\"Space ID\" : \""+SSID+"\"";
	string x4 = "\"Status\" : \"Unsuccessful\"";
	string x5 = "\"Status Description\" : \"Unknown Service\"";
	return " { " + x1 + "," + x2 + ","+ x3 + ","+ x4 + ","+ x5 + " }";
}



void 	API_Generator::tesing_ServiceCall(){

	cout<<"\n\ntesting calling Tweets:"<<endl; 
	IoTDDL_Parser DDLM;
    	string ThingATID = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Thing","Descriptive_Metadata","Thing_ATID");
     	string ThingSSID = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Thing","Descriptive_Metadata","Thing_SSID");
  	string x1 = "\"Tweet Type\" : \"Service call\"";
	string x2 = "\"Thing ID\" : \""+ThingATID+"\"";
	string x3 = "\"Space ID\" : \""+ThingSSID+"\"";
	string x4 = "\"Service Name\" : \"case1\"";
	//string x5 = "\"Service Inputs\" : \"()\"";          //no inputs: can also be like (NULL), (null), (void)
	string x5 = "\"Service Inputs\" : \"(18)\"";      //single input (integer)
	//string x5 = "\"Service Inputs\" : \"(18, 20)\"";  //more than one input (all are integers)

	string JSONNN = " { " + x1 + "," + x2 + ","+ x3 + ","+ x4+ ","+ x5 + " }";
	cout<<JSONNN<<endl;
	string resultJSONs;
	char json[JSONNN.length() + 1];
   	strcpy(json, JSONNN.c_str()); 
	Document document;  
	char buffer[4048];
	memcpy(buffer, json, sizeof(json));
	
	if (document.ParseInsitu(buffer).HasParseError()) {  cout<<"received unformated message: "<< buffer<<endl;  return; }

	assert(document.IsObject());
	if(ThingATID != document["Thing ID"].GetString()){
          		cout<<"The unicast tweet dosn't match thing id......."<<endl;
			string x1 = "\"Status\" : \"ATID doesn't match\"";
			resultJSONs = " { " + x1 + " }";

	}else if(ThingSSID != document["Space ID"].GetString()){
          		cout<<"The unicast tweet dosn't match space id......."<<endl;
			string x1 = "\"Status\" : \"SSID doesn't match\"";
			resultJSONs = " { " + x1 + " }";
	}else{
          		cout<<"processing the received packet................"<<endl;
			resultJSONs = Handle_ServiceCall(json);
	}

	cout<<resultJSONs<<endl;
        cout<<"done testing the received packet..................."<<endl;
}

/*
* FileName: API_Generator.h
* Description: Generates API for each service offered by the thing to thing mates, parses and runs received API call.
* Note: Supported datatypes for the current version of Atlas-IoT_thing are void, int, bool. This can be extended to 
* include other types.
*/

#ifndef API_Generator_H
#define API_Generator_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <map>
#include <ctime>
using namespace std;

class API_Generator {
  
   public:
    	vector<string> APIs;                     //List of available APIs regarding the generated services.

    	API_Generator();
    	void 	Generate_ServicesBundles();      	//generate the service.def along with the full .cpp and .so
	void 	Generate_ServicesAPIs();         	//generate the corresponding API per service into APIS vector
	string 	Handle_ServiceCall(string APICall);     //handle/trigger the corresponding service w.r.t. the input API call
    	void 	tesing_ServiceCall();

	//+++++++++++++++++++++++++++Functions/Variable NOT IN USE NOW+++++++++++++++++++++++++++++++
 	vector<string> ThingServices_tweets;
    	vector<string> Services_APIs;
    	vector<string> Call_APIs;
   
	string 	Parse_ReceivedAPI(string API);
	int 	callType1_1(const string& API_Name);
	bool 	callType1_2(const string& API_Name);
	void 	callType1_3(const string& API_Name);
	int 	callType2_1(const string& API_Name, int value);
	bool 	callType2_2(const string& API_Name, int value);
	void 	callType2_3(const string& API_Name, int value);
	int 	callType3_1(const string& API_Name, int value1, int value2);
	bool 	callType3_2(const string& API_Name, int value1, int value2);
	void 	callType3_3(const string& API_Name, int value1, int value2);
};

#endif

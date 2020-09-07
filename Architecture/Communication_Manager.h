/*
* FileName: Communication_Manager.h
* Description: Holds the different communication protocols that the thing can speak with other thing mates (CoAP, Unix Socket, MQTT, RESTful).
* Unit: the interface and communication engine of the architecture - the interface sublayer - Atlas thing architecture
* Date: August 2020
*/


#include "Attachment_Engine.h"
#include "Identity_Parser.h"
#include "API_Generator.h"
#include "Knowledge_Tweeting_Manager.h"
#include "IoTDDL_Parser.h"
#define MSGBUFSIZE 552

/*****MQTT libraries****/
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "MQTTClient.h"
#define TIMEOUT     10000L

/***UNIX/IP socket libraries**/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <string>
#include <thread>

/********JSON*************/
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/document.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <map>
#include <cstdio>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
using namespace std;
using namespace rapidjson;




class Communication_Manager {
  public:
	//+++++++First Part: Atlas Communication Framework+++++++++++++++++++++++++++++++++++++++++++++++++++++++
	std::vector<std::string> 		thingTweets;
    	std::vector<std::string> 		receivedTweets;
    	static std::string 			ThingATID;
    	static std::string 			ThingSSID;
	
    	Communication_Manager(); 
	void TestSemanticCorrelation(string recService);
	

    	//++++++++++++++Socket Functions+++++++
    	int   MulticastPort; 		//port the multicast socket listening to
    	char* MulticastIP;   		//multicast socket IP
    	int   ServerPort;    		//port the unicast socket listening to

	//Multicast sockets
    	void BroadcastTweets_IPSocket();			    //Announce/broadcast tweets to the smart space 
    	void CaptureMatesTweets_IPSocket();			    //Capture and parse tweets announced/broadcasted by thing mates
 
	//Unicast sockets    	
	void CaptureAPIcalls_IPSocket();			    //Capture and parse API calls for thing's offered services
    	void InitiateAPICall_IPSocket(char* MateIP, int MatePort,std::string APICall_Tweet);  
								    //Initiate an API call for a service offered by a thing mate

    	//++++++++++++++MQTT Functions+++++++++
    	std::string ADDRESS;
    	std::string CLIENTID;
	std::string CLIENTID_subscribe;
	std::string CLIENTID_publish;
    	std::string RootTOPIC;
    	std::string TOPIC;
    	std::string MQTTClientTOPIC;
    	std::string PrivateBrokerTOPIC;
    	std::string MulticastThingIdTOPIC;
    	std::string MulticastEntityIdTOPIC;
    	std::string MulticastAPITOPIC;
    	std::string UnicastTOPIC;
    	std::string PAYLOAD;
    	short QOS;  
   
    	void MQTT_Subscribe();
    	void MQTT_Publish();

};

/*
* FileName: Identity_Parser.h
* Description: Build up information-based interactions (tweets) that describe the thing interms of (identity, communication language, entities)
* Unit: the identity parser - the DDL sublayer - Atlas thing architecture
* Date: August 2020
*/

#ifndef IdentityParser_H
#define IdentityParser_H

#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/document.h"
#include "IoTDDL_Parser.h"
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
#include <netdb.h>
#include <exception>
#include <string>
#include <ctime>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <chrono>
#include <ctime>
#include <cstdlib>
#include <curl/curl.h>
#include <netinet/in.h>
#include <time.h>
using namespace std;
using namespace rapidjson;


/// what is left is method to parse the entity info of the thing
/// add the entity and language info to the structure and continue parse the tweet from thing mates


class Identity_Parser {
  public:
    struct thingInfo{
	//thing information
	string thingID;
	string smartspaceID;
	string thingName;
	string thingModel;
	string thingVendor;
	string thingOwner;
	string thingDescription;
	string thingOperatingSystem;
	string thingType;
	string thingReleaseDate;
    };

    struct thingEntity{
	//entity information per thing
	string thingID;
	string smartspaceID;
	string entityID;
	string entityName;
	string entityCategory;
	string entityOwner;
	string entityVendor;
	string entityType;
	string entityDescription;
    };
    
    struct thingLanguage{
	//language
	string thingID;
	string smartspaceID;
	string networkName;
	string networkCommunicationLanguage;
	string networkTransportProtocol;
	string thingIP;
	string thingPort;
    };


    thingInfo           selfInfo_identity;              //the thing's identity information
    thingLanguage       selfInfo_language;              //the thing's language information
    vector<thingEntity> selfInfo_entityFull;                //the thing's entity information

    vector<string> ThingIdentity_tweets;                //the thing's identity tweet to be announced
    vector<string> ThingLanguages_tweets;               //the thing's entity tweet to be announced
    vector<string> EntityIdentity_tweets;               //the thing's language tweet to be announced

    map<string, thingInfo>     thingmates_identity;     //the thing mates' identity information intercepted
    map<string, thingEntity>   thingmates_entities;     //the thing mates' entity information intercepted
    map<string, thingLanguage> thingmates_languages;    //the thing mates' language information intercepted
	
    Identity_Parser();
    void parse_ThingIdentity();
    void parse_ThingLanguages();
    void parse_EntityIdentity();
    void parse_mateIdentityTweet(string);
    void parse_mateLanguageTweet(string);
    void parse_mateEntityTweet(string);
    void display_mateIdentities();
    void display_mateLanguages();
    void display_mateEntities();
};
#endif

/*
* FileName: Identity_Parser.cpp
* Description: Build up information-based interactions (tweets) that describe the thing interms of (identity, communication language, entities)
* Unit: the identity parser - the DDL sublayer - Atlas thing architecture
* Date: August 2020
*/

#include "Identity_Parser.h"

void error(const char *msg){
    perror(msg);
    exit(0);
}

Identity_Parser::Identity_Parser(){
    	ThingIdentity_tweets.clear();
        ThingLanguages_tweets.clear();
        EntityIdentity_tweets.clear();
	selfInfo_entityFull.clear();
     	thingmates_identity.clear();
     	thingmates_entities.clear();
     	thingmates_languages.clear();
}

void Identity_Parser::parse_ThingIdentity(){
	IoTDDL_Parser DDLM;
     	selfInfo_identity.thingID              = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Thing","Descriptive_Metadata","Thing_ATID");
     	selfInfo_identity.smartspaceID         = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Thing","Descriptive_Metadata","Thing_SSID");
     	selfInfo_identity.thingName            = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Thing","Descriptive_Metadata","Thing_Name");
     	selfInfo_identity.thingModel           = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Thing","Descriptive_Metadata","Thing_Model");
     	selfInfo_identity.thingVendor          = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Thing","Descriptive_Metadata","Thing_Vendor");
     	selfInfo_identity.thingOwner           = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Thing","Descriptive_Metadata","Thing_Owner");
     	selfInfo_identity.thingDescription     = 
		DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Thing","Descriptive_Metadata","Thing_ShortDescription");
     	selfInfo_identity.thingOperatingSystem = 
		DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Thing","Descriptive_Metadata","Thing_OperatingSystem");

	string x1 = "\"Tweet Type\" : \"Identity_Thing\"";
	string x2 = "\"Thing ID\" : \""		+selfInfo_identity.thingID+"\"";
	string x3 = "\"Space ID\" : \""		+selfInfo_identity.smartspaceID+"\"";
	string x4 = "\"Name\" : \""    		+selfInfo_identity.thingName+"\"";
	string x5 = "\"Model\" : \""   		+selfInfo_identity.thingModel+"\"";
	string x6 = "\"Vendor\" : \""  		+selfInfo_identity.thingVendor+"\"";
	string x7 = "\"Owner\" : \""      	+selfInfo_identity.thingOwner+"\"";
	string x8 = "\"Description\" : \""	+selfInfo_identity.thingDescription+"\"";
	string x9 = "\"OS\" : \""         	+selfInfo_identity.thingOperatingSystem+"\"";

	string JSONs = " { " + x1 + "," + x2 + ","+ x3 + ","+ x4 + ","+ x5 + ","+ x6 + ","+ x7 + ","+ x8 + "," + x9 + " }";
	ThingIdentity_tweets.push_back(JSONs);
}

void Identity_Parser::parse_ThingLanguages(){
     	struct ifreq ifr;
 	int fd = socket(AF_INET, SOCK_DGRAM, 0);
 	ifr.ifr_addr.sa_family = AF_INET;

	IoTDDL_Parser DDLM;


	string temp = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Thing","Administrative_Metadata","Network_Manager","Module");
     	
	std::size_t found=0;
	found = temp.find("Ethernet");
	if (found!=std::string::npos) strncpy(ifr.ifr_name, "eth0", IFNAMSIZ-1);
	else                          strncpy(ifr.ifr_name, "tap0", IFNAMSIZ-1);


 	ioctl(fd, SIOCGIFADDR, &ifr);
 	close(fd);
	char *listenAddressString = inet_ntoa(((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr);


     	selfInfo_language.thingID                       = 
		DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Thing","Descriptive_Metadata","Thing_ATID");
     	selfInfo_language.smartspaceID                  = 
		DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Thing","Descriptive_Metadata","Thing_SSID");
     	selfInfo_language.networkName                   = 
		DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Thing","Administrative_Metadata","Network_Manager","Network_Name");
     	selfInfo_language.networkCommunicationLanguage 	= 
		DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Thing","Administrative_Metadata","Network_Manager","Communication_Protocol");
     	//selfInfo_language.networkTransportProtocol    = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Thing","Administrative_Metadata","Network_Manager","Transport_Protocol");
	
	selfInfo_language.thingIP = listenAddressString;
	selfInfo_language.thingPort = "6668";

	string x1 = "\"Tweet Type\" : \"Identity_Language\"";
	string x2 = "\"Thing ID\" : \""			+selfInfo_language.thingID+"\"";
	string x3 = "\"Space ID\" : \""			+selfInfo_language.smartspaceID+"\"";
	string x4 = "\"Network Name\" : \""		+selfInfo_language.networkName+"\"";
	string x5 = "\"Communication Language\" : \""	+selfInfo_language.networkCommunicationLanguage+"\"";
	//string x6 = "\"Transport Protocol\" : \""	+selfInfo_language.networkTransportProtocol+"\"";
	string x7 = "\"IP\" : \""			+selfInfo_language.thingIP+"\"";
	string x8 = "\"Port\" : \""			+selfInfo_language.thingPort+"\"";
	string JSONs = " { " + x1 + "," + x2 + ","+ x3 + ","+ x4 + ","+ x5 + ","+ x7 + ","+ x8 + " }";
	ThingLanguages_tweets.push_back(JSONs);
}

void Identity_Parser::parse_EntityIdentity(){

	IoTDDL_Parser DDLM;
	int num_Entities = 1;
    	string num_of_Entities = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Thing","Structural_Metadata","Number_Entities");
    	num_Entities =  atoi(num_of_Entities.c_str());

	for(int j=1;j<=num_Entities;j++){

                thingEntity selfInfo_entity;

	     	selfInfo_entity.thingID           = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Thing","Descriptive_Metadata","Thing_ATID");
	     	selfInfo_entity.smartspaceID      = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Thing","Descriptive_Metadata","Thing_SSID");
		selfInfo_entity.entityID          = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"Descriptive_Metadata","ID");
		selfInfo_entity.entityName        = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"Descriptive_Metadata","Name");
		selfInfo_entity.entityCategory    = 
			DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"Descriptive_Metadata","Category");
		selfInfo_entity.entityOwner       = 
			DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"Descriptive_Metadata","Owner");
		selfInfo_entity.entityVendor      = 
			DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"Descriptive_Metadata","Vendor");
		selfInfo_entity.entityType        = 
			DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"Descriptive_Metadata","Type");
		selfInfo_entity.entityDescription = 
			DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"Descriptive_Metadata","Description");
	     	
		string x1 = "\"Tweet Type\" : \"Identity_Entity\"";
		string x2 = "\"Thing ID\" : \""		+selfInfo_entity.thingID+"\"";
		string x3 = "\"Space ID\" : \""		+selfInfo_entity.smartspaceID+"\"";
		string x4 = "\"Name\" : \""		+selfInfo_entity.entityName+"\"";
		string x5 = "\"ID\" : \""		+selfInfo_entity.entityID+"\"";
		string x6 = "\"Type\" : \""		+selfInfo_entity.entityType+"\"";
		string x7 = "\"Owner\" : \""		+selfInfo_entity.entityOwner+"\"";
		string x8 = "\"Vendor\" : \""		+selfInfo_entity.entityVendor+"\"";
		string x9 = "\"Description\" : \""	+selfInfo_entity.entityDescription+"\"";
		string JSONs = " { " + x1 + "," + x2 + ","+ x3 + ","+ x4 + ","+ x5 + ","+ x6 + ","+ x7 + "," + x8 + "," + x9 + " }";
		EntityIdentity_tweets.push_back(JSONs);
		selfInfo_entityFull.push_back(selfInfo_entity);
	}
}

void Identity_Parser::parse_mateIdentityTweet(string JSONs){
        char json[4048];
	strcpy(json, JSONs.c_str());
	Document document;  
	char buffer[4048];
	memcpy(buffer, json, sizeof(json));
	if (document.ParseInsitu(buffer).HasParseError())  return;

	assert(document.IsObject());
	assert(document["Thing ID"].IsString());
	printf("ThingID = %s \n", document["Thing ID"].GetString());

	map<string,thingInfo>::iterator it = thingmates_identity.find(document["Thing ID"].GetString());
	if(it != thingmates_identity.end()){   cout<<"captured a repeated identity tweet from thing mate - updating"<<endl;}
        else{                                  cout<<"captured a new identity tweet from thing mate"<<endl;                }
	assert(document.IsObject());
	assert(document["Space ID"].IsString());

	thingInfo tInfo;
	tInfo.thingID 			= document["Thing ID"].GetString();
	tInfo.smartspaceID 		= document["Space ID"].GetString();
	tInfo.thingName 		= document["Name"].GetString();
	tInfo.thingModel 		= document["Model"].GetString();
	tInfo.thingVendor 		= document["Vendor"].GetString();
	tInfo.thingOwner 		= document["Owner"].GetString();
	tInfo.thingDescription 		= document["Description"].GetString();
	tInfo.thingOperatingSystem 	= document["OS"].GetString();
	thingmates_identity.insert (std::pair<string, thingInfo> (document["Thing ID"].GetString(),tInfo));
}

void Identity_Parser::parse_mateLanguageTweet(string JSONs){
        char json[4048];
	strcpy(json, JSONs.c_str());
	Document document;  
	char buffer[4048];
	memcpy(buffer, json, sizeof(json));
	if (document.ParseInsitu(buffer).HasParseError())  return;

	assert(document.IsObject());
	assert(document["Thing ID"].IsString());
	printf("ThingID = %s \n", document["Thing ID"].GetString());

	map<string,thingLanguage>::iterator it = thingmates_languages.find(document["Thing ID"].GetString());
	if(it != thingmates_languages.end()){   cout<<"captured a repeated language tweet from thing mate - updating"<<endl;}
        else{                                   cout<<"captured a new language tweet from thing mate"<<endl;                }
	assert(document.IsObject());
	assert(document["Space ID"].IsString());

	thingLanguage tInfo;
	tInfo.thingID = document["Thing ID"].GetString();
	tInfo.smartspaceID = document["Space ID"].GetString();
	tInfo.networkName = document["Network Name"].GetString();
	tInfo.networkCommunicationLanguage = document["Communication Language"].GetString();
	//tInfo.networkTransportProtocol = document["Transport Protocol"].GetString();
	tInfo.thingIP = document["IP"].GetString();
	tInfo.thingPort = document["Port"].GetString();
	thingmates_languages.insert (std::pair<string, thingLanguage> (document["Thing ID"].GetString(),tInfo));
}

void Identity_Parser::parse_mateEntityTweet(string JSONs){
        char json[4048];
	strcpy(json, JSONs.c_str());
	Document document;  
	char buffer[4048];
	memcpy(buffer, json, sizeof(json));
	if (document.ParseInsitu(buffer).HasParseError())  return;

	assert(document.IsObject());
	assert(document["Thing ID"].IsString());
	printf("ThingID = %s \n", document["Thing ID"].GetString());

	map<string,thingEntity>::iterator it = thingmates_entities.find(document["Thing ID"].GetString());
	if(it != thingmates_entities.end()){   cout<<"captured a repeated entity tweet from thing mate - updating"<<endl;}
        else{                                  cout<<"captured a new entity tweet from thing mate"<<endl;                }
	assert(document.IsObject());
	assert(document["Space ID"].IsString());

	thingEntity tInfo;
	tInfo.thingID 		= document["Thing ID"].GetString();
	tInfo.smartspaceID 	= document["Space ID"].GetString();
	tInfo.entityName 	= document["Name"].GetString();
	tInfo.entityID 		= document["ID"].GetString();
	tInfo.entityType 	= document["Type"].GetString();
	tInfo.entityOwner 	= document["Owner"].GetString();
	tInfo.entityVendor 	= document["Vendor"].GetString();
	tInfo.entityDescription = document["Description"].GetString();
	thingmates_entities.insert (std::pair<string, thingEntity> (document["Thing ID"].GetString(),tInfo));
}

void Identity_Parser::display_mateIdentities(){
	cout<< "display thing mates info ...."<<endl; 
	map<string,thingInfo>::iterator it2 = thingmates_identity.begin();
	for(it2=thingmates_identity.begin();it2!=thingmates_identity.end();++it2){
		cout<< "thing id"<< it2->first; 
		cout<< " space id"<< it2->second.smartspaceID; 
		cout<<endl;
	}
	cout<< "done with display"<<endl; 
}

void Identity_Parser::display_mateLanguages(){
	cout<< "display thing mates info ...."<<endl; 
	map<string,thingLanguage>::iterator it2 = thingmates_languages.begin();
	for(it2=thingmates_languages.begin();it2!=thingmates_languages.end();++it2){
		cout<< "thing id"<< it2->first; 
		cout<< " space id"<< it2->second.smartspaceID; 
		cout<<endl;
	}
	cout<< "done with display"<<endl; 
}

void Identity_Parser::display_mateEntities(){
	cout<< "display thing mates info ...."<<endl; 
	map<string,thingEntity>::iterator it2 = thingmates_entities.begin();
	for(it2=thingmates_entities.begin();it2!=thingmates_entities.end();++it2){
		cout<< "thing id"<< it2->first; 
		cout<< " space id"<< it2->second.smartspaceID; 
		cout<<endl;
	}
	cout<< "done with display"<<endl; 
}

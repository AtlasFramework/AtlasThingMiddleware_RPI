/*
* FileName: Knowledge_Tweeting_Manager.cpp
* Description: this module handles the relationships and links between the offered services and others' services
* Unit: the knowledge engine - the DDL sublayer - Atlas thing architecture
* Date: August 2020
*/

#ifndef Knowledge_Tweeting_H
#define Knowledge_Tweeting_H

#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <stack>
#include <map>
#include <set>
#include <algorithm>
#include "Identity_Parser.h"
#include "API_Generator.h"
using namespace std;
//++++++++++++++++++++++++++++++++++++++++++
//+++++++++++CryptoPP+++++++++++++++++++++++
/*

#include "../CRYPTOPPLib/modes.h"
#include "../CRYPTOPPLib/aes.h"
#include "../CRYPTOPPLib/filters.h"
#include <iostream>
#include "../CRYPTOPPLib/sha.h"
#include "../CRYPTOPPLib/filters.h"
#include "../CRYPTOPPLib/hex.h"
#include <string>
#include <sstream> 
#include "../CRYPTOPPLib/cryptlib.h"
using CryptoPP::Exception;
#include "../CRYPTOPPLib/hex.h"
using CryptoPP::HexEncoder;
using CryptoPP::HexDecoder;
#include "../CRYPTOPPLib/filters.h"
using CryptoPP::StringSink;
using CryptoPP::StringSource;
using CryptoPP::StreamTransformationFilter;
#include "../CRYPTOPPLib/des.h"
using CryptoPP::DES_EDE2;
#include "../CRYPTOPPLib/modes.h"
using CryptoPP::CBC_Mode;
#include "../CRYPTOPPLib/secblock.h"
using CryptoPP::SecByteBlock;
#include <iostream>
#include <string>
#include "../CRYPTOPPLib/modes.h"
#include "../CRYPTOPPLib/aes.h"
#include "../CRYPTOPPLib/filters.h"
#include "../CRYPTOPPLib/rsa.h"
#include <cstdint>
#include "../CRYPTOPPLib/integer.h"
#include "../CRYPTOPPLib/osrng.h"
*/

//++++++++++++++++++++++++++++++++++++++++++

class ServiceNetwork;
class TRelation;
class TService;

/*******************************************************************************
************************Thing Relationship class********************************
********************************************************************************
*****represents a link in the local graph that connect two/more services********
*******************************************************************************/

class TRelation {
	public:
	   string thingID;       // the unique ID of the thing
  	   string spaceID;       // the unique ID of the smart space
	   
  	   string name;          // given name of the relationship
  	   string owner;         // who established this (vendor/developer or discovered by thing)
  	   string category;      // Cooperative or Competitive
	   string type;          // Control/Drive/Support/Extend or Contest/Interfere/Refine/Subsume
	   string description;   // Short description by the vendor
	   TService * SPI1;      //first endpoint of the relationship, two services for now
           TService * SPI2;      //second endpoint of the relationship, two services for now

	   //constructor
  	   TRelation(string vname, TService* v1, TService* v2){name=vname;  SPI1=v1;  SPI2=v2;} 
  	   TRelation()                                        {SPI1=NULL;   SPI2=NULL;}   

	   //get methods
  	   string getRelationThingID()         const {return thingID;}
  	   string getRelationSpaceID()         const {return spaceID;}
	   string getRelationName()            const {return name;}
  	   string getRelationVendor()          const {return owner;}
  	   string getRelationCategory()        const {return category;}
  	   string getRelationType()            const {return type;}
  	   string getRelationDescription()     const {return description;}
           TService* getSPI1()                 const {return SPI1;}
  	   TService* getSPI2()                 const {return SPI2;}

           //set methods

	   void setRelationThingID(string value)     {thingID=value;}
  	   void setRelationSpaceID(string value)     {spaceID=value;}
  	   void setRelationName(string value)        {name=value;}
  	   void setRelationVendor(string value)      {owner=value;}
  	   void setRelationCategory(string value)    {category=value;}
  	   void setRelationType(string value)        {type=value;}
  	   void setRelationDescription(string value) {description=value;}
  	   void setSPI1(TService * v)                {SPI1=v;}
  	   void setSPI2(TService * v)                {SPI2=v;}

  	   void displayInfo(){
  	   	std::cout<<"Display relationship Info: "<<endl;
	   	std::cout<<"name: "<<name<<endl;
	   	std::cout<<"category: "<<category<<endl;
	   	std::cout<<"type: "<<type<<endl;
	   	std::cout<<"description: "<<description<<endl;
	   	std::cout<<"type: "<<type<<endl;
	   	std::cout<<"--------------------------"<<endl;
	   	std::cout<<endl;
  	   }
};

/*******************************************************************************
************************TService Thing Service class****************************
********************************************************************************
**************** vertex (node) in the local graph of services ******************
******* the node is either service offered by the thing or received by mate*****
********************************************************************************
*******************************************************************************/

class TService {
	public:
  	   string name;                  // given name of the service
  	   string thingID;               // the unique ID of the thing
  	   string entityID;              // the unique ID of the entity in the thing
  	   string spaceID;               // the unique ID of the smart space
	   string type;                  // Condition, Report or Action
	   string appCategory;           // Automation, Health, Eating, Office, Time Alarms, ...
	   string description;           // Short description by the vendor
           string API;                   // the API to access the service
           string vendor;                // the vendor of this service
           string Keywords;              // keywords that describe the service
           vector<TRelation *> TSLinks;  //set of links connected to the SPI
           bool visited;                 //to traverse the service network
  
	   //constructor
  	   TService()            { visited=false; 	Keywords.clear(); 			 }
  	   TService(string vname){ name=vname;   	visited=false; 		Keywords.clear();}

	   //get methods
  	   string getServiceName()            const {return name;}
  	   string getServiceThingID()         const {return thingID;}
  	   string getServiceEntityID()        const {return entityID;}
  	   string getServiceSpaceID()         const {return spaceID;}
  	   string getServiceType()            const {return type;}
  	   string getServiceAppCategory()     const {return appCategory;}
  	   string getServiceDescription()     const {return description;}
  	   string getServiceAPI()             const {return API;}
  	   string getServiceVendor()          const {return vendor;}
  	   string getServiceKeywords()        const {return Keywords;}
  	   vector<TRelation*> getRelations()  const {return TSLinks;}
  	   bool getVisited()                        {return visited;}

           //set methods
  	   void setServiceName(string value)        {name=value;}
  	   void setServiceThingID(string value)     {thingID=value;}
  	   void setServiceVendor(string value)      {vendor=value;}
  	   void setServiceEntityID(string value)    {entityID=value;}
  	   void setServiceSpaceID(string value)     {spaceID=value;}
  	   void setServiceType(string value)        {type=value;}
  	   void setServiceAppCategory(string value) {appCategory=value;}
  	   void setServiceDescription(string value) {description=value;}
  	   void setServiceAPI(string value)         {API=value;}
  	   void setServiceKeywords(string value)    {Keywords=value;}
           void setVisited(bool v)                  {visited=v;}
	   //add one for keywords

  	   TRelation* getLinkTo(string d){
    		 for (vector<TRelation *>::iterator it = TSLinks.begin(); it != TSLinks.end(); ++it){
      		     if ((*it)->getSPI2()->getServiceName() == d) return (*it);
    		 }
    		 return 0;
  	   }

  	   void addLink(TRelation * e){
  		 if (std::find(TSLinks.begin(), TSLinks.end(), e) != TSLinks.end())
  	   			std::cout<<"repeated edge for SPI node"<<std::endl;
		 else           TSLinks.push_back(e);
  	   }

  	   void removeLink(TRelation * e){
  			TSLinks.erase(remove(TSLinks.begin(),TSLinks.end(),e),TSLinks.end());
  	   }

  	   void removeLinkTo(string l){
    		TRelation * e = getLinkTo(l);
    		removeLink(e);
  	   }

  	   void displayInfo(){
  	   	std::cout<<"Display service Info: "	<<endl;
	   	std::cout<<"name: "			<<name<<endl;
	   	std::cout<<"thing id: "			<<thingID<<endl;
	   	std::cout<<"entity id: "		<<entityID<<endl;
	   	std::cout<<"space id: "			<<spaceID<<endl;
	   	std::cout<<"application category: "	<<appCategory<<endl;
	   	std::cout<<"type: "			<<type<<endl;
	   	std::cout<<"description: "		<<description<<endl;
	   	std::cout<<"API: "			<<API<<endl;
	   	std::cout<<"Keywords: "			<<Keywords<<endl;
	   	std::cout<<"--------------------------"	<<endl;
	   	std::cout<<endl;
  	   }
};


/*******************************************************************************
******************************Graph class***************************************
********************************************************************************
***********network of SPIs (vertices) connected via relationships (edges)*******
*******************************************************************************/

class ServiceNetwork {
	public:
  	   vector<TRelation*> 		Links;
  	   map<string, TService*> 	vertices;

  	   void addSPI(TService * newSPI){
    		if (vertices.find(newSPI->name) != vertices.end())
  	   		std::cout<<"repeated SPI"<<std::endl;
  		else    vertices[newSPI->name]=newSPI;
  	   }

  	   void addSPILabel(string label){
    		TService * v = new TService(label);  vertices[label]=v;
  	   }

  	   map<string, TService*> getVertices(){  return vertices; }
  
  	   vector<TRelation*> getLinks()       {  return Links;    }

  	   void addLink(string w, string from, string to){
    		if (vertices.find(from) != vertices.end() && vertices.find(to) != vertices.end()){
      			TService * vfrom = vertices.find(from)->second;
      			TService * vto = vertices.find(to)->second;
      			TRelation * e = new TRelation(w,vfrom,vto);
      			(*vfrom).addLink(e);
      			(*vto).addLink(e);
      			Links.push_back(e);
    		}else{
  	   		std::cout<<"One of SPIs does't exist"<<std::endl;
    		}
  	   }
  
  	   void addSPI_Link(TRelation* link){
    		if (vertices.find(link->SPI1->name) != vertices.end() && vertices.find(link->SPI2->name) != vertices.end()){    			
			TService * vfrom = vertices.find(link->SPI1->name)->second;
      			TService * vto = vertices.find(link->SPI2->name)->second;
      			(*vfrom).addLink(link);
      			(*vto).addLink(link);
      			Links.push_back(link);
    		}else{
  	   		std::cout<<"One of SPIs does't exist"<<std::endl;
    		}
  	   }
  
  	   TRelation * getLink(string from, string to){
    		if (vertices.find(from) != vertices.end() && vertices.find(to) != vertices.end()){
      			TService * v1 = vertices.find(from)->second;
      			TService* v2 = vertices.find(to)->second;
      			TRelation * e = (*v1).getLinkTo(to);
      			return e;
    		}else {
      			//need to handle case where vertices did not exist.
      			return 0;
    		}
  	   }

  	   void removeLink(string from, string to){
    		TRelation * e = getLink(from,to);
    		if (e != 0){
      			Links.erase(remove(Links.begin(),Links.end(),e),Links.end());
      			(*e).getSPI1()->removeLink(e);
      			(*e).getSPI2()->removeLink(e);
    		}
    		//handle case where Link did not exist?
  	   }

  	   TService* getSPIWithLabel(string l){
    		if (vertices.find(l) != vertices.end())  return vertices.find(l)->second;
    		else                                     return 0;
           }


  	   void removeSPI(string l){
    		TService * v = getSPIWithLabel(l);
    		if (v != 0){
      			vector<TRelation *> Links = getSPIWithLabel(l)->getRelations();
      			for (vector<TRelation *>::iterator it = Links.begin(); it != Links.end(); ++it){
    				string from = (*it)->getSPI1()->getServiceName();
    				string to = (*it)->getSPI2()->getServiceName();
    				removeLink(from,to);
      			}
      			vertices.erase(l);
    		}else { //Need to handle case where SPI does not exist.
     		}
  	   }


  	   void removeSPI(TService * l){
    		if (l != 0){
      			vector<TRelation *> Links = getSPIWithLabel(l->name)->getRelations();
      			for (vector<TRelation *>::iterator it = Links.begin(); it != Links.end(); ++it){
    				string from = (*it)->getSPI1()->getServiceName();
    				string to = (*it)->getSPI2()->getServiceName();
    				removeLink(from,to);
      			}
      			vertices.erase(l->name);
    		}else { //Need to handle case where SPI does not exist.
     		}
  	   }

  	   vector<TService *> getNeighbors(TService * v){
    		vector<TService *> destinations;
    		vector<TRelation *> Links = v->getRelations();
    		for (vector<TRelation *>::const_iterator it = Links.begin(); it != Links.end(); ++it) {
      			if ((*it)->getSPI1() != v) destinations.push_back((*it)->getSPI1());
      			if ((*it)->getSPI2() !=v)  destinations.push_back((*it)->getSPI2());
    		}      
    		destinations.push_back(v);
    		return destinations;
  	   }
};

/*******************************************************************************
******************Knowledge Tweeting Manager  class****************************
*******************************************************************************/

class Knowledge_Tweeting_Manager {
  public:
    	struct UnBService{
		string name;
		string vendor;
		string type;
		string keywords;
		int    match;
    	};
    	vector<TService> 	ThingServices;
    	vector<UnBService> 	ThingUnboundedServices;
    	vector<TRelation> 	ThingRelationships;
    	vector<TRelation> 	ThingUnboundedRelationships;
    	vector<string> 		Services_tweets;
    	vector<string> 		Relationships_tweets;
    	ServiceNetwork 		thingLocalGraph;

    	Knowledge_Tweeting_Manager();

	//+++++++++++++++Graph+++++++++++++++
	void printGraph(ServiceNetwork * t);
	bool isPath(ServiceNetwork * t, string from, string to);
	void discoverOpportunity(ServiceNetwork * t, TService * node);
	int test();

        //+++++++++++++++Service/relation++++++++++++++
    	int findMatch(UnBService);
    	void parse_ThingServices();
    	void parse_ThingRelationships();
	void parse_ThingUnboundedServices();
    	void parse_mateServicesTweet(string);
    	void parse_mateRelationsTweet(string);
    	void display_mateInfo();

        //+++++++++++++++Secure++++++++++++++
        void CRYPTO_security();
	string Decrypt(string Interaction);
	string Encrypt(string Interaction);
};

#endif

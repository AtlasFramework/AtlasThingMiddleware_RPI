/*
* FileName: Knowledge_Tweeting_Manager.cpp
* Description: this module handles the relationships and links between the offered services and others' services
* Unit: the knowledge engine - the DDL sublayer - Atlas thing architecture
* Date: August 2020
*/

#include "Knowledge_Tweeting_Manager.h"
#include <iostream>
#include <ctime>

Knowledge_Tweeting_Manager::Knowledge_Tweeting_Manager(){
        Services_tweets.clear();
        Relationships_tweets.clear();
        ThingServices.clear();
        ThingRelationships.clear();
	ThingUnboundedServices.clear();
    	ThingUnboundedRelationships.clear();
}

void Knowledge_Tweeting_Manager::printGraph(ServiceNetwork * t){
  	map<string,TService*> vertices = t->getVertices();
  	for (map<string, TService*>::iterator it = vertices.begin(); it != vertices.end(); ++it){
    		cout << it->first <<": ";
    		vector<TRelation *> Links = it->second->getRelations();
    		for (vector<TRelation *>::iterator jit = Links.begin(); jit != Links.end(); ++jit){
        		string re = (*jit)->getRelationName();
        		string l1 = (*jit)->getSPI1()->getServiceName();
    			string l2=(*jit)->getSPI2()->getServiceName();
    			if (l1 != it->first){cout << l1 << "(" << re <<") ";}
    			if (l2 != it->first){cout << l2 << "(" << re <<") ";}
    		}
    		cout << endl;
    	}
}

bool  Knowledge_Tweeting_Manager::isPath(ServiceNetwork * t, string from, string to) {
  	TService * vfrom = t->getSPIWithLabel(from);
  	TService * vto = t->getSPIWithLabel(to);
  	if (vfrom == 0 || vto == 0) { return false;}
  	if (from==to) { return true;}
  	map<string, TService*> vertices = t->getVertices();
  	vector<TRelation *> Links = t->getLinks();
  	vector<TService *> verticesToCheck;
  	verticesToCheck.push_back(vfrom);
  	vertices.erase(from);

  	while (verticesToCheck.size() != 0){
    		vector<TService *> destinations = t->getNeighbors(verticesToCheck[0]);
    		verticesToCheck.erase(verticesToCheck.begin());
    		for (vector<TService *>::const_iterator it = destinations.begin(); it != destinations.end(); ++it) {
        		if (vertices.find((*it)->getServiceName()) != vertices.end()) {
     	  			if ((*it)->getServiceName()==to) { return true; }
    	  			verticesToCheck.push_back((*it));
     	  			vertices.erase((*it)->getServiceName());
	    		}
    		}
  	}
  	return false;
}


void  Knowledge_Tweeting_Manager::discoverOpportunity(ServiceNetwork * t, TService * node) {
  	//if (node == null) { return; }
  	std::cout<<"Singleton API: "<<node->name<<std::endl;
  	std::vector<string> setVisit;// = new std::set<TService>();
  	std::stack<TService*> st;// = new std::stack<TService>();
  	st.push(node);
  	while (!st.empty()) {
        	TService* curr = st.top();
        	st.pop();
        	if(!(std::find(setVisit.begin(), setVisit.end(), curr->API) != setVisit.end())) {         
            		std::cout<<curr->name<<"->";
            		setVisit.push_back(curr->name);
        	}
        	vector<TService*> destinations = t->getNeighbors(curr);
        	for (int i = 0; i < destinations.size(); i++) {
            		TService* neighbor = destinations[i];
    			if(!(std::find(setVisit.begin(), setVisit.end(), neighbor->name) != setVisit.end())) { st.push(neighbor); }
        	}
    	}
    	std::cout<<std::endl;
}


void Knowledge_Tweeting_Manager::parse_ThingServices(){
	API_Generator APIGen;
	APIGen.Generate_ServicesAPIs();
	IoTDDL_Parser DDLM;
	int entityId = 1;

	int num_Entities = 1;
	string num_of_Entities = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Thing","Structural_Metadata","Number_Entities");
    	num_Entities =  atoi(num_of_Entities.c_str());

	for(int j=0;j<num_Entities;j++){

		//get the number of services under this entity
		int num_Services = 1;
		string num_of_Services = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j+1),"Services","Number_Services");
  	   	
		num_Services =  atoi(num_of_Services.c_str());

		for(int i=0;i<num_Services;i++){
			entityId = j + 1;
			TService TS;
		  	TS.setServiceThingID    (DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Thing","Descriptive_Metadata","Thing_ATID"));
		  	TS.setServiceSpaceID    (DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Thing","Descriptive_Metadata","Thing_SSID"));
		  	TS.setServiceVendor     (DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(entityId),"Descriptive_Metadata","Vendor"));
		  	TS.setServiceEntityID   (DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(entityId),"Descriptive_Metadata","ID"));
		  	//TS.setServiceAPI        (APIGen.APIs[i]);


			TS.setServiceName       (DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(entityId),"Services","Service_"+std::to_string(i+1),"Name"));
		  	TS.setServiceType       (DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(entityId),"Services","Service_"+std::to_string(i+1),"Type"));
			TS.setServiceAppCategory(DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(entityId),"Services","Service_"+std::to_string(i+1),"AppCategory"));
			TS.setServiceDescription(DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(entityId),"Services","Service_"+std::to_string(i+1),"Description"));
			TS.setServiceKeywords   (DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(entityId),"Services","Service_"+std::to_string(i+1),"Keywords"));
			


			//======== API for service

			int Service_Number = i;
			string Service_Name              = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j+1),"Services","Service_"+std::to_string(i+1),"Name");

			string Service_InputTypes        = "NULL";
			string Service_InputDescription  = "NULL";
			string Service_InputRange        = "NULL";
		    	int num_Inputs = 0;
		    	string num_of_Inputs = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j+1),"Services","Service_"+std::to_string(i+1),"Service_NumberInputs");
		    	num_Inputs =  atoi(num_of_Inputs.c_str());

			if(num_Inputs > 0){
			      Service_InputTypes        = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j+1),"Services","Service_"+std::to_string(i+1),"Service_InputTypes");
			      Service_InputDescription  = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j+1),"Services","Service_"+std::to_string(i+1),"Service_InputDescriptions");
			      //Service_InputRange      = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j+1),"Services","Service_"+std::to_string(i+1),"Service_InputRange");
			}
		
			string Service_OutputTypes       = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j+1),"Services","Service_"+std::to_string(i+1),"OutputType");
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
				string Service_OutputDescription = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j+1),"Services","Service_"+std::to_string(i+1),"OutputDescription");
				//string Service_OutputRange     = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entity_"+std::to_string(num_Entities),"Resource_Service","Service_"+std::to_string(i+1),"OutputRange");
							
				API_Output = Service_OutputDescription+","+Service_OutputTypes+", NULL"; //+Service_OutputRange;
			}
	
			string Generated_API = Service_Name+":["+API_Input+"]:("+API_Output+")";
			
			TS.setServiceAPI        (Generated_API);

			//=======================





			//TS.displayInfo();

				string x1 = "\"Tweet Type\" : \"Service\"";
				string x2 = "\"Name\" : \""		+TS.getServiceName()+"\"";
				string x3 = "\"Thing ID\" : \""		+TS.getServiceThingID()+"\"";
				string x4 = "\"Entity ID\" : \""	+TS.getServiceEntityID()+"\"";
				string x5 = "\"Space ID\" : \""		+TS.getServiceSpaceID()+"\"";
				string x6 = "\"Vendor\" : \""		+TS.getServiceVendor()+"\"";
				string x7 = "\"API\" : \""		+TS.getServiceAPI()+"\"";
				string x8 = "\"Type\" : \""		+TS.getServiceType()+"\"";
				string x9 = "\"AppCategory\" : \""	+TS.getServiceAppCategory()+"\"";
				string x10 = "\"Description\" : \""	+TS.getServiceDescription()+"\"";
				string x11 = "\"Keywords\" : \""	+TS.getServiceKeywords()+"\"";

				string JSONs = " { " + x1 + "," + x2 + ","+ x3 + ","+ x4 + ","+ x5 + ","+ x6 + ","+ x7 + ","+ x8 + ","+ x9 + ","+ x10 + ","+ x11 + " }";
				Services_tweets.push_back(JSONs);
				ThingServices.push_back(TS);

				thingLocalGraph.addSPI(&ThingServices[ThingServices.size()-1]);
		}
	}
    	
}

void Knowledge_Tweeting_Manager::parse_mateServicesTweet(string JSONs){
        char json[4048];
	strcpy(json, JSONs.c_str());
	Document document;  
	char buffer[4048];
	memcpy(buffer, json, sizeof(json));
	if (document.ParseInsitu(buffer).HasParseError())  return;

	assert(document.IsObject());
	assert(document["Thing ID"].IsString());
	printf("ThingID = %s \n", document["Thing ID"].GetString());
	string thingID = document["Thing ID"].GetString();
	string spaceID = document["Space ID"].GetString();
        string servicename = document["Name"].GetString();

	for(int i=0;i<ThingServices.size();i++){
		if((ThingServices[i].getServiceThingID() == thingID) &&
		   (ThingServices[i].getServiceSpaceID() == spaceID) &&
		   (ThingServices[i].getServiceName() == servicename)){
			cout<<"this service "+servicename+ " from " + thingID+ " already in the network"<<endl;
			return;
		}
	}

	TService TS;
  	TS.setServiceName(servicename);
  	TS.setServiceThingID(thingID);
  	TS.setServiceEntityID(document["Entity ID"].GetString());
  	TS.setServiceSpaceID(spaceID);
  	TS.setServiceAPI(document["API"].GetString());
	TS.setServiceType(document["Type"].GetString());
	TS.setServiceDescription(document["Description"].GetString());
	TS.setServiceVendor(document["Vendor"].GetString());
	TS.setServiceAppCategory(document["AppCategory"].GetString());
	TS.setServiceKeywords(document["Keywords"].GetString());

	cout<<"adding new service "+servicename+ " from " + thingID<<endl;
	//TS.displayInfo();
        ThingServices.push_back(TS);
	
	//thingLocalGraph.addSPI(&TS);
}


void Knowledge_Tweeting_Manager::parse_ThingUnboundedServices(){
	
	IoTDDL_Parser DDLM;
    	int num_Entities = 1;
    	string num_of_Entities = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Thing","Structural_Metadata","Number_Entities");
    	num_Entities =  atoi(num_of_Entities.c_str());

	for(int j=1;j<=num_Entities;j++){
		int num_UBS = 0;  
	    	string num_of_UBS = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"UnboundedService","Number_UnboundedServices");
	    	num_UBS =  atoi(num_of_UBS.c_str());
		if(num_UBS == 0) return;
	    	for(int i=1;i<=num_UBS;i++){
			UnBService UBS;
	  	   	UBS.name     = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"UnboundedService","UB_"+std::to_string(i),"Name");
	  	   	UBS.vendor   = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"UnboundedService","UB_"+std::to_string(i),"MatchVendor");
	  	   	UBS.type     = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"UnboundedService","UB_"+std::to_string(i),"MatchType");
	  	   	UBS.keywords = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"UnboundedService","UB_"+std::to_string(i),"MatchKeywords");
	  	   	string vmatch= DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"UnboundedService","UB_"+std::to_string(i),"MatchValue");
	  	   	UBS.match = atoi(vmatch.c_str());
			ThingUnboundedServices.push_back(UBS);
	     	}
	}
}


void Knowledge_Tweeting_Manager::parse_ThingRelationships(){

	parse_ThingUnboundedServices();
	IoTDDL_Parser DDLM;
    	int num_Entities = 1;
    	string num_of_Entities = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Thing","Structural_Metadata","Number_Entities");
    	num_Entities =  atoi(num_of_Entities.c_str());

	for(int j=1;j<=num_Entities;j++){

		int num_relations = 0;  
	    	string num_of_services = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"Relationships","Number_Relationships");
	    	num_relations =  atoi(num_of_services.c_str());

		if(num_relations == 0) return;

	    	for(int i=1;i<=num_relations;i++){
			TRelation TR;

			TR.setRelationThingID    (DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Thing","Descriptive_Metadata","Thing_ATID"));
			TR.setRelationSpaceID    (DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Thing","Descriptive_Metadata","Thing_SSID"));
	  	   	TR.setRelationName       (DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"Relationships","Relationship_"+std::to_string(i),"Name"));
	  	   	TR.setRelationType       (DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"Relationships","Relationship_"+std::to_string(i),"type"));
	  	   	TR.setRelationCategory   ("Cooperative");
	  	   	//TR.setRelationCategory   (DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"Relationships","Relationship_"+std::to_string(i),"category"));
	  	   	TR.setRelationVendor     (DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"Relationships","Relationship_"+std::to_string(i),"Establisher"));
	  	   	TR.setRelationDescription(DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"Relationships","Relationship_"+std::to_string(i),"Description"));
	  	   	
			string x1 = "\"Tweet Type\" : \"Relationship\"";
			string x9 = "\"Thing ID\" : \""		+TR.getRelationThingID()+"\"";
			string x10 = "\"Space ID\" : \""		+TR.getRelationSpaceID()+"\"";			
			string x2 = "\"Name\" : \""+TR.getRelationName()+"\"";
			string x3 = "\"Owner\" : \""+TR.getRelationVendor()+"\"";
			string x4 = "\"Category\" : \""+TR.getRelationCategory()+"\"";
			string x5 = "\"Type\" : \""+TR.getRelationType()+"\"";
			string x6 = "\"Description\" : \""+TR.getRelationDescription()+"\"";
			string x7 = "\"FS name\" : \""+DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"Relationships","Relationship_"+std::to_string(i),"Input1")+"\"";
			string x8 = "\"SS name\" : \""+DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"Relationships","Relationship_"+std::to_string(i),"Input2")+"\"";
					
			string JSONs = " { " + x1 + "," + x9 + "," + x10 + "," + x2 + ","+ x3 + ","+ x4 + ","+ x5 + ","+ x6 + ","+ x7 + "," + x8 + " }";
			cout<<"new full relation added"<<endl;
			Relationships_tweets.push_back(JSONs);
			ThingRelationships.push_back(TR);




			//need to be updated>>>>>

			//string inputNames       = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"Relationships","Relationship_"+std::to_string(i),"Inputs");
	  	   	//string inputDescription = DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Entities","Entity_"+std::to_string(j),"Relationships","Relationship_"+std::to_string(i),"InputDescription");
			/*
		      	vector<string> Names;
		      	vector<string> Description;
		      	string s;
		      	istringstream iNames(inputNames);
		      	istringstream iDescription(inputDescription);        //the services first then the unbounded service
		     	while(getline(iNames,s,','))        Names.push_back(s);
		     	while(getline(iDescription,s,','))  Description.push_back(s);
		
			int numberservice = 0;
			int firstSer = 1,secondSer = 1; //0 for service and 1 for unbounded
			for(int i=0;i<Description.size();i++){
				if(Description[i] == "Service"){
					//cout<<"input is normal service named"<<Names[i]<<endl;
					for(int j=0;j<ThingServices.size();j++){
						if(ThingServices[j].getServiceName() == Names[i]){
							//cout<<"found a match in the vector"<<endl;
							numberservice++;
							if(numberservice == 1)     { firstSer = 0; TR.setSPI1(&ThingServices[j]); }
							else if(numberservice == 2){ secondSer= 0; TR.setSPI2(&ThingServices[j]); }
							break;
						}
					}//end of ThingServices
				}
				else if(Description[i] == "UnboundedService"){ 
					//cout<<"input is unbounded service named"<<Names[i]<<endl;
					numberservice++;
					if(numberservice == 1){ 
						for(int x=0;x<ThingUnboundedServices.size();x++){
							if(Names[i] == ThingUnboundedServices[x].name){
								int index = findMatch(ThingUnboundedServices[x]); 
								if(index == -1) break;
								TR.setSPI1(&ThingServices[index]); 
								firstSer = 0;
							}
						}
					}
					else if(numberservice == 2){ 
						for(int x=0;x<ThingUnboundedServices.size();x++){
							if(Names[i] == ThingUnboundedServices[x].name){
								int index = findMatch(ThingUnboundedServices[x]); 
								if(index == -1) break;
								TR.setSPI2(&ThingServices[index]); 
								secondSer = 0;
							}
						}
					}
				}
				else                                          
					cout<<"undefined type of service listed in the ddl"<<endl;
			}//the end of Description


			if(firstSer == 0 && secondSer == 0){
				string x1 = "\"Tweet Type\" : \"Relationship\"";
				string x2 = "\"Name\" : \""+TR.getRelationName()+"\"";
				string x3 = "\"Owner\" : \""+TR.getRelationVendor()+"\"";
				string x4 = "\"Category\" : \""+TR.getRelationCategory()+"\"";
				string x5 = "\"Type\" : \""+TR.getRelationType()+"\"";
				string x6 = "\"Description\" : \""+TR.getRelationDescription()+"\"";
				string x7 = "\"FS name\" : \""+TR.getSPI1()->getServiceName()+"\"";
				string x8 = "\"FS tID\" : \""+TR.getSPI1()->getServiceThingID()+"\"";
				string x9 = "\"FS sID\" : \""+TR.getSPI1()->getServiceSpaceID()+"\"";
				string x10 = "\"SS name\" : \""+TR.getSPI2()->getServiceName()+"\"";
				string x11 = "\"SS tID\" : \""+TR.getSPI2()->getServiceThingID()+"\"";
				string x12 = "\"SS sID\" : \""+TR.getSPI2()->getServiceSpaceID()+"\"";
					
				string JSONs = " { " + x1 + "," + x2 + ","+ x3 + ","+ x4 + ","+ x5 + ","+ x6 + ","+ x7 + "," + x8 + ","+ x9 + ","+ x10 + ","+ x11 + ","+ x12 + " }";
				cout<<"new full relation added"<<endl;
				Relationships_tweets.push_back(JSONs);
				ThingRelationships.push_back(TR);
				//thingLocalGraph.addSPI_Link(&ThingRelationships[ThingRelationships.size()-1]);
			}else{
				cout<<"new unbounded relation added"<<endl;
				ThingUnboundedRelationships.push_back(TR);
			}
			*/
	     	}

	}
}

void Knowledge_Tweeting_Manager::parse_mateRelationsTweet(string JSONs){
        char json[4048];
	strcpy(json, JSONs.c_str());
	Document document;  
	char buffer[4048];
	memcpy(buffer, json, sizeof(json));
	if (document.ParseInsitu(buffer).HasParseError())  return;

	assert(document.IsObject());
	assert(document["Thing ID"].IsString());
	printf("ThingID = %s \n", document["Thing ID"].GetString());
	



/*

string name 		= document["Name"].GetString();
	string firstSName 	= document["FS name"].GetString();
        string SecondSName 	= document["SS name"].GetString();
	string firstSID 	= document["FS tID"].GetString();
        string SecondSID 	= document["SS tID"].GetString();
	string firstSPID 	= document["FS sID"].GetString();
        string SecondSPID 	= document["SS sID"].GetString();

	for(int i=0;i<ThingRelationships.size();i++){
		if((ThingRelationships[i].getRelationName() 		 == name)        &&
		   (ThingRelationships[i].getSPI1()->getServiceName()    == firstSName)  &&
		   (ThingRelationships[i].getSPI2()->getServiceName() 	 == SecondSName) &&
		   (ThingRelationships[i].getSPI1()->getServiceThingID() == firstSID)    &&
		   (ThingRelationships[i].getSPI2()->getServiceThingID() == SecondSID)   &&
		   (ThingRelationships[i].getSPI1()->getServiceSpaceID() == firstSPID)   &&
		   (ThingRelationships[i].getSPI2()->getServiceSpaceID() == SecondSPID) ){
			cout<<"this relation "+name+ " between " + firstSName+ " and "+SecondSName +" already in the network"<<endl;
			return;
		}
	}

	//need to handle case of relationship but the service not exist yet
	TRelation TR;
  	TR.setRelationName(name);
  	TR.setRelationType(document["Type"].GetString());
  	TR.setRelationCategory(document["Category"].GetString());
  	TR.setRelationVendor(document["Owner"].GetString());
  	TR.setRelationDescription(document["Description"].GetString());
	for(int j=0;j<ThingServices.size();j++){
		if(ThingServices[j].getServiceName() == document["First service name"].GetString()){
			TR.setSPI1(&ThingServices[j]);	  break;
		}
	}
	for(int j=0;j<ThingServices.size();j++){
		if(ThingServices[j].getServiceName() == document["Second service name"].GetString()){
			TR.setSPI2(&ThingServices[j]);	  break;
		}
	}

	cout<<"this relation "+name+ " between " + firstSName+ " and "+SecondSName +" already in the network"<<endl;
        ThingRelationships.push_back(TR);
	//thingLocalGraph.addSPI_Link(*TR);
*/
}


int Knowledge_Tweeting_Manager::findMatch(UnBService UBS){
        vector<string> kwords;
	string s;
        istringstream ikwords(UBS.keywords);
        while(getline(ikwords,s,','))  kwords.push_back(s);

	int index  = -1;
	int maxMatch = 0;
        for(int i=0;i<ThingServices.size();i++){
		int currentMatch = 0;
		if((UBS.vendor == "*") || (UBS.vendor == ThingServices[i].getServiceVendor())) currentMatch+=30;
		if((UBS.type == "*")   || (UBS.type   == ThingServices[i].getServiceType()))   currentMatch+=30;
		if(UBS.keywords == "*")
			currentMatch+=30;
		else{
		        vector<string> skwords;
			string s2;
		        istringstream iskwords(ThingServices[i].getServiceKeywords());
		        while(getline(iskwords,s2,','))  skwords.push_back(s2);
			for(int x=0;x<kwords.size();x++)
				for(int y=0;y<skwords.size();y++)
					if(kwords[x] == skwords[x])   currentMatch+=20;
		}

		if(currentMatch >= maxMatch) { maxMatch = currentMatch; index = i;}
	}
	if(maxMatch < UBS.match)  return -1;
	return index;
}





















//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

string Knowledge_Tweeting_Manager::Encrypt(string plaintext){
/*

    byte key[ CryptoPP::AES::DEFAULT_KEYLENGTH ], iv[ CryptoPP::AES::BLOCKSIZE ];
    memset( key, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH );
    memset( iv, 0x00, CryptoPP::AES::BLOCKSIZE );
    std::string ciphertext;
    CryptoPP::AES::Encryption aesEncryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption( aesEncryption, iv );
    CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink(ciphertext));
    stfEncryptor.Put(reinterpret_cast<const unsigned char*>( plaintext.c_str()), plaintext.length()+1);
    stfEncryptor.MessageEnd();
    return ciphertext;
*/
}

string Knowledge_Tweeting_Manager::Decrypt(string ciphertext){
/*

    byte key[ CryptoPP::AES::DEFAULT_KEYLENGTH ], iv[ CryptoPP::AES::BLOCKSIZE ];
    memset( key, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH );
    memset( iv, 0x00, CryptoPP::AES::BLOCKSIZE );
    string decryptedtext;
    CryptoPP::AES::Decryption aesDecryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption( aesDecryption, iv );
    CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink( decryptedtext ) );
    stfDecryptor.Put( reinterpret_cast<const unsigned char*>( ciphertext.c_str() ), ciphertext.size() );
    stfDecryptor.MessageEnd();
    return decryptedtext;
*/
}




void Knowledge_Tweeting_Manager::CRYPTO_security(){

/*
//write and read key and IV from file
byte key[ CryptoPP::AES::DEFAULT_KEYLENGTH ], 
      iv[ CryptoPP::AES::BLOCKSIZE ];
::memset( key, 0x01, CryptoPP::AES::DEFAULT_KEYLENGTH );
::memset(  iv, 0x01, CryptoPP::AES::BLOCKSIZE );


string PlainText = "Turn on boiler for duration:[Duration,integer,(1:20)]:(null)";   // Message M
cout <<"Plain Text:"<< PlainText <<" - size (" << PlainText.size() << " bytes)" << endl;

string CipherText;  // Cipher Text Sink

CryptoPP::CBC_Mode<CryptoPP::AES>::Encryption Encryptor( key, sizeof(key), iv );  // Encryption

CryptoPP::StringSource( PlainText, true, new CryptoPP::StreamTransformationFilter( Encryptor, new CryptoPP::StringSink( CipherText ) ) // StreamTransformationFilter
); // StringSource

//Write data -- you can hash till write.close to read directly from the previously stored key
ofstream write ("file.txt", ios::out | ios::binary);
write.write((char*)key,sizeof(key));
write.write((char*)iv,sizeof(iv));
int at = CipherText.length();
write.write(CipherText.c_str(),at); 
write.close();


CipherText.erase();
byte key1[ CryptoPP::AES::DEFAULT_KEYLENGTH ], iv1[ CryptoPP::AES::BLOCKSIZE ];   //Using new key and iv later;
*/

//Read data
/*ifstream read ("file.txt", ios::in | ios::binary); 
read.seekg (0, ios::end);
int fsize = read.tellg();
read.seekg (0, ios::beg);
read.read((char*)key1,sizeof(key));
read.read((char*)iv1,sizeof(iv));
int toRead = fsize - sizeof(key) - sizeof(iv);
vector<char> bData(toRead);
read.read(&bData[0],toRead);
read.close();

string RecoveredText;  // Recovered Text Sink

// Decryption
CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption Decryptor( key1, sizeof(key1), iv1 );

CryptoPP::StringSource( &bData[0], true, new CryptoPP::StreamTransformationFilter( Decryptor, new CryptoPP::StringSink( RecoveredText )) // StreamTransformationFilter
); // StringSink

// Debug
cout << "Recovered Text:" << endl;
cout << "  '" << RecoveredText << "'" << endl;
cout << endl;
*/





/*

//ASE generate key

    //Generating a random key
    byte key[ CryptoPP::AES::DEFAULT_KEYLENGTH ], iv[ CryptoPP::AES::BLOCKSIZE ];
    memset( key, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH );
    memset( iv, 0x00, CryptoPP::AES::BLOCKSIZE );
    CryptoPP::AutoSeededRandomPool rnd;
    rnd.GenerateBlock(key,CryptoPP::AES::DEFAULT_KEYLENGTH );
    rnd.GenerateBlock(iv,CryptoPP::AES::BLOCKSIZE);
  
    for( int i = 0; i < key.size(); i++ ) {
        std::cout << "0x" << std::hex << (0xFF & static_cast<byte>(key[i])) << " ";
    }

    for( int i = 0; i < iv.size(); i++ ) {
        std::cout << "0x" << std::hex << (0xFF & static_cast<byte>(iv[i])) << " ";
    }




    std::string plaintext = "Now is the time for all good men to come to the aide...";
    std::cout << plaintext <<" - size (" << plaintext.size() << " bytes)" << std::endl;

    std::string ciphertext;
    std::string decryptedtext;


    // Create Cipher Text
    CryptoPP::AES::Encryption aesEncryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption( aesEncryption, iv );
    CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink(ciphertext));
    stfEncryptor.Put(reinterpret_cast<const unsigned char*>( plaintext.c_str()), plaintext.length()+1);
    stfEncryptor.MessageEnd();


    // Dump Cipher Text
    std::cout << "Cipher Text (" << ciphertext.size() << " bytes)" << std::endl;
    for( int i = 0; i < ciphertext.size(); i++ ) {
        std::cout << "0x" << std::hex << (0xFF & static_cast<byte>(ciphertext[i])) << " ";
    }
    std::cout << std::endl << std::endl;



    std::cout << "Cipher Text" << ciphertext.c_str() << std::endl;


    // Decrypt
    CryptoPP::AES::Decryption aesDecryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption( aesDecryption, iv );
    CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink( decryptedtext ) );
    stfDecryptor.Put( reinterpret_cast<const unsigned char*>( ciphertext.c_str() ), ciphertext.size() );
    stfDecryptor.MessageEnd();

    // Dump Decrypted Text
    std::cout << "Decrypted Text: " << decryptedtext <<std::endl;

*/


/*

//ASE default key of Crypto++ library

    //Key and IV setup
    //AES encryption uses a secret key of a variable length (128/196/256-bit). 
    //This key is secretly exchanged between two parties before communication begins. 
    //DEFAULT_KEYLENGTH= 16 bytes
    byte key[ CryptoPP::AES::DEFAULT_KEYLENGTH ], iv[ CryptoPP::AES::BLOCKSIZE ];
    memset( key, 0x00, CryptoPP::AES::DEFAULT_KEYLENGTH );
    memset( iv, 0x00, CryptoPP::AES::BLOCKSIZE );


    string plaintext = "Turn on boiler for duration:[Duration,integer,(1:20)]:(null)";   // Message M
    cout <<"Plain Text:"<< plaintext <<" - size (" << plaintext.size() << " bytes)" << endl;


    std::string ciphertext;
    std::string decryptedtext;


    // Create Cipher Text
    CryptoPP::AES::Encryption aesEncryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Encryption cbcEncryption( aesEncryption, iv );
    CryptoPP::StreamTransformationFilter stfEncryptor(cbcEncryption, new CryptoPP::StringSink(ciphertext));
    stfEncryptor.Put(reinterpret_cast<const unsigned char*>( plaintext.c_str()), plaintext.length()+1);
    stfEncryptor.MessageEnd();


    // Dump Cipher Text
    std::cout << "Cipher Text (" << ciphertext.size() << " bytes)" << std::endl;
    for( int i = 0; i < ciphertext.size(); i++ ) {
        std::cout << "0x" << std::hex << (0xFF & static_cast<byte>(ciphertext[i])) << " ";
    }
    std::cout << std::endl << std::endl;



    std::cout << "Cipher Text" << ciphertext.c_str() << std::endl;


    // Decrypt
    CryptoPP::AES::Decryption aesDecryption(key, CryptoPP::AES::DEFAULT_KEYLENGTH);
    CryptoPP::CBC_Mode_ExternalCipher::Decryption cbcDecryption( aesDecryption, iv );
    CryptoPP::StreamTransformationFilter stfDecryptor(cbcDecryption, new CryptoPP::StringSink( decryptedtext ) );
    stfDecryptor.Put( reinterpret_cast<const unsigned char*>( ciphertext.c_str() ), ciphertext.size() );
    stfDecryptor.MessageEnd();

    // Dump Decrypted Text
    std::cout << "Decrypted Text: " << decryptedtext <<std::endl;
*/

}








/*
    	std::cout<<"Graph 1"<<std::endl;
    	ServiceNetwork g;
    	TService* s1 = new TService("S1");
    	TService* s2 = new TService("S2");
    	TService* s3 = new TService("S3");
    	TService* s4 = new TService("S4");

    	TRelation* r1 = new TRelation("R1",s1,s2);
    	TRelation* r2 = new TRelation("R2",s3,s2);
  	g.addSPI(s1); g.addSPI(s2); g.addSPI(s3); g.addSPI(s4);
  	g.addSPI_Link(r1); g.addSPI_Link(r2);
    	printGraph(&g);

    	TRelation* r3 = new TRelation("R3",s4,s2); 
	g.addSPI_Link(r3);
    	printGraph(&g);
    --------------------------------
    	std::cout<<"Graph 2"<<std::endl;
    	ServiceNetwork g2;
    
    	TService * v6 = new TService("vx");
    	TService * v7 = new TService("v2");
    	TRelation * e3 = new TRelation(1,v6,v7);
  	g2.addSPI(v6); g2.addSPI(v7);
  	g2.addSPI_Link(e3); g2.addSPI_Link(e3);
  	printGraph(&g2);
    
	std::cout<<"Merging Graphs"<<std::endl;
	for(map<string, TService*>::iterator it = g2.vertices.begin(); it != g2.vertices.end(); ++it)
  		g.addSPI(it->second);
  		
  	for (unsigned i=0; i<g2.Links.size(); i++)
    		g.addSPI_Link(g2.Links.at(i));
    
  	printGraph(&g);
  	
	std::cout<<"Remove Node vy"<<std::endl;
    	//g.removeSPI(v2);
  	//printGraph(&g);
  	discoverOpportunity(&g,v2);
*/

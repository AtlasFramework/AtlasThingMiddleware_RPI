#ifndef OBJECTSENGINE_H_
#define OBJECTSENGINE_H_

#include "IoTDDL_Parser.h"
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <thread>
#include <ctime>
using namespace std;

class Objects_Engine {
	IoTDDL_Parser DDLM;
  public:
    	Objects_Engine();
	void Start_DeviceManagement();

	//Atlas Thing Descriptive Metadata section of IoT-DDL
	void createAtlasThing_DesMetadata_Object();
	void updateAtlasThing_DesMetadata_Object();

	//Atlas Entity 1 Descriptive Metadata section of IoT-DDL
	void createAtlasEntity_DesMetadata_Object();
	void updateAtlasEntity_DesMetadata_Object();
	
	//Atlas Attachment DataRepository section of IoT-DDL
	void createAtlasAttachment_DataRepository_Object();
	void updateAtlasAttachment_DataRepository_Object();
	
	void createAtlasEntity_ResouceService_Object();
	//void createAtlasAttachment_DataRepository_Object();
	void createAtlasAttachment_VendorOnlineDoc_Object();
	char* read_AtlasEntity_ResouceService_Attribute(int instancenumber, char* attribute);
	void update_AtlasEntity_API(int instancenumber, char* attribute);
	void display_AtlasObjects();
	void connect_OMA_Server();
   	 ~Objects_Engine();
};

#endif /* LWM2MCLIENT_H_ */
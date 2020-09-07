#include "Objects_Engine.h"
extern "C" {
#include "../OMALib/core/objectHandler.h"
}



Objects_Engine::Objects_Engine(){
}

void Objects_Engine::Start_DeviceManagement(){

        //these functions are implemented under /lib/OMALib/core/objectHandler
        //each Atlas object is defined as standalone object (.c) in /lib/OMALib/core/ directory

	/*
		create_Atlas_Thing_DesMetadata_Object_par(DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Thing","Descriptive_Metadata","Thing_Vendor").c_str(),
			                                  DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Thing","Descriptive_Metadata","Thing_Owner").c_str(),
			                                  DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Thing","Descriptive_Metadata","Thing_Name").c_str(),
			                                  DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Thing","Descriptive_Metadata","Thing_ATID").c_str(),
			                                  DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Thing","Descriptive_Metadata","Thing_ShortDescription").c_str());
	*/

	string thingName       	= DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Thing","Descriptive_Metadata","Thing_ATID");
		
	string serverIP   	= DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Thing","Administrative_Metadata","Network_Manager","OMA_DM","IP");

	string serverPort 	= DDLM.parseXMLTag("Atlas_IoTDDL","Atlas_Thing","Administrative_Metadata","Network_Manager","OMA_DM","Port");
	cout<<"creating connection with server "<<serverIP <<":"<<serverPort<<endl;	
	connectServer(thingName.c_str(),serverIP.c_str(),serverPort.c_str());
}

Objects_Engine::~Objects_Engine(){
	//delete_Atlas_Objects();
}






























/*	
void Objects_Engine::createAtlasAttachment_VendorOnlineDoc_Object(){

	Data_Object = lwm2m_data_new(1);
	//IoTDDL_Parser DDLM;
	AtlasThingAttachment_VendorOnlineDoc = get_Atlas_Attachment_VendorOnlineDoc_object();
    if (NULL == AtlasThingAttachment_VendorOnlineDoc){
		cout << "Failed to create AtlasAttachment_vendorOnline object"<< endl;
    }else{ cout<<"AtlasAttachment_vendorOnline"<<endl; }
	
	Data_Object->type = LWM2M_TYPE_STRING;
    Data_Object->id = 0;
    lwm2m_data_encode_string(DDLM.retriveXMLTag("Atlas_Thing","Vendor_OnlineDocumentation","Vendor").c_str(), Data_Object);
	AtlasThingAttachment_VendorOnlineDoc->writeFunc(0,0, Data_Object, AtlasThingAttachment_VendorOnlineDoc);
	Data_Object->id = 1;
    lwm2m_data_encode_string(DDLM.retriveXMLTag("Atlas_Thing","Vendor_OnlineDocumentation","URI").c_str(), Data_Object);
    AtlasThingAttachment_VendorOnlineDoc->writeFunc(0,0, Data_Object, AtlasThingAttachment_VendorOnlineDoc);
	
	display_Atlas_Attachment_VendorOnlineDoc_object(AtlasThingAttachment_VendorOnlineDoc);
    lwm2m_data_free(1, Data_Object);

}
*/
/*
void Objects_Engine::createAtlasAttachment_DataRepository_Object(){

	Data_Object = lwm2m_data_new(1);
	//IoTDDL_Parser DDLM;
	AtlasThingAttachment_DataRepository = get_Atlas_Attachment_DataRepository_object();
    if (NULL == AtlasThingAttachment_DataRepository){
		cout << "Failed to create AtlasAttachment_dataReposity object"<< endl;
    }else{ cout<<"AtlasAttachment_dataReposity"<<endl; }
	
	Data_Object->type = LWM2M_TYPE_STRING;
    Data_Object->id = 0;
    lwm2m_data_encode_string(DDLM.retriveXMLTag("Atlas_Thing","Data_repository","Vendor").c_str(), Data_Object);
	AtlasThingAttachment_DataRepository->writeFunc(0,0, Data_Object, AtlasThingAttachment_DataRepository);
	Data_Object->id = 1;
    lwm2m_data_encode_string(DDLM.retriveXMLTag("Atlas_Thing","Data_repository","URI").c_str(), Data_Object);
    AtlasThingAttachment_DataRepository->writeFunc(0,0, Data_Object, AtlasThingAttachment_DataRepository);
    Data_Object->id = 2;
    lwm2m_data_encode_string(DDLM.retriveXMLTag("Atlas_Thing","Data_repository","LoginName").c_str(), Data_Object);
	AtlasThingAttachment_DataRepository->writeFunc(0,0, Data_Object, AtlasThingAttachment_DataRepository);
	Data_Object->id = 3;
    lwm2m_data_encode_string(DDLM.retriveXMLTag("Atlas_Thing","Data_repository","LoginPasscode").c_str(), Data_Object);
    AtlasThingAttachment_DataRepository->writeFunc(0,0, Data_Object, AtlasThingAttachment_DataRepository);
    Data_Object->id = 4;
    lwm2m_data_encode_string(DDLM.retriveXMLTag("Atlas_Thing","Data_repository","Key").c_str(), Data_Object);
	AtlasThingAttachment_DataRepository->writeFunc(0,0, Data_Object, AtlasThingAttachment_DataRepository);
	Data_Object->id = 5;
    lwm2m_data_encode_string(DDLM.retriveXMLTag("Atlas_Thing","Data_repository","MessageFormat").c_str(), Data_Object);
    AtlasThingAttachment_DataRepository->writeFunc(0,0, Data_Object, AtlasThingAttachment_DataRepository);
	
	display_Atlas_Attachment_DataRepository_object(AtlasThingAttachment_DataRepository);
    lwm2m_data_free(1, Data_Object);

}
*/
/*
void Objects_Engine::createAtlasEntity_ResouceService_Object(){

	Data_Object = lwm2m_data_new(1);
	//IoTDDL_Parser DDLM;
	AtlasEntity_ResouceService = get_atlas_entity_resourceservice_object();
    if (NULL == AtlasEntity_ResouceService){
		cout << "Failed to create AtlasEntity_ResouceService object"<< endl;
    }else{ cout<<"AtlasEntity_ResouceService"<<endl; }
	
	Data_Object->type = LWM2M_TYPE_STRING;
    Data_Object->id = 0;
	lwm2m_data_encode_string(DDLM.retriveXMLTag("Atlas_Entity","Resource_Service","Res1_Describtion").c_str(), Data_Object);
	AtlasEntity_ResouceService->writeFunc(0,0, Data_Object, AtlasEntity_ResouceService);
	lwm2m_data_encode_string(DDLM.retriveXMLTag("Atlas_Entity","Resource_Service","Res2_Describtion").c_str(), Data_Object);
	AtlasEntity_ResouceService->writeFunc(1,0, Data_Object, AtlasEntity_ResouceService);
	
    Data_Object->id = 1;
	lwm2m_data_encode_string(DDLM.retriveXMLTag("Atlas_Entity","Resource_Service","Res1_InputTypes").c_str(), Data_Object);
	AtlasEntity_ResouceService->writeFunc(0,0, Data_Object, AtlasEntity_ResouceService);
	lwm2m_data_encode_string(DDLM.retriveXMLTag("Atlas_Entity","Resource_Service","Res2_InputTypes").c_str(), Data_Object);
	AtlasEntity_ResouceService->writeFunc(1,0, Data_Object, AtlasEntity_ResouceService);
	
    Data_Object->id = 2;
	lwm2m_data_encode_string(DDLM.retriveXMLTag("Atlas_Entity","Resource_Service","Res1_InputRange").c_str(), Data_Object);
	AtlasEntity_ResouceService->writeFunc(0,0, Data_Object, AtlasEntity_ResouceService);
	lwm2m_data_encode_string(DDLM.retriveXMLTag("Atlas_Entity","Resource_Service","Res2_InputRange").c_str(), Data_Object);
	AtlasEntity_ResouceService->writeFunc(1,0, Data_Object, AtlasEntity_ResouceService);
	
    Data_Object->id = 3;
	lwm2m_data_encode_string(DDLM.retriveXMLTag("Atlas_Entity","Resource_Service","Res1_OutputType").c_str(), Data_Object);
	AtlasEntity_ResouceService->writeFunc(0,0, Data_Object, AtlasEntity_ResouceService);
	lwm2m_data_encode_string(DDLM.retriveXMLTag("Atlas_Entity","Resource_Service","Res2_OutputType").c_str(), Data_Object);
	AtlasEntity_ResouceService->writeFunc(1,0, Data_Object, AtlasEntity_ResouceService);
	
    Data_Object->id = 4;
	lwm2m_data_encode_string("generated API 1", Data_Object);
	AtlasEntity_ResouceService->writeFunc(0,0, Data_Object, AtlasEntity_ResouceService);
	lwm2m_data_encode_string("generated API 2", Data_Object);
	AtlasEntity_ResouceService->writeFunc(1,0, Data_Object, AtlasEntity_ResouceService);
	
	display_atlas_entity_resourceservice_object(AtlasEntity_ResouceService);
    lwm2m_data_free(1, Data_Object);

}
	*/


/*
char* Objects_Engine::read_AtlasEntity_ResouceService_Attribute(int instancenumber, char* attribute){
if(strcmp(attribute, "Describtion") == 0){
				lwm2m_data_t* dataArray = lwm2m_data_new(5);
				int num = 0;
				AtlasEntity_ResouceService->readFunc(instancenumber, &num, &dataArray, AtlasEntity_ResouceService);
				char* ch = (char *)lwm2m_malloc(dataArray[0].value.asBuffer.length + 1);
				strcpy(ch, (char*)dataArray[0].value.asBuffer.buffer);
				lwm2m_data_free(5, dataArray);
				return ch;
				
}else if(strcmp(attribute, "InputTypes") == 0){
				lwm2m_data_t* dataArray = lwm2m_data_new(5);
				int num = 0;
				AtlasEntity_ResouceService->readFunc(instancenumber, &num, &dataArray, AtlasEntity_ResouceService);
				char* ch = (char *)lwm2m_malloc(dataArray[1].value.asBuffer.length + 1);
				strcpy(ch, (char*)dataArray[1].value.asBuffer.buffer);
				lwm2m_data_free(5, dataArray);
				return ch;
				
}else if(strcmp(attribute, "OutputTypes") == 0){
				lwm2m_data_t* dataArray = lwm2m_data_new(5);
				int num = 0;
				AtlasEntity_ResouceService->readFunc(instancenumber, &num, &dataArray, AtlasEntity_ResouceService);
				char* ch = (char *)lwm2m_malloc(dataArray[3].value.asBuffer.length + 1);
				strcpy(ch, (char*)dataArray[3].value.asBuffer.buffer);
				lwm2m_data_free(5, dataArray);
				return ch;
}else{
				cout<<"nothing"<<endl;
				return (char *)"";
}
}

*/

void Objects_Engine::update_AtlasEntity_API(int instancenumber, char* attribute){
/*
	lwm2m_data_t* Data_Object = lwm2m_data_new(1);
	Data_Object->type = LWM2M_TYPE_STRING;
    Data_Object->id = 4;
	lwm2m_data_encode_string("generated API 8", Data_Object);
	AtlasEntity_ResouceService->writeFunc(instancenumber,0, Data_Object, AtlasEntity_ResouceService);
    lwm2m_data_free(1, Data_Object);
*/
}



/*

int main(int argc, char *argv[]) {

	cout << "Parsing Atlas IoTDDL and Initializing Atlas objects own"<< endl;
	AtlasThing_DesMetadata = get_object_Atlas_Thing_Descriptive_Metadata();
    if (NULL == AtlasThing_DesMetadata){
		cout << "Failed to create AtlasThing_DesMdata object"<< endl;   return -1;
    }else{ cout<<"AtlasThing_DescMetadata"<<endl; }
	
	AtlasEntity_DesMetadata = get_object_Atlas_Entity_Descriptive_Metadata();
    if (NULL == AtlasEntity_DesMetadata){
		cout << "Failed to create AtlasEntity_DesMdatadata"<< endl;   return -1;
    }else{ cout<<"AtlasAttachment_vendorOnline"<<endl; }
	
	AtlasThingAttachment_VendorOnlineDoc = get_Atlas_Attachment_VendorOnlineDoc_object();
    if (NULL == AtlasThingAttachment_VendorOnlineDoc){
		cout << "Failed to create AtlasAttachment_vendorOnline object"<< endl;   return -1;
    }else{ cout<<"AtlasAttachment_vendorOnline"<<endl; }
	
	AtlasThingAttachment_DataRepository = get_Atlas_Attachment_DataRepository_object();
    if (NULL == AtlasThingAttachment_DataRepository){
		cout << "Failed to create AtlasAttachment_dataReposity object"<< endl;   return -1;
    }else{ cout<<"AtlasAttachment_dataReposity"<<endl; }
	
	AtlasEntity_ResouceService = get_atlas_entity_resourceservice_object();
    if (NULL == AtlasEntity_ResouceService){
		cout << "Failed to create AtlasEntity_ResouceService object"<< endl;   return -1;
    }else{ cout<<"AtlasEntity_ResouceService"<<endl; }
	
	
	createAtlasThing_DesMetadata_Object();
	createAtlasEntity_DesMetadata_Object();
	createAtlasAttachment_VendorOnlineDoc_Object();
	createAtlasAttachment_DataRepository_Object();
	createAtlasEntity_ResouceService_Object();
	
	
	
	
	
	
	
	
    
	/*
	
	string my_string2 = DDLM.retriveXMLTag("Atlas_Thing","Descriptive_Metadata","Thing_Vendor");
	string my_string = DDLM.retriveXMLTag("Atlas_Entity","Descriptive_Metadata","Entity_Vendor");
	
	
	
	
    Data_Object_t* datarep = Data_Object_new(6);
	AtlasThingAttachment_DataRepository = get_Atlas_Attachment_DataRepository_object();
    if (NULL == AtlasThingAttachment_DataRepository){
		cout << "Failed to create AtlasEntity_DesMdata object"<< endl;   return -1;
    }
	datarep->type = LWM2M_TYPE_STRING;
    datarep->id = 0;
    Data_Object_encode_string(DDLM.retriveXMLTag("Thing_Attachment","Data_repository","Vendor").c_str(), datarep);
    AtlasThingAttachment_DataRepository->writeFunc(0,0, datarep,AtlasThingAttachment_DataRepository);
    datarep->id = 1;
    Data_Object_encode_string(DDLM.retriveXMLTag("Thing_Attachment","Data_repository","URI").c_str(), datarep);
    AtlasThingAttachment_DataRepository->writeFunc(0,0, datarep,AtlasThingAttachment_DataRepository);
    datarep->id = 2;
    Data_Object_encode_string(DDLM.retriveXMLTag("Thing_Attachment","Data_repository","LoginName").c_str(), datarep);
    AtlasThingAttachment_DataRepository->writeFunc(0,0, datarep,AtlasThingAttachment_DataRepository);
    datarep->id = 3;
    Data_Object_encode_string(DDLM.retriveXMLTag("Thing_Attachment","Data_repository","LoginPasscode").c_str(), datarep);
    AtlasThingAttachment_DataRepository->writeFunc(0,0, datarep,AtlasThingAttachment_DataRepository);
    datarep->id = 4;
    Data_Object_encode_string(DDLM.retriveXMLTag("Thing_Attachment","Data_repository","Key").c_str(), datarep);
    AtlasThingAttachment_DataRepository->writeFunc(0,0, datarep,AtlasThingAttachment_DataRepository);
    datarep->id = 5;
    Data_Object_encode_string(DDLM.retriveXMLTag("Thing_Attachment","Data_repository","MessageFormat").c_str(), datarep);
    AtlasThingAttachment_DataRepository->writeFunc(0,0, datarep,AtlasThingAttachment_DataRepository);
	
	
    Data_Object_t* Data_Object = Data_Object_new(1);
	AtlasThingAttachment_VendorOnlineDoc = get_Atlas_Attachment_VendorOnlineDoc_object();
    if (NULL == AtlasThingAttachment_VendorOnlineDoc){
		cout << "Failed to create AtlasEntity_DesMdata object"<< endl;   return -1;
    }
	Data_Object->type = LWM2M_TYPE_STRING;
    Data_Object->id = 0;
    //Data_Object_encode_string(DDLM.retriveXMLTag("Thing_Attachment","Vendor_OnlineDocumentation","Vendor").c_str(), Data_Object);
    Data_Object_encode_string("", Data_Object);
    
	AtlasThingAttachment_VendorOnlineDoc->writeFunc(0,0, Data_Object,AtlasThingAttachment_VendorOnlineDoc);
    Data_Object->id = 1;
    //Data_Object_encode_string(DDLM.retriveXMLTag("Thing_Attachment","Vendor_OnlineDocumentation","URI").c_str(), Data_Object);
    Data_Object_encode_string("link.com", Data_Object);
    AtlasThingAttachment_VendorOnlineDoc->writeFunc(0,0, Data_Object,AtlasThingAttachment_VendorOnlineDoc);
    
	
	
	
	
    display_Atlas_Thing_Descriptive_Metadata_object(AtlasThing_DesMetadata);
    display_Atlas_Entity_Descriptive_Metadata_object(AtlasEntity_DesMetadata);
	display_Atlas_Attachment_VendorOnlineDoc_object(AtlasThingAttachment_VendorOnlineDoc);
	display_Atlas_Attachment_DataRepository_object(AtlasThingAttachment_DataRepository);
	
    free_object_Atlas_Thing_Descriptive_Metadata(AtlasThing_DesMetadata);
    free_object_Atlas_Entity_Descriptive_Metadata(AtlasEntity_DesMetadata);
	free_Atlas_Attachment_VendorOnlineDoc_object(AtlasThingAttachment_VendorOnlineDoc);
	free_Atlas_Attachment_DataRepository_object(AtlasThingAttachment_DataRepository);
	
	
	*/
	
	
	
	
	
	
	
	
	

	/*
    datax->id = 2;
    datax->type = LWM2M_TYPE_FLOAT;
    Data_Object_encode_float(3.5, datax);
    readres = obj1->writeFunc(0,0, datax,obj1);
    if (readres != COAP_205_CONTENT){
        printf("Error \r\n");
    }else{
        printf("Write is Ok");
    }
    display_Atlas_Thing_Descriptive_Metadata_object(obj1);

    Data_Object_t* dataArray = Data_Object_new(5);
    int numData = 0;
    readres = obj1->readFunc( 0, &numData, &dataArray, obj1);
    if (readres != COAP_205_CONTENT){
        printf("Error \r\n");
    }else{
        printf("Read is Ok");
        printf("reading result : %d\r\n", (dataArray)[0].id);
        printf("reading result : %s\r\n", (dataArray)[0].value.asBuffer.buffer);
        printf("reading result : %d\r\n", (dataArray)[1].id);
        printf("reading result : %s\r\n", (dataArray)[1].value.asBuffer.buffer);
        printf("reading result : %d\r\n", (dataArray)[2].id);
        printf("reading result : %f\r\n", (dataArray)[2].value.asFloat);
    }
	
	*/
	
	
	
	
	
	
	
	
	
	
	
	
	
	/*
	//calling identity parser module
    Identity_Parser IDP;
	IDP.parse_ThingInformation();
	IDP.parse_DevicesInformation();

	//calling API generator module
    API_Generator APIG;
	APIG.parse_DeviceInterfaces();
	APIG.parse_DeviceAPI();

	//calling Interaction and Relationship module
	Interaction_RShip_Manager IRSHP;
	IRSHP.parse_PermanentKnowledge();
	IRSHP.parse_TemporaryKnowledge();

	//calling knowledge and tweeting manager
	Knowledge_Tweeting_Manager KTM(IDP,APIG,IRSHP);
	KTM.generate_IdentityTweets();
	KTM.generate_FunctionalTweets();
	KTM.generate_KnowledgeTweets();
	KTM.update_Knowledge(KTM.IdentityTweets[0]);
	KTM.update_Knowledge(KTM.KnowledgeTweets[2]);
	KTM.display();
	
	
	Communication_Manager CM;
	CM.addIdentityTweets(KTM.IdentityTweets);
	CM.addFunctionalTweets(KTM.FunctionalTweets);
	CM.addKnowledgeTweets(KTM.KnowledgeTweets);
	
	std::thread t1(&Communication_Manager::serverSide,&CM);
	std::thread t2(&Communication_Manager::clientSide,&CM);
	t1.join();
	t2.join();
	*/

	//g++ -o atlas DDL_Manager.cpp Identity_Parser.cpp API_Generator.cpp Knowledge_Tweeting_Manager.cpp Interaction_RShip_Manager.cpp Communication_Manager.cpp main.cpp -pthread -std=c++0x
	//std::thread t1(&Communication_Manager::serverSide,Communication_Manager());
	//std::thread t2(&Communication_Manager::clientSide,Communication_Manager());
	//cout << "Thing Name>> " << IDP.Thing_Name << endl;
	//cout << "Thing ID>> " << IDP.D_id[0].deviceName <<endl;
    //DDL_Manager DDLM;
    //DDLM.addXMLTag("Thing","Nxyz","hello22");
	
	
	//
	/*
	g++ -o wakaama AtlasArchitecture/DDL_Manager.cpp AtlasArchitecture/Identity_Parser.cpp AtlasArchitecture/API_Generator.cpp AtlasArchitecture/Knowledge_Tweeting_Manager.cpp AtlasArchitecture/Interaction_RShip_Manager.cpp AtlasArchitecture/Communication_Manager.cpp OMAlib/bootstrap.c OMAlib/data.c OMAlib/discover.c OMAlib/er-coap-13.c OMAlib/json.c OMAlib/list.c OMAlib/liblwm2m.c lwm2mclient.c OMAlib/management.c OMAlib/memtrace.c OMAlib/object_access_control.c OMAlib/AtlasAttachment_DataRepository.c OMAlib/AtlasAttachment_VendorOnlineDoc.c OMAlib/AtlasEntity_DescriptiveMetadata.c OMAlib/AtlasEntity_Relationship.c OMAlib/AtlasEntity_ResourceService.c OMAlib/AtlasThing_Attachment.c OMAlib/AtlasThing_DescriptiveMetadata.c OMAlib/object_connectivity_moni.c OMAlib/object_connectivity_stat.c OMAlib/object_device.c OMAlib/object_firmware.c OMAlib/object_location.c OMAlib/object_security.c OMAlib/object_server.c OMAlib/objects.c OMAlib/observe.c OMAlib/packet.c OMAlib/platform.c OMAlib/registration.c OMAlib/system_api.c OMAlib/test_object.c OMAlib/uri.c OMAlib/utils.c OMAlib/transaction.c -pthread -std=c++0x
	
	*/
	/*
	time_t now = time(0);
	tm *ltm = localtime(&now);
	cout<<ltm->tm_hour<<endl;
	cout<<ltm->tm_min<<endl;
	cout<<ltm->tm_sec<<endl;
	*/	
	/*
	for (std::vector<string>::iterator it = KTM.IdentityTweets.begin() ; it != KTM.IdentityTweets.end(); ++it)
       std::cout << ' ' << *it<<endl;
	for (std::vector<string>::iterator it = KTM.FunctionalTweets.begin() ; it != KTM.FunctionalTweets.end(); ++it)
       std::cout << ' ' << *it<<endl;
	for (std::vector<string>::iterator it = KTM.KnowledgeTweets.begin() ; it != KTM.KnowledgeTweets.end(); ++it)
       std::cout << ' ' << *it<<endl;
	*/


	/*
	
    fprintf(stdout,"** am here \r\n");
    fprintf(stdout,"** Starting client Side\r\n");


    fprintf(stdout,"** Starting Atlas D Metadata object\r\n");
    lwm2m_object_t * obj1;
    obj1 = get_object_Atlas_Thing_Descriptive_Metadata();
    if (NULL == obj1){
        fprintf(stderr, "Failed to create Atlas D Mdata object\r\n");
        return -1;
    }
    display_Atlas_Thing_Descriptive_Metadata_object(obj1);

    Data_Object_t* datax = Data_Object_new(1);
    datax->id = 0;
    datax->type = LWM2M_TYPE_STRING;
    Data_Object_encode_string("khaled", datax);
    uint16_t readres = obj1->writeFunc(0,0, datax,obj1);
    if (readres != COAP_205_CONTENT){
        printf("Error \r\n");
    }
    display_Atlas_Thing_Descriptive_Metadata_object(obj1);

    datax->id = 2;
    datax->type = LWM2M_TYPE_FLOAT;
    Data_Object_encode_float(3.5, datax);
    readres = obj1->writeFunc(0,0, datax,obj1);
    if (readres != COAP_205_CONTENT){
        printf("Error \r\n");
    }else{
        printf("Write is Ok");
    }
    display_Atlas_Thing_Descriptive_Metadata_object(obj1);

    Data_Object_t* dataArray = Data_Object_new(5);
    int numData = 0;
    readres = obj1->readFunc( 0, &numData, &dataArray, obj1);
    if (readres != COAP_205_CONTENT){
        printf("Error \r\n");
    }else{
        printf("Read is Ok");
        printf("reading result : %d\r\n", (dataArray)[0].id);
        printf("reading result : %s\r\n", (dataArray)[0].value.asBuffer.buffer);
        printf("reading result : %d\r\n", (dataArray)[1].id);
        printf("reading result : %s\r\n", (dataArray)[1].value.asBuffer.buffer);
        printf("reading result : %d\r\n", (dataArray)[2].id);
        printf("reading result : %f\r\n", (dataArray)[2].value.asFloat);
    }


    printf("=======================================================\r\n");
    printf("=======================================================\r\n");


    fprintf(stdout,"** Starting AtlasAttachment VendorDoc object\r\n");
    lwm2m_object_t * objx;
    objx = get_Atlas_Attachment_VendorOnlineDoc_object();
    if (NULL == objx){
        fprintf(stderr, "Failed to create AtlasAttachment VendorDoc object\r\n");
        return -1;
    }
    display_Atlas_Attachment_VendorOnlineDoc_object(objx);

    Data_Object_t* dataz = Data_Object_new(1);
    dataz->id = 0;
    dataz->type = LWM2M_TYPE_STRING;
    Data_Object_encode_string("khaled", dataz);
    objx->writeFunc(0,0, dataz,objx);
    display_Atlas_Attachment_VendorOnlineDoc_object(objx);

    dataz->id = 1;
    dataz->type = LWM2M_TYPE_STRING;
    Data_Object_encode_string("www.newVendorDoc.com", dataz);
    objx->writeFunc(0,0, dataz,objx);
    display_Atlas_Attachment_VendorOnlineDoc_object(objx);

    Data_Object_t* dataArrayy = Data_Object_new(5);
    numData = 0;
    objx->readFunc( 0, &numData, &dataArrayy, objx);
    printf("reading result : %d\r\n", (dataArrayy)[0].id);
    printf("reading result : %s\r\n", (dataArrayy)[0].value.asBuffer.buffer);
    printf("reading result : %d\r\n", (dataArrayy)[1].id);
    printf("reading result : %s\r\n", (dataArrayy)[1].value.asBuffer.buffer);


    printf("=======================================================\r\n");
    printf("=======================================================\r\n");

    fprintf(stdout,"** Initiate entity relationship object\r\n");
    lwm2m_object_t * obj2;
    obj2 = get_atlas_entity_relationship_object();
    if (NULL == obj2){
        fprintf(stderr, "Failed to create test object\r\n");
        return -1;
    }
    fprintf(stdout,"** entity relationship object successful \r\n");

    fprintf(stdout,"xyz \r\n");
    display_atlas_entity_relationship_object(obj2);
    Data_Object_t* dataxy = Data_Object_new(1);
    dataxy->id = 0;
    dataxy->type = LWM2M_TYPE_STRING;
    Data_Object_encode_string("uses", dataxy);
    uint16_t readresx = obj2->writeFunc(11,0, dataxy,obj2);
    if (readresx != COAP_205_CONTENT){
        printf("Error \r\n");
    }
    display_atlas_entity_relationship_object(obj2);
    Data_Object_t* dataxyz = Data_Object_new(1);
    dataxyz->id = 0;
    dataxyz->type = LWM2M_TYPE_STRING;
    Data_Object_encode_string("zzzzz", dataxyz);
    readresx = obj2->createFunc(14,0, dataxyz,obj2);
     if (readresx != COAP_205_CONTENT){
        printf("Error \r\n");
    }
    display_atlas_entity_relationship_object(obj2);
   

    printf("=======================================================");
    printf("=======================================================");


    lwm2m_object_t * obj3;
    fprintf(stdout,"** Initiate device object\r\n");
    obj3 = get_object_device();
    if (NULL == obj3){
        fprintf(stderr, "Failed to create Device object\r\n");
        return -1;
    }

    Data_Object_t * dataArrayx = Data_Object_new(10);
    int numDataxx = 0;
    uint16_t readresxx = obj3->readFunc( 0, &numDataxx, &dataArrayx, obj3);
    if (readresxx != COAP_205_CONTENT){
        printf("Error \r\n");
    }else{
        printf("Read is Ok");
        int i=0;
        for (i = 0 ; i < 7 ; i++){
        printf("reading result at : %i\r\n", i);
        printf("reading result : %d\r\n", (dataArrayx)[i].id);
        }
    }

    display_device_object(obj3);

    Data_Object_t* dataxv = Data_Object_new(1);
    dataxv->id = 15;
    dataxv->type = LWM2M_TYPE_STRING;
    Data_Object_encode_string("hello", dataxv);

    readres = obj3->writeFunc(0,0, dataxv,obj3);
    if (readres != COAP_205_CONTENT){
        printf("Error \r\n");
    }
    printf("=======================================================\r\n");
    printf("=======================================================\r\n");

    printf("===============freee eveerrrryy thinnggg===============\r\n");

    lwm2m_free(obj3);
    free_object_Atlas_Thing_Descriptive_Metadata(obj1);
    free_atlas_entity_relationship_object(obj2);
	*/
  //  printf("=======================================================\r\n");


 //   return 0;
//}

//*/

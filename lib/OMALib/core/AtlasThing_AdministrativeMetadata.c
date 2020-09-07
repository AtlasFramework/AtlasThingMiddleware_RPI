/*******************************************************************************
 *  Developer: Ahmed E. Khaled 
 *  Date: June, 2020
 *  This object is single instance only, 
 *  Mandatory for Atlas thing architecture 
 *
 *  =====================================================================
 *          Object                      |  ID   | M-Instances | Mandatoty |
 *  AtlasThing_Administrative_metadata  | 8001  |    No       |    Yes    |
 *  =====================================================================
 *  Ressources            Supported    Multiple
 *  Name           | ID | Operations | Instances | Mandatory |  Type   | Range | Units |               Description                  |
 *                 |    |            |           |           |         |       |       |                                            |
 *  
 * Attributes regarding the Network Manager
 *  Type           |  0 |    R/W     |    No     |    Yes    | String  |       |       |                                            |
 *  Module         |  1 |    R/W     |    No     |    Yes    | String  |       |       |   			 	    	    |
 *  Protocol       |  2 |    R/W     |    No     |    Yes    | String  |       |       |                                  	    |
 *  NetworkName    |  3 |    R/W     |    No     |    Yes    | String  |       |       |               				    |
 *  type           |  4 |    R/W     |    No     |    Yes    | String  |       |       |                                            |
 *     ...................................................................................................................
 *     ...................................................................................................................
 *  OMAIP          |  9 |    R/W     |    No     |    Yes    | String  |       |       |                                            |
 *  OMAPort        | 10 |    R/W     |    No     |    Yes    | String  |       |       |                                            |
 *
 *
 * Attributes regarding the Memory Manager
 *  StoreFormate   | 11 |    R/W     |    No     |    Yes    | String   |       |       |                                           |
 *  MemApplication | 12 |    R/W     |    No     |    Yes    | String   |       |       |                                           | 
 *  MemKnowledge   | 13 |    R/W     |    No     |    Yes    | String   |       |       |                                           | 
 *  =====================================================================
 */


#include "liblwm2m.h"
#include "objectHandler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>


// Object and Resource Id
#define ATLAS_THING_ADMINISTRATIVE_METAdATA_OBJECT_ID    8001
#define RES_NM_Type               0
#define RES_NM_Module             1
#define RES_NM_CommProtocol       2
#define RES_NM_NetName            3
#define RES_NM_NetPasscode        4
#define RES_NM_BrokerVendor       5
#define RES_NM_BrokerURL          6
#define RES_NM_BrokerDashboard    7
#define RES_NM_OMADM_Name         8
#define RES_NM_OMADM_IP           9
#define RES_NM_OMADM_Port         10


typedef struct{
    char* Type;
    char* Module;
    char* CommProtocol;
    char* NetName;
    char* NetPasscode;
    char* BrokerVendor;
    char* BrokerURL;
    char* BrokerDashboard;
    char* OMADM_Name;
    char* OMADM_IP;
    char* OMADM_Port;
} device_data_t;


char *DDL_FilePathh = "ConfigurationFiles/Atlas_IoTDDL.xml";

//Retrive data of an XML tag of forth level
void removeSubstrX (char *string, char *sub) {
    char *match;
    int len = strlen(sub);
    while ((match = strstr(string, sub))) {
        *match = '\0';
        strcat(string, match+len);
    }
}

char* retriveXMLTagX(char* mainTag, char* mainTag2, char* mainTag3, char* mainTag4, char* tag){
	char * S1 = (char *) malloc(strlen(tag)+ 2);   		//<tag>
	strcpy(S1, "<");      	strcat(S1, tag);      	strcat(S1, ">");
	char * S2 = (char *) malloc(strlen(tag)+ 3);   		//</tag>
	strcpy(S2, "</");      	strcat(S2, tag);      	strcat(S2, ">");

      	char * S3 = (char *) malloc(strlen(mainTag)+ 2);   		//<mainTag>
	strcpy(S3, "<");      	strcat(S3, tag);      	strcat(S3, ">");
      	char * S4 = (char *) malloc(strlen(mainTag2)+ 2);   	//<mainTag2>
	strcpy(S4, "<");      	strcat(S4, tag);      	strcat(S4, ">");
      	char * S5 = (char *) malloc(strlen(mainTag3)+ 2);   	//<mainTag3>
	strcpy(S5, "<");      	strcat(S5, tag);      	strcat(S5, ">");
      	char * S6 = (char *) malloc(strlen(mainTag4)+ 2);   	//<mainTag4>
	strcpy(S6, "<");      	strcat(S6, tag);      	strcat(S6, ">");


	bool mainTagB = false;
	bool mainTag2B = false;
	bool mainTag3B = false;
	bool mainTag4B = false;
	size_t found=0;
	size_t found1=0;
    	size_t found2=0;
    	size_t found3=0;
	FILE* file = fopen(DDL_FilePathh, "r"); 
    	char line[500];

    	while (fgets(line, sizeof(line), file)) {
		if(strstr(line, S3))		//found tag in the mainTag seciton
			mainTagB = true;
		if(mainTagB){
			if(strstr(line, S4))		//found tag in the mainTag2 seciton
				mainTag2B = true;
		}
		if(mainTagB&&mainTag2B){
			if(strstr(line, S5))		//found tag in the mainTag2 seciton
				mainTag3B = true;
		}
		if(mainTagB&&mainTag2B&&mainTag3B){
			if(strstr(line, S6))		//found tag in the mainTag2 seciton
				mainTag4B = true;
		}

		if(mainTagB&&mainTag2B&&mainTag3B&&mainTag4B){
			if(strstr(line, S1)){		//found tag in the mainTag2 seciton
				removeSubstrX(line, S1);
				removeSubstrX(line, S2);
     				char* tagReturn = (char*)malloc(strlen(line)+1);
     				strcpy(tagReturn,line);
				return tagReturn;
			}
		}
	}
    	fclose(file);
	return "";
}


static uint8_t prv_set_value(lwm2m_data_t * dataP, device_data_t * devDataP){

    switch (dataP->id){
	    case RES_NM_Type:
	        lwm2m_data_encode_string(devDataP->Type, dataP);
    		printf("Reading value of Type... \n");
	        return COAP_205_CONTENT;
	    case RES_NM_Module:
	        lwm2m_data_encode_string(devDataP->Module, dataP);
    		printf("Reading value of Module... \n");
	        return COAP_205_CONTENT;
	    case RES_NM_CommProtocol:
	        lwm2m_data_encode_string(devDataP->CommProtocol, dataP);
    		printf("Reading value of Communication protocol... \n");
	        return COAP_205_CONTENT;
	    case RES_NM_NetName:
	        lwm2m_data_encode_string(devDataP->NetName, dataP);
    		printf("Reading value of network name... \n");
	        return COAP_205_CONTENT;
	    case RES_NM_NetPasscode:
	        lwm2m_data_encode_string(devDataP->NetPasscode, dataP);
    		printf("Reading value of network passcode... \n");
	        return COAP_205_CONTENT;
	    case RES_NM_BrokerVendor:
	        lwm2m_data_encode_string(devDataP->BrokerVendor, dataP);
    		printf("Reading value of broker vendor... \n");
	        return COAP_205_CONTENT;
	    case RES_NM_BrokerURL:
	        lwm2m_data_encode_string(devDataP->BrokerURL, dataP);
    		printf("Reading value of broker url... \n");
	        return COAP_205_CONTENT;
	    case RES_NM_BrokerDashboard:
	        lwm2m_data_encode_string(devDataP->BrokerDashboard, dataP);
    		printf("Reading value of broker dashboard... \n");
	        return COAP_205_CONTENT;
	    case RES_NM_OMADM_Name:
	        lwm2m_data_encode_string(devDataP->OMADM_Name, dataP);
    		printf("Reading value of OMA DM name... \n");
	        return COAP_205_CONTENT;
	    case RES_NM_OMADM_IP:
	        lwm2m_data_encode_string(devDataP->OMADM_IP, dataP);
    		printf("Reading value of OMA DM IP... \n");
	        return COAP_205_CONTENT;
	    case RES_NM_OMADM_Port:
	        lwm2m_data_encode_string(devDataP->OMADM_Port, dataP);
    		printf("Reading value of OMA DM Port... \n");
	        return COAP_205_CONTENT;
	    default:
	        return COAP_404_NOT_FOUND;
    }
}

static uint8_t prv_device_read(uint16_t instanceId,int * numDataP, lwm2m_data_t ** dataArrayP, lwm2m_object_t * objectP){

    	if (instanceId != 0)    
		return COAP_404_NOT_FOUND;    // this is a single instance object

    	uint8_t result;
    	int i;
    	if (*numDataP == 0){        // is the server asking for the full object
        	uint16_t resList[] = { 
        		RES_NM_Type,
			RES_NM_Module,             
			RES_NM_CommProtocol,       
			RES_NM_NetName,            
			RES_NM_NetPasscode,        
			RES_NM_BrokerVendor,       
			RES_NM_BrokerURL,          
			RES_NM_BrokerDashboard,    
			RES_NM_OMADM_Name,         
			RES_NM_OMADM_IP,           
			RES_NM_OMADM_Port         

        	};
        	int nbRes = sizeof(resList)/sizeof(uint16_t);

        	*dataArrayP = lwm2m_data_new(nbRes);
        	if (*dataArrayP == NULL) return COAP_500_INTERNAL_SERVER_ERROR;
        	*numDataP = nbRes;
        	for (i = 0 ; i < nbRes ; i++)
            		(*dataArrayP)[i].id = resList[i];
    	}

    	i = 0;
    	do{
        	result = prv_set_value((*dataArrayP)+i, (device_data_t*)(objectP->userData));
        	i++;
    	} while (i < *numDataP && result == COAP_205_CONTENT);
    	return result;
}

static uint8_t prv_device_discover(uint16_t instanceId, int * numDataP, lwm2m_data_t ** dataArrayP, lwm2m_object_t * objectP){

    	if (instanceId != 0)    
		return COAP_404_NOT_FOUND;// this is a single instance object
    	
	uint8_t result;
    	int i;
    	result = COAP_205_CONTENT;

    	if (*numDataP == 0){   // is the server asking for the full object ?
        	uint16_t resList[] = { 
        		RES_NM_Type,
			RES_NM_Module,             
			RES_NM_CommProtocol,       
			RES_NM_NetName,            
			RES_NM_NetPasscode,        
			RES_NM_BrokerVendor,       
			RES_NM_BrokerURL,          
			RES_NM_BrokerDashboard,    
			RES_NM_OMADM_Name,         
			RES_NM_OMADM_IP,           
			RES_NM_OMADM_Port    
        	};
        	int nbRes = sizeof(resList) / sizeof(uint16_t);
        	*dataArrayP = lwm2m_data_new(nbRes);
        	if (*dataArrayP == NULL) return COAP_500_INTERNAL_SERVER_ERROR;
        	*numDataP = nbRes;
        	for (i = 0; i < nbRes; i++)
			(*dataArrayP)[i].id = resList[i];
    	
	}else{
        	for (i = 0; i < *numDataP && result == COAP_205_CONTENT; i++){
            		switch ((*dataArrayP)[i].id){
				case RES_NM_Type:  		return COAP_205_CONTENT;
				case RES_NM_Module:  		return COAP_205_CONTENT;             
				case RES_NM_CommProtocol:  	return COAP_205_CONTENT;       
				case RES_NM_NetName:  		return COAP_205_CONTENT;            
				case RES_NM_NetPasscode:  	return COAP_205_CONTENT;        
				case RES_NM_BrokerVendor:  	return COAP_205_CONTENT;       
				case RES_NM_BrokerURL:  	return COAP_205_CONTENT;          
				case RES_NM_BrokerDashboard:  	return COAP_205_CONTENT;
				case RES_NM_OMADM_Name:  	return COAP_205_CONTENT;         
				case RES_NM_OMADM_IP:  		return COAP_205_CONTENT;           
				case RES_NM_OMADM_Port:  	return COAP_205_CONTENT;  
	            		default: 		
						result = COAP_404_NOT_FOUND;
            		}
        	}
    	}
    	return result;
}





static uint8_t prv_device_write(uint16_t instanceId, int numData, lwm2m_data_t * dataArray, lwm2m_object_t * objectP) {


    	if (instanceId != 0)  
		return COAP_404_NOT_FOUND;	// this is a single instance object
        
    	int i=0;
    	uint8_t result;

    	do{
        	switch (dataArray[i].id){
	        	case RES_NM_Type:
				printf("Writing new Type\n");
	            		if(NULL != intRes){ 

					//clear the old value
					char* temp =  "\0";
            				((device_data_t*)objectP->userData)->Type = (char*)malloc(sizeof(char)* (sizeof(temp)));
            				strcpy(((device_data_t*)objectP->userData)->Type, temp);
					
					//set the new value
					strncpy(((device_data_t*)(objectP->userData))->Type, (char*)dataArray[i].value.asBuffer.buffer, dataArray[i].value.asBuffer.length);
	            			result = COAP_204_CHANGED;
                                	printf("The new Type: %s\n", ((device_data_t*)(objectP->userData))->Type);
    				}else{
					strncpy(((device_data_t*)(objectP->userData))->Type, intRes, strlen(intRes));
	            			result = COAP_204_CHANGED;
                                }
	            		break;


	        	case RES_NM_Module:
				printf("Writing new Module\n");
	            		if(NULL != intRes){ 

					//clear the old value
					char* temp =  "\0";
            				((device_data_t*)objectP->userData)->Module = (char*)malloc(sizeof(char)* (sizeof(temp)));
            				strcpy(((device_data_t*)objectP->userData)->Module, temp);
					
					//set the new value
					strncpy(((device_data_t*)(objectP->userData))->Module, (char*)dataArray[i].value.asBuffer.buffer, dataArray[i].value.asBuffer.length);
	            			result = COAP_204_CHANGED;
                                	printf("The new Type: %s\n", ((device_data_t*)(objectP->userData))->Module);
    				}else{
					strncpy(((device_data_t*)(objectP->userData))->Module, intRes, strlen(intRes));
	            			result = COAP_204_CHANGED;
                                }
	            		break;


	        	case RES_NM_CommProtocol:
				printf("Writing new Comm protocol\n");
	            		if(NULL != intRes){ 

					//clear the old value
					char* temp =  "\0";
            				((device_data_t*)objectP->userData)->CommProtocol = (char*)malloc(sizeof(char)* (sizeof(temp)));
            				strcpy(((device_data_t*)objectP->userData)->CommProtocol, temp);
					
					//set the new value
					strncpy(((device_data_t*)(objectP->userData))->CommProtocol, (char*)dataArray[i].value.asBuffer.buffer, dataArray[i].value.asBuffer.length);
	            			result = COAP_204_CHANGED;
                                	printf("The new comm protocol: %s\n", ((device_data_t*)(objectP->userData))->CommProtocol);
    				}else{
					strncpy(((device_data_t*)(objectP->userData))->CommProtocol, intRes, strlen(intRes));
	            			result = COAP_204_CHANGED;
                                }
	            		break;


	        	case RES_NM_NetName:
				printf("Writing new network name\n");
	            		if(NULL != intRes){ 

					//clear the old value
					char* temp =  "\0";
            				((device_data_t*)objectP->userData)->NetName = (char*)malloc(sizeof(char)* (sizeof(temp)));
            				strcpy(((device_data_t*)objectP->userData)->NetName, temp);
					
					//set the new value
					strncpy(((device_data_t*)(objectP->userData))->NetName, (char*)dataArray[i].value.asBuffer.buffer, dataArray[i].value.asBuffer.length);
	            			result = COAP_204_CHANGED;
                                	printf("The new network name: %s\n", ((device_data_t*)(objectP->userData))->NetName);
    				}else{
					strncpy(((device_data_t*)(objectP->userData))->NetName, intRes, strlen(intRes));
	            			result = COAP_204_CHANGED;
                                }
	            		break;


	        	case RES_NM_NetPasscode:
				printf("Writing new network passcode\n");
	            		if(NULL != intRes){ 

					//clear the old value
					char* temp =  "\0";
            				((device_data_t*)objectP->userData)->NetPasscode = (char*)malloc(sizeof(char)* (sizeof(temp)));
            				strcpy(((device_data_t*)objectP->userData)->NetPasscode, temp);
					
					//set the new value
					strncpy(((device_data_t*)(objectP->userData))->NetPasscode, (char*)dataArray[i].value.asBuffer.buffer, dataArray[i].value.asBuffer.length);
	            			result = COAP_204_CHANGED;
                                	printf("The new network passcode: %s\n", ((device_data_t*)(objectP->userData))->NetPasscode);
    				}else{
					strncpy(((device_data_t*)(objectP->userData))->NetPasscode, intRes, strlen(intRes));
	            			result = COAP_204_CHANGED;
                                }
	            		break;


	        	case RES_NM_BrokerVendor:
				printf("Writing new broker vendor\n");
	            		if(NULL != intRes){ 

					//clear the old value
					char* temp =  "\0";
            				((device_data_t*)objectP->userData)->BrokerVendor = (char*)malloc(sizeof(char)* (sizeof(temp)));
            				strcpy(((device_data_t*)objectP->userData)->BrokerVendor, temp);
					
					//set the new value
					strncpy(((device_data_t*)(objectP->userData))->BrokerVendor, (char*)dataArray[i].value.asBuffer.buffer, dataArray[i].value.asBuffer.length);
	            			result = COAP_204_CHANGED;
                                	printf("The new broker vendor: %s\n", ((device_data_t*)(objectP->userData))->BrokerVendor);
    				}else{
					strncpy(((device_data_t*)(objectP->userData))->BrokerVendor, intRes, strlen(intRes));
	            			result = COAP_204_CHANGED;
                                }
	            		break;


	        	case RES_NM_BrokerURL:
				printf("Writing new broker URL\n");
	            		if(NULL != intRes){ 

					//clear the old value
					char* temp =  "\0";
            				((device_data_t*)objectP->userData)->BrokerURL = (char*)malloc(sizeof(char)* (sizeof(temp)));
            				strcpy(((device_data_t*)objectP->userData)->BrokerURL, temp);
					
					//set the new value
					strncpy(((device_data_t*)(objectP->userData))->BrokerURL, (char*)dataArray[i].value.asBuffer.buffer, dataArray[i].value.asBuffer.length);
	            			result = COAP_204_CHANGED;
                                	printf("The new broker URL: %s\n", ((device_data_t*)(objectP->userData))->BrokerURL);
    				}else{
					strncpy(((device_data_t*)(objectP->userData))->BrokerURL, intRes, strlen(intRes));
	            			result = COAP_204_CHANGED;
                                }
	            		break;


	        	case RES_NM_BrokerDashboard:
				printf("Writing new broker dashboard\n");
	            		if(NULL != intRes){ 

					//clear the old value
					char* temp =  "\0";
            				((device_data_t*)objectP->userData)->BrokerDashboard = (char*)malloc(sizeof(char)* (sizeof(temp)));
            				strcpy(((device_data_t*)objectP->userData)->BrokerDashboard, temp);
					
					//set the new value
					strncpy(((device_data_t*)(objectP->userData))->BrokerDashboard, (char*)dataArray[i].value.asBuffer.buffer, dataArray[i].value.asBuffer.length);
	            			result = COAP_204_CHANGED;
                                	printf("The new broker dashboard: %s\n", ((device_data_t*)(objectP->userData))->BrokerDashboard);
    				}else{
					strncpy(((device_data_t*)(objectP->userData))->BrokerDashboard, intRes, strlen(intRes));
	            			result = COAP_204_CHANGED;
                                }
	            		break;



	        	case RES_NM_OMADM_Name:
				printf("Writing new OMADM Name\n");
	            		if(NULL != intRes){ 

					//clear the old value
					char* temp =  "\0";
            				((device_data_t*)objectP->userData)->OMADM_Name = (char*)malloc(sizeof(char)* (sizeof(temp)));
            				strcpy(((device_data_t*)objectP->userData)->OMADM_Name, temp);
					
					//set the new value
					strncpy(((device_data_t*)(objectP->userData))->OMADM_Name, (char*)dataArray[i].value.asBuffer.buffer, dataArray[i].value.asBuffer.length);
	            			result = COAP_204_CHANGED;
                                	printf("The new OMADM Name: %s\n", ((device_data_t*)(objectP->userData))->OMADM_Name);
    				}else{
					strncpy(((device_data_t*)(objectP->userData))->OMADM_Name, intRes, strlen(intRes));
	            			result = COAP_204_CHANGED;
                                }
	            		break;


	        	case RES_NM_OMADM_IP:
				printf("Writing new OMADM IP\n");
	            		if(NULL != intRes){ 

					//clear the old value
					char* temp =  "\0";
            				((device_data_t*)objectP->userData)->OMADM_IP = (char*)malloc(sizeof(char)* (sizeof(temp)));
            				strcpy(((device_data_t*)objectP->userData)->OMADM_IP, temp);
					
					//set the new value
					strncpy(((device_data_t*)(objectP->userData))->OMADM_IP, (char*)dataArray[i].value.asBuffer.buffer, dataArray[i].value.asBuffer.length);
	            			result = COAP_204_CHANGED;
                                	printf("The new OMADM IP: %s\n", ((device_data_t*)(objectP->userData))->OMADM_IP);
    				}else{
					strncpy(((device_data_t*)(objectP->userData))->OMADM_IP, intRes, strlen(intRes));
	            			result = COAP_204_CHANGED;
                                }
	            		break;


	        	case RES_NM_OMADM_Port:
				printf("Writing new OMADM Port\n");
	            		if(NULL != intRes){ 

					//clear the old value
					char* temp =  "\0";
            				((device_data_t*)objectP->userData)->OMADM_Port = (char*)malloc(sizeof(char)* (sizeof(temp)));
            				strcpy(((device_data_t*)objectP->userData)->OMADM_Port, temp);
					
					//set the new value
					strncpy(((device_data_t*)(objectP->userData))->OMADM_Port, (char*)dataArray[i].value.asBuffer.buffer, dataArray[i].value.asBuffer.length);
	            			result = COAP_204_CHANGED;
                                	printf("The new OMADM Port: %s\n", ((device_data_t*)(objectP->userData))->OMADM_Port);
    				}else{
					strncpy(((device_data_t*)(objectP->userData))->OMADM_Port, intRes, strlen(intRes));
	            			result = COAP_204_CHANGED;
                                }
	            		break;


		    	default:
		        	result = COAP_405_METHOD_NOT_ALLOWED;
        	} i++;
    	} while (i < numData && result == COAP_204_CHANGED);
    	
	return result;
}



static uint8_t prv_device_execute(uint16_t instanceId, uint16_t resourceId, uint8_t * buffer, int length, lwm2m_object_t * objectP){
    
        	printf("device execute ... \n");
    		if (instanceId != 0)    return COAP_404_NOT_FOUND;      // this is a single instance object
    		if (length != 0) 	return COAP_400_BAD_REQUEST;
    		switch (resourceId)	{
				case RES_NM_Type:  		return COAP_204_CHANGED;
				case RES_NM_Module:  		return COAP_204_CHANGED;             
				case RES_NM_CommProtocol:  	return COAP_204_CHANGED;       
				case RES_NM_NetName:  		return COAP_204_CHANGED;            
				case RES_NM_NetPasscode:  	return COAP_204_CHANGED;        
				case RES_NM_BrokerVendor:  	return COAP_204_CHANGED;       
				case RES_NM_BrokerURL:  	return COAP_204_CHANGED;          
				case RES_NM_BrokerDashboard:  	return COAP_204_CHANGED;
				case RES_NM_OMADM_Name:  	return COAP_204_CHANGED;         
				case RES_NM_OMADM_IP:  		return COAP_204_CHANGED;           
				case RES_NM_OMADM_Port:  	return COAP_204_CHANGED;  

	            		default: 			return COAP_405_METHOD_NOT_ALLOWED;
    		}
}

void display_Atlas_Thing_Administrative_Metadata_object(lwm2m_object_t * object) {
	
	device_data_t * data = (device_data_t *)object->userData;

    	fprintf(stdout, "  /%u:  Atlas Thing Administrative-Metadata:\r\n", object->objID);
    	
	if (NULL != data) {
        	printf("%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s \r\n",
			    data->Type,
			    data->Module,
			    data->CommProtocol,
			    data->NetName,
			    data->NetPasscode,
			    data->BrokerVendor,
			    data->BrokerURL,
			    data->BrokerDashboard,
			    data->OMADM_Name,
			    data->OMADM_IP,
			    data->OMADM_Port);
    	}
}

lwm2m_object_t * get_object_Atlas_Thing_Administrative_Metadata(){

    lwm2m_object_t * deviceObj;  //structure definition in liblwm2m.h
    deviceObj = (lwm2m_object_t *)lwm2m_malloc(sizeof(lwm2m_object_t));

    if (NULL != deviceObj) {
        memset(deviceObj, 0, sizeof(lwm2m_object_t));

        //setting Object ID and InstanceList
        deviceObj->objID = ATLAS_THING_ADMINISTRATIVE_METAdATA_OBJECT_ID;
        deviceObj->instanceList = (lwm2m_list_t *)lwm2m_malloc(sizeof(lwm2m_list_t));
        
	if (NULL != deviceObj->instanceList)
            memset(deviceObj->instanceList, 0, sizeof(lwm2m_list_t));
        else{
            lwm2m_free(deviceObj);
            return NULL;
        }

        //setting functions to read/write resources' values
        deviceObj->readFunc     = prv_device_read;
        deviceObj->discoverFunc = prv_device_discover;
        deviceObj->writeFunc    = prv_device_write;
        deviceObj->executeFunc  = prv_device_execute;



        //setting Object's User defined data
        deviceObj->userData = lwm2m_malloc(sizeof(device_data_t));
        if (NULL != deviceObj->userData){

		//Type Attribute
		char* typetemp =  retriveXMLTagX("Atlas_IoTDDL", "Atlas_Thing", "Administrative_Metadata","Network_Manager","Type");
            	((device_data_t*)deviceObj->userData)->Type = (char*)malloc(sizeof(char)* (sizeof(typetemp)+1));
		strcpy(((device_data_t*)deviceObj->userData)->Type, typetemp);
            
		//Module Attribute
		char* moduletemp =  retriveXMLTagX("Atlas_IoTDDL", "Atlas_Thing", "Administrative_Metadata","Network_Manager","Module");
            	((device_data_t*)deviceObj->userData)->Module = (char*)malloc(sizeof(char)* (sizeof(moduletemp)+1));
		strcpy(((device_data_t*)deviceObj->userData)->Module, moduletemp);

		//CommProtocol Attribute
		char* commProttemp =  retriveXMLTagX("Atlas_IoTDDL", "Atlas_Thing", "Administrative_Metadata","Network_Manager","Communication_Protocol");
            	((device_data_t*)deviceObj->userData)->CommProtocol = (char*)malloc(sizeof(char)* (sizeof(commProttemp)+1));
		strcpy(((device_data_t*)deviceObj->userData)->CommProtocol, commProttemp);


		//NetName Attribute
		char* NNametemp =  retriveXMLTagX("Atlas_IoTDDL", "Atlas_Thing", "Administrative_Metadata","Network_Manager","Network_Name");
            	((device_data_t*)deviceObj->userData)->NetName = (char*)malloc(sizeof(char)* (sizeof(NNametemp)+1));
		strcpy(((device_data_t*)deviceObj->userData)->NetName, NNametemp);


		//NetPasscode Attribute
		char* NCodetemp =  retriveXMLTagX("Atlas_IoTDDL", "Atlas_Thing", "Administrative_Metadata","Network_Manager","Network_Passcode");
            	((device_data_t*)deviceObj->userData)->NetPasscode = (char*)malloc(sizeof(char)* (sizeof(NCodetemp)+1));
		strcpy(((device_data_t*)deviceObj->userData)->NetPasscode, NCodetemp);


		//Broker Vendor Attribute
		char* Bvendortemp =  retriveXMLTagX("Atlas_Thing","Administrative_Metadata","Network_Manager","InterOperableCommunication","BrokerVendor");
            	((device_data_t*)deviceObj->userData)->BrokerVendor = (char*)malloc(sizeof(char)* (sizeof(Bvendortemp)+1));
		strcpy(((device_data_t*)deviceObj->userData)->BrokerVendor, Bvendortemp);


		//Broker URL Attribute
		char* Burltemp =  retriveXMLTagX("Atlas_Thing","Administrative_Metadata","Network_Manager","InterOperableCommunication","BrokerURL");
            	((device_data_t*)deviceObj->userData)->BrokerURL = (char*)malloc(sizeof(char)* (sizeof(Burltemp)+1));
		//strcpy(((device_data_t*)deviceObj->userData)->BrokerURL, Burltemp);


		//Broker dashboard Attribute
		char* Bdashtemp =  retriveXMLTagX("Atlas_Thing","Administrative_Metadata","Network_Manager","InterOperableCommunication","BrokerDashboard");
            	((device_data_t*)deviceObj->userData)->BrokerDashboard = (char*)malloc(sizeof(char)* (sizeof(Bdashtemp)+1));
		strcpy(((device_data_t*)deviceObj->userData)->BrokerDashboard, Bdashtemp);


		//OMA Name Attribute
		char* OMANtemp =  retriveXMLTagX("Atlas_Thing","Administrative_Metadata","Network_Manager","OMA_DM","AccessName");
            	((device_data_t*)deviceObj->userData)->OMADM_Name = (char*)malloc(sizeof(char)* (sizeof(OMANtemp)+1));
		strcpy(((device_data_t*)deviceObj->userData)->OMADM_Name, OMANtemp);


		//OMA IP Attribute
		char* OMAIPtemp =  retriveXMLTagX("Atlas_Thing","Administrative_Metadata","Network_Manager","OMA_DM","IP");
            	((device_data_t*)deviceObj->userData)->OMADM_IP = (char*)malloc(sizeof(char)* (sizeof(OMAIPtemp)+1));
		strcpy(((device_data_t*)deviceObj->userData)->OMADM_IP, OMAIPtemp);

		//OMA Port Attribute
		char* OMAPTtemp =  retriveXMLTagX("Atlas_Thing","Administrative_Metadata","Network_Manager","OMA_DM","Port");
            	((device_data_t*)deviceObj->userData)->OMADM_Port = (char*)malloc(sizeof(char)* (sizeof(OMAPTtemp)+1));
		strcpy(((device_data_t*)deviceObj->userData)->OMADM_Port, OMAPTtemp);

        }
        else{
            lwm2m_free(deviceObj->instanceList);
            lwm2m_free(deviceObj);
            deviceObj = NULL;
        }
    }
    return deviceObj;
}

void free_object_Atlas_Thing_Administrative_Metadata(lwm2m_object_t * objectP) {
    if (NULL != objectP->userData) {
        lwm2m_free(objectP->userData);
        objectP->userData = NULL;
    }
    if (NULL != objectP->instanceList) {
        lwm2m_free(objectP->instanceList);
        objectP->instanceList = NULL;
    }
    lwm2m_free(objectP);
}

uint8_t Atlas_Thing_Administrative_Metadata_change(lwm2m_data_t * dataArray, lwm2m_object_t * objectP) {



    printf("change value ... \n");

    uint8_t result;
    result = COAP_204_CHANGED;
    /*
    switch (dataArray->id){
    	case RES_O_VENDOR:{
                int64_t value;
                if (1 == lwm2m_data_decode_string(dataArray, &value)){
                    if ((0 <= value) && (100 >= value)){
                        ((device_data_t*)(objectP->userData))->vendor = value;
                        result = COAP_204_CHANGED;
                    }
                    else{
                        result = COAP_400_BAD_REQUEST;
                    }
                }
                else{
                    result = COAP_400_BAD_REQUEST;
                }
            }
            break;
        case RES_O_TYPE:
            if (1 == lwm2m_data_decode_string(dataArray, &((device_data_t*)(objectP->userData))->type)){
                result = COAP_204_CHANGED;
            }
            else{
                result = COAP_400_BAD_REQUEST;
            }
            break;
        case RES_O_ATLASVERSION:
            if (1 == lwm2m_data_decode_float(dataArray, &((device_data_t*)(objectP->userData))->atlas_release)){
                result = COAP_204_CHANGED;
            }
            else{
                result = COAP_400_BAD_REQUEST;
            }
            break;
        default:
            result = COAP_405_METHOD_NOT_ALLOWED;
            break;
        }
    */
    return result;
}


/*******************************************************************************
 *  Developer: Ahmed E. Khaled 
 *  Date: June, 2020
 *  This object is single instance only, 
 *  Mandatory for Atlas thing architecture 
 *
 *  =====================================================================
 *          Object                   |  ID   | M-Instances | Mandatoty |
 *  AtlasThing_descriptive_metadata  | 8000  |    No       |    Yes    |
 *  =====================================================================
 *  Ressources            Supported    Multiple
 *  Name           | ID | Operations | Instances | Mandatory |  Type   | Range | Units |               Description                  |
 *                 |    |            |           |           |         |       |       |                                            |
 *  vendor         |  0 |    R/W     |    No     |    Yes    | String  |       |       |  Thing Vendor                              |
 *  owner          |  1 |    R/W     |    No     |    Yes    | String  |       |       |  Thing Owner 			 	    |
 *  name           |  2 |    R/W     |    No     |    Yes    | String  |       |       |  Thing Name                                |
 *  description    |  3 |    R/W     |    No     |    Yes    | String  |       |       |  Long description on the Thing             |
 *  type           |  4 |    R/W     |    No     |    Yes    | String  |       |       |  Type of Thing (Bit, SW, Thing of Things)  |
 *     ...................................................................................................................
 *     ...................................................................................................................
 *     ...................................................................................................................

 *  atlasVersion   | 12 |    R/W     |    No     |    Yes    | Float   |       |       |  Atlas architecutre version                |
 *  releaseDate    | 13 |    R/W     |    No     |    Yes    | Float   |       |       |  release date of the mounted architecutre  |
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
#define ATLAS_THING_DESCRIPTIVE_METAdATA_OBJECT_ID    8000
#define RES_VENDOR               0
#define RES_OWNER                1
#define RES_NAME                 2
#define RES_DESCRIPTION          3
#define RES_TYPE                 4
#define RES_MODEL                5
#define RES_SHORTDESCRIPTION     6           
#define RES_OPERATINGSYSTEM      7          
#define RES_ATID                 8
#define RES_SSID                 9
#define RES_SPACECOORDINATES     10          
#define RES_SPACEDESCRIPTION     11       
#define RES_ATLASVERSION         12
#define RES_RELEASEDATE          13



typedef struct{
    char* vendor;
    char* owner;
    char* name;
    char* description;
    char* type;
    char* model;
    char* ShortDescription;
    char* OperatingSystem;
    char* ATID;
    char* SSID;
    char* SpaceCoordinates;
    char* SpaceDescription;
    double atlasVersion;
    double releaseDate;

} device_data_t;


char *DDL_FilePath = "ConfigurationFiles/Atlas_IoTDDL.xml";

//Retrive data of an XML tag of forth level
void removeSubstr (char *string, char *sub) {
    char *match;
    int len = strlen(sub);
    while ((match = strstr(string, sub))) {
        *match = '\0';
        strcat(string, match+len);
    }
}

char* retriveXMLTag(char* mainTag, char* mainTag2, char* mainTag3, char* tag){
	char * S1 = (char *) malloc(1 + strlen(tag)+ 2);   		//<tag>
	strcpy(S1, "<");      	strcat(S1, tag);      	strcat(S1, ">");
	char * S2 = (char *) malloc(1 + strlen(tag)+ 3);   		//</tag>
	strcpy(S2, "</");      	strcat(S2, tag);      	strcat(S2, ">");
      	char * S3 = (char *) malloc(1 + strlen(mainTag)+ 2);   		//<mainTag>
	strcpy(S3, "<");      	strcat(S3, tag);      	strcat(S3, ">");
      	char * S4 = (char *) malloc(1 + strlen(mainTag2)+ 2);   	//<mainTag2>
	strcpy(S4, "<");      	strcat(S4, tag);      	strcat(S4, ">");
      	char * S5 = (char *) malloc(1 + strlen(mainTag3)+ 2);   	//<mainTag2>
	strcpy(S5, "<");      	strcat(S5, tag);      	strcat(S5, ">");

	bool mainTagB = false;
	bool mainTag2B = false;
	bool mainTag3B = false;
	size_t found=0;
	size_t found1=0;
    	size_t found2=0;
	FILE* file = fopen(DDL_FilePath, "r"); 
    	char line[256];

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
			if(strstr(line, S1)){		//found tag in the mainTag2 seciton
				removeSubstr(line, S1);
				removeSubstr(line, S2);
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
	    case RES_VENDOR:
	        lwm2m_data_encode_string(devDataP->vendor, dataP);
    		printf("Reading value of Vendor... \n");
	        return COAP_205_CONTENT;
	    case RES_OWNER:
	        lwm2m_data_encode_string(devDataP->owner, dataP);
    		printf("Reading value of owner... \n");
	        return COAP_205_CONTENT;
	    case RES_NAME:
	        lwm2m_data_encode_string(devDataP->name, dataP);
    		printf("Reading value of name... \n");
	        return COAP_205_CONTENT;
	    case RES_DESCRIPTION:
	        lwm2m_data_encode_string(devDataP->description, dataP);
    		printf("Reading value of Description... \n");
	        return COAP_205_CONTENT;
	    case RES_TYPE:
	        lwm2m_data_encode_string(devDataP->type, dataP);
    		printf("Reading value of Type... \n");
	        return COAP_205_CONTENT;
	    case RES_MODEL:
	        lwm2m_data_encode_string(devDataP->model, dataP);
    		printf("Reading value of Model... \n");
	        return COAP_205_CONTENT;
	    case RES_SHORTDESCRIPTION:
	        lwm2m_data_encode_string(devDataP->ShortDescription, dataP);
    		printf("Reading value of Short Description... \n");
	        return COAP_205_CONTENT;
	    case RES_OPERATINGSYSTEM:
	        lwm2m_data_encode_string(devDataP->OperatingSystem, dataP);
    		printf("Reading value of Operating System... \n");
	        return COAP_205_CONTENT;
	    case RES_ATID:
	        lwm2m_data_encode_string(devDataP->ATID, dataP);
    		printf("Reading value of ATID... \n");
	        return COAP_205_CONTENT;
	    case RES_SSID:
	        lwm2m_data_encode_string(devDataP->SSID, dataP);
    		printf("Reading value of SSID... \n");
	        return COAP_205_CONTENT;
	    case RES_SPACECOORDINATES:
	        lwm2m_data_encode_string(devDataP->SpaceCoordinates, dataP);
    		printf("Reading value of Space Coordinates... \n");
	        return COAP_205_CONTENT;
	    case RES_SPACEDESCRIPTION:
	        lwm2m_data_encode_string(devDataP->SpaceDescription, dataP);
    		printf("Reading value of Space Description... \n");
	        return COAP_205_CONTENT;
	    case RES_ATLASVERSION:
	        lwm2m_data_encode_float(devDataP->atlasVersion, dataP);
    		printf("Reading value of Atlas version... \n");
	        return COAP_205_CONTENT;
	    case RES_RELEASEDATE:
	        lwm2m_data_encode_float(devDataP->releaseDate, dataP);
    		printf("Reading value of Release Date... \n");
	        return COAP_205_CONTENT;
	    default:
	        return COAP_404_NOT_FOUND;
    }
}

static uint8_t prv_device_read(uint16_t instanceId,int * numDataP, lwm2m_data_t ** dataArrayP, lwm2m_object_t * objectP){

	printf("Reading from Athing_DescriptiveMetadata object \n");

    	if (instanceId != 0)   return COAP_404_NOT_FOUND;     // this is a single instance object
        
    	uint8_t result;
    	int i;
    	if (*numDataP == 0){        // is the server asking for the full object
        	uint16_t resList[] = { 
        		RES_VENDOR,
        		RES_OWNER,
        		RES_NAME,
        		RES_DESCRIPTION,
        		RES_TYPE,
			RES_MODEL,
			RES_SHORTDESCRIPTION,          
			RES_OPERATINGSYSTEM,          
			RES_ATID,
			RES_SSID,
			RES_SPACECOORDINATES,          
			RES_SPACEDESCRIPTION,
        		RES_ATLASVERSION,
        		RES_RELEASEDATE
        	};
        	int nbRes = sizeof(resList)/sizeof(uint16_t);

        	*dataArrayP = lwm2m_data_new(nbRes);
        	if (*dataArrayP == NULL) return COAP_500_INTERNAL_SERVER_ERROR;
        	*numDataP = nbRes;
        	for (i = 0 ; i < nbRes ; i++){
            		(*dataArrayP)[i].id = resList[i];
        	}
    	}

    	i = 0;
    	do{
        	result = prv_set_value((*dataArrayP)+i, (device_data_t*)(objectP->userData));
        	i++;
    	} while (i < *numDataP && result == COAP_205_CONTENT);
    	return result;
}

static uint8_t prv_device_discover(uint16_t instanceId, int * numDataP, lwm2m_data_t ** dataArrayP, lwm2m_object_t * objectP){

    	if (instanceId != 0)    return COAP_404_NOT_FOUND;// this is a single instance object
    	
	uint8_t result;
    	int i;
    	result = COAP_205_CONTENT;

    	if (*numDataP == 0){   // is the server asking for the full object ?
        	uint16_t resList[] = { 
        		RES_VENDOR,
        		RES_OWNER,
        		RES_NAME,
        		RES_DESCRIPTION,
        		RES_TYPE,
			RES_MODEL,
			RES_SHORTDESCRIPTION,          
			RES_OPERATINGSYSTEM,          
			RES_ATID,
			RES_SSID,
			RES_SPACECOORDINATES,          
			RES_SPACEDESCRIPTION,
        		RES_ATLASVERSION,
        		RES_RELEASEDATE
        	};
        	int nbRes = sizeof(resList) / sizeof(uint16_t);
        	*dataArrayP = lwm2m_data_new(nbRes);
        	if (*dataArrayP == NULL) 
			return COAP_500_INTERNAL_SERVER_ERROR;
        	*numDataP = nbRes;
        	for (i = 0; i < nbRes; i++)
			(*dataArrayP)[i].id = resList[i];

    	}else{
        	for (i = 0; i < *numDataP && result == COAP_205_CONTENT; i++){
            		switch ((*dataArrayP)[i].id){
	            		case RES_VENDOR:  		return COAP_205_CONTENT;
			    	case RES_OWNER: 		return COAP_205_CONTENT;
			    	case RES_NAME: 			return COAP_205_CONTENT;
			    	case RES_DESCRIPTION: 		return COAP_205_CONTENT;
			    	case RES_TYPE: 			return COAP_205_CONTENT;
				case RES_MODEL: 		return COAP_205_CONTENT;
				case RES_SHORTDESCRIPTION: 	return COAP_205_CONTENT;          
				case RES_OPERATINGSYSTEM: 	return COAP_205_CONTENT;          
				case RES_ATID: 			return COAP_205_CONTENT;
				case RES_SSID: 			return COAP_205_CONTENT;
				case RES_SPACECOORDINATES: 	return COAP_205_CONTENT;      
				case RES_SPACEDESCRIPTION: 	return COAP_205_CONTENT;
			    	case RES_ATLASVERSION:  	return COAP_205_CONTENT;
			    	case RES_RELEASEDATE:  		return COAP_205_CONTENT;
	            		default: 		
					result = COAP_404_NOT_FOUND;
            		}
        	}
    	}
    	return result;
}

static uint8_t prv_device_write(uint16_t instanceId, int numData, lwm2m_data_t * dataArray, lwm2m_object_t * objectP) {

	printf("Writing to Athing_DescriptiveMetadata object \n");
    	if (instanceId != 0) 	return COAP_404_NOT_FOUND;	// this is a single instance object
        
    	int i=0;
    	uint8_t result;
    	do{
        	switch (dataArray[i].id){
	        	case RES_VENDOR:
                                printf("Writing new Vendor\n");
	            		if(NULL != intRes){ 

					//clear the old value
					char* temp =  "\0";
            				((device_data_t*)objectP->userData)->vendor = (char*)malloc(sizeof(char)* (sizeof(temp)));
            				strcpy(((device_data_t*)objectP->userData)->vendor, temp);
					
					//set the new value
					strncpy(((device_data_t*)(objectP->userData))->vendor, (char*)dataArray[i].value.asBuffer.buffer, dataArray[i].value.asBuffer.length);
	            			result = COAP_204_CHANGED;
                                	printf("The new Vendor: %s\n", ((device_data_t*)(objectP->userData))->vendor);
    				}else{
					strncpy(((device_data_t*)(objectP->userData))->vendor, intRes, strlen(intRes));
	            			result = COAP_204_CHANGED;
                                }
	            		break;

	        	case RES_OWNER:
                                printf("Writing new Owner\n");
	            		if(NULL != intRes){ 

					//clear the old value
					char* temp =  "\0";
            				((device_data_t*)objectP->userData)->owner = (char*)malloc(sizeof(char)* (sizeof(temp)));
            				strcpy(((device_data_t*)objectP->userData)->owner, temp);
					
					//set the new value
					strncpy(((device_data_t*)(objectP->userData))->owner, (char*)dataArray[i].value.asBuffer.buffer, dataArray[i].value.asBuffer.length);
	            			result = COAP_204_CHANGED;
                                	printf("The new Owner: %s\n", ((device_data_t*)(objectP->userData))->owner);
    				}else{
					strncpy(((device_data_t*)(objectP->userData))->owner, intRes, strlen(intRes));
	            			result = COAP_204_CHANGED;
                                }
	            		break;

	        	case RES_NAME:
                                printf("Writing new Name\n");
	            		if(NULL != intRes){ 

					//clear the old value
					char* temp =  "\0";
            				((device_data_t*)objectP->userData)->name = (char*)malloc(sizeof(char)* (sizeof(temp)));
            				strcpy(((device_data_t*)objectP->userData)->name, temp);
					
					//set the new value
					strncpy(((device_data_t*)(objectP->userData))->name, (char*)dataArray[i].value.asBuffer.buffer, dataArray[i].value.asBuffer.length);
	            			result = COAP_204_CHANGED;
                                	printf("The new Name: %s\n", ((device_data_t*)(objectP->userData))->name);
    				}else{
					strncpy(((device_data_t*)(objectP->userData))->name, intRes, strlen(intRes));
	            			result = COAP_204_CHANGED;
                                }
	            		break;

	        	case RES_DESCRIPTION:
                                printf("Writing new Description\n");
	            		if(NULL != intRes){ 

					//clear the old value
					char* temp =  "\0";
            				((device_data_t*)objectP->userData)->description = (char*)malloc(sizeof(char)* (sizeof(temp)));
            				strcpy(((device_data_t*)objectP->userData)->description, temp);
					
					//set the new value
					strncpy(((device_data_t*)(objectP->userData))->description, (char*)dataArray[i].value.asBuffer.buffer, dataArray[i].value.asBuffer.length);
	            			result = COAP_204_CHANGED;
                                	printf("The new description: %s\n", ((device_data_t*)(objectP->userData))->description);
    				}else{
					strncpy(((device_data_t*)(objectP->userData))->description, intRes, strlen(intRes));
	            			result = COAP_204_CHANGED;
                                }
	            		break;

	        	case RES_TYPE:
                                printf("Writing new type\n");
	            		if(NULL != intRes){ 

					//clear the old value
					char* temp =  "\0";
            				((device_data_t*)objectP->userData)->type = (char*)malloc(sizeof(char)* (sizeof(temp)));
            				strcpy(((device_data_t*)objectP->userData)->type, temp);
					
					//set the new value
					strncpy(((device_data_t*)(objectP->userData))->type, (char*)dataArray[i].value.asBuffer.buffer, dataArray[i].value.asBuffer.length);
	            			result = COAP_204_CHANGED;
                                	printf("The new type: %s\n", ((device_data_t*)(objectP->userData))->type);
    				}else{
					strncpy(((device_data_t*)(objectP->userData))->type, intRes, strlen(intRes));
	            			result = COAP_204_CHANGED;
                                }
	            		break;

	        	case RES_MODEL:
                                printf("Writing new Model\n");
	            		if(NULL != intRes){ 

					//clear the old value
					char* temp =  "\0";
            				((device_data_t*)objectP->userData)->model = (char*)malloc(sizeof(char)* (sizeof(temp)));
            				strcpy(((device_data_t*)objectP->userData)->model, temp);
					
					//set the new value
					strncpy(((device_data_t*)(objectP->userData))->model, (char*)dataArray[i].value.asBuffer.buffer, dataArray[i].value.asBuffer.length);
	            			result = COAP_204_CHANGED;
                                	printf("The new Model: %s\n", ((device_data_t*)(objectP->userData))->model);
    				}else{
					strncpy(((device_data_t*)(objectP->userData))->model, intRes, strlen(intRes));
	            			result = COAP_204_CHANGED;
                                }
	            		break;

	        	case RES_SHORTDESCRIPTION:
                                printf("Writing new Short Description\n");
	            		if(NULL != intRes){ 

					//clear the old value
					char* temp =  "\0";
            				((device_data_t*)objectP->userData)->ShortDescription = (char*)malloc(sizeof(char)* (sizeof(temp)));
            				strcpy(((device_data_t*)objectP->userData)->ShortDescription, temp);
					
					//set the new value
					strncpy(((device_data_t*)(objectP->userData))->ShortDescription, (char*)dataArray[i].value.asBuffer.buffer, dataArray[i].value.asBuffer.length);
	            			result = COAP_204_CHANGED;
                                	printf("The new Short Description: %s\n", ((device_data_t*)(objectP->userData))->ShortDescription);
    				}else{
					strncpy(((device_data_t*)(objectP->userData))->ShortDescription, intRes, strlen(intRes));
	            			result = COAP_204_CHANGED;
                                }
	            		break;

	        	case RES_OPERATINGSYSTEM:
                                printf("Writing new OS\n");
	            		if(NULL != intRes){ 

					//clear the old value
					char* temp =  "\0";
            				((device_data_t*)objectP->userData)->OperatingSystem = (char*)malloc(sizeof(char)* (sizeof(temp)));
            				strcpy(((device_data_t*)objectP->userData)->OperatingSystem, temp);
					
					//set the new value
					strncpy(((device_data_t*)(objectP->userData))->OperatingSystem, (char*)dataArray[i].value.asBuffer.buffer, dataArray[i].value.asBuffer.length);
	            			result = COAP_204_CHANGED;
                                	printf("The new OS: %s\n", ((device_data_t*)(objectP->userData))->OperatingSystem);
    				}else{
					strncpy(((device_data_t*)(objectP->userData))->OperatingSystem, intRes, strlen(intRes));
	            			result = COAP_204_CHANGED;
                                }
	            		break;

	        	case RES_ATID:
                                printf("Writing new ATID\n");
	            		if(NULL != intRes){ 

					//clear the old value
					char* temp =  "\0";
            				((device_data_t*)objectP->userData)->ATID = (char*)malloc(sizeof(char)* (sizeof(temp)));
            				strcpy(((device_data_t*)objectP->userData)->ATID, temp);
					
					//set the new value
					strncpy(((device_data_t*)(objectP->userData))->ATID, (char*)dataArray[i].value.asBuffer.buffer, dataArray[i].value.asBuffer.length);
	            			result = COAP_204_CHANGED;
                                	printf("The new ATID: %s\n", ((device_data_t*)(objectP->userData))->ATID);
    				}else{
					strncpy(((device_data_t*)(objectP->userData))->ATID, intRes, strlen(intRes));
	            			result = COAP_204_CHANGED;
                                }
	            		break;

	        	case RES_SSID:
                                printf("Writing new SSID\n");
	            		if(NULL != intRes){ 

					//clear the old value
					char* temp =  "\0";
            				((device_data_t*)objectP->userData)->SSID = (char*)malloc(sizeof(char)* (sizeof(temp)));
            				strcpy(((device_data_t*)objectP->userData)->SSID, temp);
					
					//set the new value
					strncpy(((device_data_t*)(objectP->userData))->SSID, (char*)dataArray[i].value.asBuffer.buffer, dataArray[i].value.asBuffer.length);
	            			result = COAP_204_CHANGED;
                                	printf("The new SSID: %s\n", ((device_data_t*)(objectP->userData))->SSID);
    				}else{
					strncpy(((device_data_t*)(objectP->userData))->SSID, intRes, strlen(intRes));
	            			result = COAP_204_CHANGED;
                                }
	            		break;

	        	case RES_SPACECOORDINATES:
                                printf("Writing new Space Coordinates\n");
	            		if(NULL != intRes){ 

					//clear the old value
					char* temp =  "\0";
            				((device_data_t*)objectP->userData)->SpaceCoordinates = (char*)malloc(sizeof(char)* (sizeof(temp)));
            				strcpy(((device_data_t*)objectP->userData)->SpaceCoordinates, temp);
					
					//set the new value
					strncpy(((device_data_t*)(objectP->userData))->SpaceCoordinates, (char*)dataArray[i].value.asBuffer.buffer, dataArray[i].value.asBuffer.length);
	            			result = COAP_204_CHANGED;
                                	printf("The new Space Coordinates: %s\n", ((device_data_t*)(objectP->userData))->SpaceCoordinates);
    				}else{
					strncpy(((device_data_t*)(objectP->userData))->SpaceCoordinates, intRes, strlen(intRes));
	            			result = COAP_204_CHANGED;
                                }
	            		break;

	        	case RES_SPACEDESCRIPTION:
                                printf("Writing new Space Description\n");
	            		if(NULL != intRes){ 

					//clear the old value
					char* temp =  "\0";
            				((device_data_t*)objectP->userData)->SpaceDescription = (char*)malloc(sizeof(char)* (sizeof(temp)));
            				strcpy(((device_data_t*)objectP->userData)->SpaceDescription, temp);
					
					//set the new value
					strncpy(((device_data_t*)(objectP->userData))->SpaceDescription, (char*)dataArray[i].value.asBuffer.buffer, dataArray[i].value.asBuffer.length);
	            			result = COAP_204_CHANGED;
                                	printf("The new Space Description: %s\n", ((device_data_t*)(objectP->userData))->SpaceDescription);
    				}else{
					strncpy(((device_data_t*)(objectP->userData))->SpaceDescription, intRes, strlen(intRes));
	            			result = COAP_204_CHANGED;
                                }
	            		break;

		    	case RES_ATLASVERSION:
		        	((device_data_t*)(objectP->userData))->atlasVersion = dataArray[i].value.asFloat;
	            		result = COAP_204_CHANGED;
	            		break;

		    	case RES_RELEASEDATE:
		        	((device_data_t*)(objectP->userData))->releaseDate = dataArray[i].value.asFloat;
	            		result = COAP_204_CHANGED;
	            		break;

		    	default:
		        	result = COAP_405_METHOD_NOT_ALLOWED;
        	} i++;
    	} while (i < numData && result == COAP_204_CHANGED);
    
	return result;
}

static uint8_t prv_device_execute(uint16_t instanceId, uint16_t resourceId,
                                  uint8_t * buffer, int length, lwm2m_object_t * objectP){
    
        printf("device execute ... \n");
    	if (instanceId != 0)    return COAP_404_NOT_FOUND;      // this is a single instance object
    	if (length != 0) 	return COAP_400_BAD_REQUEST;
    	switch (resourceId)	{
    				case RES_VENDOR: 		return COAP_204_CHANGED;
			    	case RES_OWNER: 		return COAP_204_CHANGED;
			    	case RES_NAME: 			return COAP_204_CHANGED;
			    	case RES_DESCRIPTION: 		return COAP_204_CHANGED;
			    	case RES_TYPE: 			return COAP_204_CHANGED;
				case RES_MODEL: 		return COAP_204_CHANGED;
				case RES_SHORTDESCRIPTION: 	return COAP_204_CHANGED;          
				case RES_OPERATINGSYSTEM: 	return COAP_204_CHANGED;          
				case RES_ATID: 			return COAP_204_CHANGED;
				case RES_SSID: 			return COAP_204_CHANGED;
				case RES_SPACECOORDINATES: 	return COAP_204_CHANGED;      
				case RES_SPACEDESCRIPTION: 	return COAP_204_CHANGED;
			    	case RES_ATLASVERSION: 		return COAP_204_CHANGED;
			    	case RES_RELEASEDATE: 		return COAP_204_CHANGED;
	            		default: 			return COAP_405_METHOD_NOT_ALLOWED;
    	}
}

void display_Atlas_Thing_Descriptive_Metadata_object(lwm2m_object_t * object) {
	device_data_t * data = (device_data_t *)object->userData;

	fprintf(stdout, "  /%u:  Atlas Thing Descriptive-Metadata:\r\n", object->objID);
    	if (NULL != data) {
        	printf("%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %f, %f\r\n",
			        data->vendor,
			        data->owner,
			        data->name,
			        data->description,
			        data->type,
				data->model,
				data->ShortDescription,
				data->OperatingSystem,
				data->ATID,
				data->SSID,
				data->SpaceCoordinates,
				data->SpaceDescription,
			        data->atlasVersion,
			        data->releaseDate);
    	}
}

lwm2m_object_t * get_object_Atlas_Thing_Descriptive_Metadata(){

    lwm2m_object_t * deviceObj;  //structure definition in liblwm2m.h
    deviceObj = (lwm2m_object_t *)lwm2m_malloc(sizeof(lwm2m_object_t));

    if (NULL != deviceObj) {
        memset(deviceObj, 0, sizeof(lwm2m_object_t));

        //setting Object ID and InstanceList
        deviceObj->objID 	= ATLAS_THING_DESCRIPTIVE_METAdATA_OBJECT_ID;
        deviceObj->instanceList = (lwm2m_list_t *)lwm2m_malloc(sizeof(lwm2m_list_t));
        
	if (NULL != deviceObj->instanceList)
            memset(deviceObj->instanceList, 0, sizeof(lwm2m_list_t));
        else{
            lwm2m_free(deviceObj);
            return NULL;
        }

        //setting functions to read/write/discover/execute functions
        deviceObj->readFunc     = prv_device_read;
        deviceObj->discoverFunc = prv_device_discover;
        deviceObj->writeFunc    = prv_device_write;
        deviceObj->executeFunc  = prv_device_execute;


        //setting Object's User defined data
        deviceObj->userData 	= lwm2m_malloc(sizeof(device_data_t));
        if (NULL != deviceObj->userData){

		//Vendor Attribute
		char* vendortemp =  retriveXMLTag("Atlas_IoTDDL","Atlas_Thing","Descriptive_Metadata","Thing_Vendor");
            	((device_data_t*)deviceObj->userData)->vendor = (char*)malloc(sizeof(char)* (sizeof(vendortemp)+1));
            	strcpy(((device_data_t*)deviceObj->userData)->vendor, vendortemp);
            
		//Owner Attribute
		char* ownertemp =  retriveXMLTag("Atlas_IoTDDL","Atlas_Thing","Descriptive_Metadata","Thing_Owner");
            	((device_data_t*)deviceObj->userData)->owner = (char*)malloc(sizeof(char)* (sizeof(ownertemp)+1));
            	strcpy(((device_data_t*)deviceObj->userData)->owner, ownertemp);

		//Name Attribute
		char* nametemp =  retriveXMLTag("Atlas_IoTDDL","Atlas_Thing","Descriptive_Metadata","Thing_Name");
            	((device_data_t*)deviceObj->userData)->name = (char*)malloc(sizeof(char)* (sizeof(nametemp)+1));
            	strcpy(((device_data_t*)deviceObj->userData)->name, nametemp);

		//Description Attribute
		char* destemp =  retriveXMLTag("Atlas_IoTDDL","Atlas_Thing","Descriptive_Metadata","Thing_Description");
            	((device_data_t*)deviceObj->userData)->description = (char*)malloc(sizeof(char)* (sizeof(destemp)+1));
            	strcpy(((device_data_t*)deviceObj->userData)->description, destemp);

		//Type Attribute
		char* typetemp =  retriveXMLTag("Atlas_IoTDDL","Atlas_Thing","Descriptive_Metadata","Thing_Type");
            	((device_data_t*)deviceObj->userData)->type = (char*)malloc(sizeof(char)* (sizeof(typetemp)+1));
            	strcpy(((device_data_t*)deviceObj->userData)->type, typetemp);

		//Model Attribute
		char* modeltemp =  retriveXMLTag("Atlas_IoTDDL","Atlas_Thing","Descriptive_Metadata","Thing_Model");
            	((device_data_t*)deviceObj->userData)->model = (char*)malloc(sizeof(char)* (sizeof(modeltemp)+1));
            	strcpy(((device_data_t*)deviceObj->userData)->model, modeltemp);

		//ShortDescription Attribute
		char* sdestemp =  retriveXMLTag("Atlas_IoTDDL","Atlas_Thing","Descriptive_Metadata","Thing_ShortDescription");
            	((device_data_t*)deviceObj->userData)->ShortDescription = (char*)malloc(sizeof(char)* (sizeof(sdestemp)+1));
            	strcpy(((device_data_t*)deviceObj->userData)->ShortDescription, sdestemp);

		//OperatingSystem Attribute
		char* OStemp =  retriveXMLTag("Atlas_IoTDDL","Atlas_Thing","Descriptive_Metadata","Thing_OperatingSystem");
            	((device_data_t*)deviceObj->userData)->OperatingSystem = (char*)malloc(sizeof(char)* (sizeof(OStemp)+1));
            	strcpy(((device_data_t*)deviceObj->userData)->OperatingSystem, OStemp);

		//ATID Attribute
		char* ATIDtemp =  retriveXMLTag("Atlas_IoTDDL","Atlas_Thing","Descriptive_Metadata","Thing_ATID");
            	((device_data_t*)deviceObj->userData)->ATID = (char*)malloc(sizeof(char)* (sizeof(ATIDtemp)+1));
            	strcpy(((device_data_t*)deviceObj->userData)->ATID, ATIDtemp);

		//SSID Attribute
		char* SSIDtemp =  retriveXMLTag("Atlas_IoTDDL","Atlas_Thing","Descriptive_Metadata","Thing_SSID");
            	((device_data_t*)deviceObj->userData)->SSID = (char*)malloc(sizeof(char)* (sizeof(SSIDtemp)+1));
            	strcpy(((device_data_t*)deviceObj->userData)->SSID, SSIDtemp);

		//Space Coordinates Attribute
		char* scoordtemp =  retriveXMLTag("Atlas_IoTDDL","Atlas_Thing","Descriptive_Metadata","Thing_SpaceCoordinates");
            	((device_data_t*)deviceObj->userData)->SpaceCoordinates = (char*)malloc(sizeof(char)* (sizeof(scoordtemp)+1));
            	strcpy(((device_data_t*)deviceObj->userData)->SpaceCoordinates, scoordtemp);

		//Space Description Attribute
		char* ssdestemp =  retriveXMLTag("Atlas_IoTDDL","Atlas_Thing","Descriptive_Metadata","Thing_SpaceDescription");
            	((device_data_t*)deviceObj->userData)->SpaceDescription = (char*)malloc(sizeof(char)* (sizeof(ssdestemp)+1));
            	strcpy(((device_data_t*)deviceObj->userData)->SpaceDescription, ssdestemp);

		//Atlas Version Attribute
            	((device_data_t*)deviceObj->userData)->atlasVersion  = 1.0;
            	
		//Release Date Attribute
		((device_data_t*)deviceObj->userData)->releaseDate  = 1.0;

        }
        else{
            lwm2m_free(deviceObj->instanceList);
            lwm2m_free(deviceObj);
            deviceObj = NULL;
        }
    }
    return deviceObj;
}

void free_object_Atlas_Thing_Descriptive_Metadata(lwm2m_object_t * objectP) {
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

uint8_t Atlas_Thing_Descriptive_Metadata_change(lwm2m_data_t * dataArray, lwm2m_object_t * objectP) {



    printf("change value ... \n");

    uint8_t result;
    result = COAP_204_CHANGED;
    /*
    switch (dataArray->id){
    	case RES_VENDOR:{
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
        case RES_TYPE:
            if (1 == lwm2m_data_decode_string(dataArray, &((device_data_t*)(objectP->userData))->type)){
                result = COAP_204_CHANGED;
            }
            else{
                result = COAP_400_BAD_REQUEST;
            }
            break;
        case RES_ATLASVERSION:
            if (1 == lwm2m_data_decode_float(dataArray, &((device_data_t*)(objectP->userData))->atlasVersion)){
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


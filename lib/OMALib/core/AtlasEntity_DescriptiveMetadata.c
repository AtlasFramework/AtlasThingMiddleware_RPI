/*******************************************************************************
 *  Developer: Ahmed E. Khaled, CISE-UF 
 *  Date: June 9th, 2016
 *  This object is single instance only, 
 *  Mandatory for Atlas thing architecture 
 *
 *  =====================================================================
 *          Object                   |  ID  | M-Instances | Mandatoty |
 *  AtlasThing_descriptive_metadata  | 8001 |    No       |    Yes    |
 *  =====================================================================
 *  Ressources            Supported    Multiple
 *  Name           | ID | Operations | Instances | Mandatory |  Type   | Range | Units |               Description                  |
 *                 |    |            |           |           |         |       |       |  Name of Thing Vendor                      |
 *  vendor         |  0 |    R/W     |    No     |    Yes    | String  |       |       |  Name of Thing Vendor                      |
 *  owner          |  1 |    R/W     |    No     |    Yes    | String  |       |       |  Type of Thing (Bit, SW, Thing of Things)  |
 *  name           |  2 |    R/W     |    No     |    Yes    | String  |       |       |  Type of Thing (Bit, SW, Thing of Things)  |
 *  description    |  3 |    R/W     |    No     |    Yes    | String  |       |       |  Type of Thing (Bit, SW, Thing of Things)  |
 *  type           |  4 |    R/W     |    No     |    Yes    | String  |       |       |  Type of Thing (Bit, SW, Thing of Things)  |
 *  atlasVersion   |  5 |    R/W     |    No     |    Yes    | Float   |       |       |  Atlas architecutre version                |
 *  releaseDate    |  6 |    R/W     |    No     |    Yes    | Float   |       |       |  Atlas architecutre version                |
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
#define ATLAS_ENTITY_DESCRIPTIVE_METAdATA_OBJECT_ID    8002
#define RES_VENDOR               0
#define RES_OWNER                1
#define RES_NAME                 2
#define RES_DESCRIPTION          3
#define RES_TYPE                 4
#define RES_ATLASVERSION         5
#define RES_RELEASEDATE          6


typedef struct{
    char* vendor;
    char* owner;
    char* name;
    char* description;
    char* type;
    double atlasVersion;
    double releaseDate;
} device_data_t;


static uint8_t prv_set_value(lwm2m_data_t * dataP, device_data_t * devDataP){
    switch (dataP->id){
	    case RES_VENDOR:
	        lwm2m_data_encode_string(devDataP->vendor, dataP);
	        return COAP_205_CONTENT;
	    case RES_OWNER:
	        lwm2m_data_encode_string(devDataP->owner, dataP);
	        return COAP_205_CONTENT;
	    case RES_NAME:
	        lwm2m_data_encode_string(devDataP->name, dataP);
	        return COAP_205_CONTENT;
	    case RES_DESCRIPTION:
	        lwm2m_data_encode_string(devDataP->description, dataP);
	        return COAP_205_CONTENT;
	    case RES_TYPE:
	        lwm2m_data_encode_string(devDataP->type, dataP);
	        return COAP_205_CONTENT;
	    case RES_ATLASVERSION:
	        lwm2m_data_encode_float(devDataP->atlasVersion, dataP);
	        return COAP_205_CONTENT;
	    case RES_RELEASEDATE:
	        lwm2m_data_encode_float(devDataP->releaseDate, dataP);
	        return COAP_205_CONTENT;
	    default:
	        return COAP_404_NOT_FOUND;
    }
}

static uint8_t prv_device_read(uint16_t instanceId,int * numDataP,
                               lwm2m_data_t ** dataArrayP,
                               lwm2m_object_t * objectP){
    if (instanceId != 0)    {     // this is a single instance object
        return COAP_404_NOT_FOUND;
    }

    uint8_t result;
    int i;
    if (*numDataP == 0){        // is the server asking for the full object
        uint16_t resList[] = { 
        		RES_VENDOR,
        		RES_OWNER,
        		RES_NAME,
        		RES_DESCRIPTION,
        		RES_TYPE,
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

static uint8_t prv_device_discover(uint16_t instanceId, int * numDataP,
                                   lwm2m_data_t ** dataArrayP,
                                   lwm2m_object_t * objectP){

    if (instanceId != 0){    // this is a single instance object
        return COAP_404_NOT_FOUND;
    }

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
        		RES_ATLASVERSION,
        		RES_RELEASEDATE
        };
        int nbRes = sizeof(resList) / sizeof(uint16_t);
        *dataArrayP = lwm2m_data_new(nbRes);
        if (*dataArrayP == NULL) return COAP_500_INTERNAL_SERVER_ERROR;
        *numDataP = nbRes;
        for (i = 0; i < nbRes; i++){ (*dataArrayP)[i].id = resList[i]; }
    }
    else{
        for (i = 0; i < *numDataP && result == COAP_205_CONTENT; i++){
            switch ((*dataArrayP)[i].id){
	            case RES_VENDOR:
			        return COAP_205_CONTENT;
			    case RES_OWNER:
			        return COAP_205_CONTENT;
			    case RES_NAME:
			        return COAP_205_CONTENT;
			    case RES_DESCRIPTION:
			        return COAP_205_CONTENT;
			    case RES_TYPE:
			        return COAP_205_CONTENT;
			    case RES_ATLASVERSION:
			        return COAP_205_CONTENT;
			    case RES_RELEASEDATE:
			        return COAP_205_CONTENT;
	            default:
	                result = COAP_404_NOT_FOUND;
            }
        }
    }
    return result;
}

static uint8_t prv_device_write(uint16_t instanceId, int numData,
                                lwm2m_data_t * dataArray, lwm2m_object_t * objectP) {
    
    if (instanceId != 0){  // this is a single instance object
        return COAP_404_NOT_FOUND;
    }
    int i;
    uint8_t result;
    i = 0;

    do{
        switch (dataArray[i].id){
	        case RES_VENDOR:
	            strncpy(((device_data_t*)(objectP->userData))->vendor, (char*)dataArray[i].value.asBuffer.buffer, dataArray[i].value.asBuffer.length);
	            result = COAP_204_CHANGED;
	            break;
		    case RES_OWNER:
	            strncpy(((device_data_t*)(objectP->userData))->owner, (char*)dataArray[i].value.asBuffer.buffer, dataArray[i].value.asBuffer.length);
	            result = COAP_204_CHANGED;
	            break;
		    case RES_NAME:
	            strncpy(((device_data_t*)(objectP->userData))->name, (char*)dataArray[i].value.asBuffer.buffer, dataArray[i].value.asBuffer.length);
	            result = COAP_204_CHANGED;
	            break;
		    case RES_DESCRIPTION:
	            strncpy(((device_data_t*)(objectP->userData))->description, (char*)dataArray[i].value.asBuffer.buffer, dataArray[i].value.asBuffer.length);
	            result = COAP_204_CHANGED;
	            break;
		    case RES_TYPE:
	            strncpy(((device_data_t*)(objectP->userData))->type, (char*)dataArray[i].value.asBuffer.buffer, dataArray[i].value.asBuffer.length);
	            result = COAP_204_CHANGED;
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
        }
        i++;
    } while (i < numData && result == COAP_204_CHANGED);
    return result;
}

static uint8_t prv_device_execute(uint16_t instanceId, uint16_t resourceId,
                                  uint8_t * buffer, int length,
                                  lwm2m_object_t * objectP){
    
    if (instanceId != 0) {     // this is a single instance object
        return COAP_404_NOT_FOUND;
    }

    if (length != 0) return COAP_400_BAD_REQUEST;
    switch (resourceId){
    			case RES_VENDOR:
			        return COAP_204_CHANGED;
			    case RES_OWNER:
			        return COAP_204_CHANGED;
			    case RES_NAME:
			        return COAP_204_CHANGED;
			    case RES_DESCRIPTION:
			        return COAP_204_CHANGED;
			    case RES_TYPE:
			        return COAP_204_CHANGED;
			    case RES_ATLASVERSION:
			        return COAP_204_CHANGED;
			    case RES_RELEASEDATE:
			        return COAP_204_CHANGED;
	            default:
	                return COAP_405_METHOD_NOT_ALLOWED;
    }
}

void display_Atlas_Entity_Descriptive_Metadata_object(lwm2m_object_t * object) {
	device_data_t * data = (device_data_t *)object->userData;
    fprintf(stdout, "  /%u: Atlas Entity descriptive metadata object:\r\n", object->objID);
    if (NULL != data) {
        printf("%s, %s, %s, %s, %s, %f, %f\r\n",
			    data->vendor,
			    data->owner,
			    data->name,
			    data->description,
			    data->type,
			    data->atlasVersion,
			    data->releaseDate);
    }
}



lwm2m_object_t * get_object_Atlas_Entity_Descriptive_Metadata(){

    lwm2m_object_t * deviceObj;  //structure definition in liblwm2m.h
    deviceObj = (lwm2m_object_t *)lwm2m_malloc(sizeof(lwm2m_object_t));

    if (NULL != deviceObj) {
        memset(deviceObj, 0, sizeof(lwm2m_object_t));
        //setting Object ID and InstanceList
        deviceObj->objID = ATLAS_ENTITY_DESCRIPTIVE_METAdATA_OBJECT_ID;
        deviceObj->instanceList = (lwm2m_list_t *)lwm2m_malloc(sizeof(lwm2m_list_t));
        if (NULL != deviceObj->instanceList){
            memset(deviceObj->instanceList, 0, sizeof(lwm2m_list_t));
        }
        else{
            lwm2m_free(deviceObj);
            return NULL;
        }

        //setting functions to read/write resources' values
        deviceObj->readFunc     = prv_device_read;
        deviceObj->discoverFunc = prv_device_discover;
        deviceObj->writeFunc    = prv_device_write;
        deviceObj->executeFunc  = prv_device_execute;
        deviceObj->userData = lwm2m_malloc(sizeof(device_data_t));



        if (NULL != deviceObj->userData){
        char vendorx[] = "                      ";
            ((device_data_t*)deviceObj->userData)->vendor = (char*)malloc(sizeof(char)* (sizeof(vendorx)+1));
            strcpy(((device_data_t*)deviceObj->userData)->vendor, vendorx);
            ((device_data_t*)deviceObj->userData)->owner = (char*)malloc(sizeof(char)* (sizeof(vendorx)+1));
            strcpy(((device_data_t*)deviceObj->userData)->owner, vendorx);
            ((device_data_t*)deviceObj->userData)->name = (char*)malloc(sizeof(char)* (sizeof(vendorx)+1));
            strcpy(((device_data_t*)deviceObj->userData)->name, vendorx);
            ((device_data_t*)deviceObj->userData)->description = (char*)malloc(sizeof(char)* (sizeof(vendorx)+1));
            strcpy(((device_data_t*)deviceObj->userData)->description, vendorx);
            ((device_data_t*)deviceObj->userData)->type = (char*)malloc(sizeof(char)* (sizeof(vendorx)+1));
            strcpy(((device_data_t*)deviceObj->userData)->type, vendorx);
            ((device_data_t*)deviceObj->userData)->atlasVersion  = 1.0;
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

void free_object_Atlas_Entity_Descriptive_Metadata(lwm2m_object_t * objectP) {
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

uint8_t Atlas_Entity_Descriptive_Metadata_change(lwm2m_data_t * dataArray, lwm2m_object_t * objectP) {

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


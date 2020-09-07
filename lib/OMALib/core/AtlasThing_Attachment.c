/*******************************************************************************
 *  Developer: Ahmed E. Khaled, CISE-UF 
 *  Date: June 9th, 2016
 *  This object is single instance only, 
 *  Mandatory for Atlas thing architecture 
 *
 *  =====================================================================
 *          Object                 |  ID  | M-Instances | Mandatoty |
 *  AtlasThing_Attachment          | 8001 |    No       |    Yes    |
 *  =====================================================================
 *  Ressources                Supported    Multiple
 *  Name              | ID | Operations | Instances | Mandatory |  Type   | Range | Units |               Description                  |
 *  VendorOnlineDoc   |  0 |    R/W     |    No     |    Yes    | ObjRef  |       |       |  Name of Thing Vendor                      |
 *  DataRepository    |  1 |    R/W     |    No     |    Yes    | ObjRef  |       |       |  Type of Thing (Bit, SW, Thing of Things)  |
 *  =====================================================================
 */

#include "liblwm2m.h"
#include "objectHandler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// Object Id
#define ATLAS_THING_ATTACHMENT_OBJECT_ID    8001

// Resource Id's:
#define RES_VENDOR_ONLINE_DOC            0
#define RES_DATA_REPOSITORY              1

typedef struct{
    lwm2m_object_t* VendorOnlineDoc;
    lwm2m_object_t* DataRepository;
} device_data_t;

//not complete <<===========================================

static uint8_t prv_set_value(lwm2m_data_t * dataP, device_data_t * devDataP){
    switch (dataP->id){
	    case RES_VENDOR_ONLINE_DOC:
	        //lwm2m_data_encode_opaque(devDataP->VendorOnlineDoc, sizeof(lwm2m_object_t), dataP);
	        return COAP_205_CONTENT;
	    case RES_DATA_REPOSITORY:
	        //lwm2m_data_encode_opaque(devDataP->DataRepository, sizeof(lwm2m_object_t), dataP);
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
        		RES_VENDOR_ONLINE_DOC, 
        		RES_DATA_REPOSITORY
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
        		RES_VENDOR_ONLINE_DOC, 
        		RES_DATA_REPOSITORY
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
	            case RES_VENDOR_ONLINE_DOC:
		    	    return COAP_205_CONTENT;
			    case RES_DATA_REPOSITORY:
			        return COAP_205_CONTENT;
			    default:
			        return COAP_404_NOT_FOUND;
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
	        case RES_VENDOR_ONLINE_DOC:
			    //strncpy(((device_data_t*)(objectP->userData))->VendorOnlineDoc, 
			    //			(char*)dataArray[i].value.asBuffer.buffer, 
			    //				   dataArray[i].value.asBuffer.length);
	            result = COAP_204_CHANGED;
	            break;
	        case RES_DATA_REPOSITORY:
			    //strncpy(((device_data_t*)(objectP->userData))->DataRepository, 
			    //			(char*)dataArray[i].value.asBuffer.buffer, 
			    //				   dataArray[i].value.asBuffer.length);
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
	    case RES_VENDOR_ONLINE_DOC:
		     return COAP_205_CONTENT;
		case RES_DATA_REPOSITORY:
			 return COAP_205_CONTENT;
		default:
			 return COAP_405_METHOD_NOT_ALLOWED;
    }
}

void display_Atlas_Thing_Attachment_object(lwm2m_object_t * object) {
	device_data_t * data = (device_data_t *)object->userData;
    fprintf(stdout, "  /%u: Device object:\r\n", object->objID);
    /*if (NULL != data) {
        printf("%s, %s, %s, %s, %s, %s\r\n", 
		        data->vendor, 
		        data->uri,
			    data->logName,
			    data->logPass,
			    data->repKey,
			    data->messageFormat);
    }*/
}



lwm2m_object_t * get_Atlas_Thing_Attachment_object(){

    lwm2m_object_t * deviceObj;  //structure definition in liblwm2m.h
    deviceObj = (lwm2m_object_t *)lwm2m_malloc(sizeof(lwm2m_object_t));

    if (NULL != deviceObj) {
        memset(deviceObj, 0, sizeof(lwm2m_object_t));
        //setting Object ID and InstanceList
        deviceObj->objID = ATLAS_THING_ATTACHMENT_OBJECT_ID;
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
        	char vendorx[] = " ";
      	  	char typex[] = " ";
            //((device_data_t*)deviceObj->userData)->VendorOnlineDoc = malloc(sizeof(lwm2m_object_t));
            ///((device_data_t*)deviceObj->userData)->DataRepository = malloc(sizeof(lwm2m_object_t));

            //strcpy(((device_data_t*)deviceObj->userData)->VendorOnlineDoc, vendorx);

            //((device_data_t*)deviceObj->userData)->DataRepository = (char*)malloc(sizeof(char)* (sizeof(typex)+1));
            //strcpy(((device_data_t*)deviceObj->userData)->DataRepository, typex);
        }
        else{
            lwm2m_free(deviceObj->instanceList);
            lwm2m_free(deviceObj);
            deviceObj = NULL;
        }
    }
    return deviceObj;
}

void free_Atlas_Thing_Attachment_object(lwm2m_object_t * objectP) {
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

uint8_t Atlas_Thing_Attachment_object_change(lwm2m_data_t * dataArray, lwm2m_object_t * objectP) {

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

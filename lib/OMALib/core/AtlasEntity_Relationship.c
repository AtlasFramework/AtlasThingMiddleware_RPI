/*******************************************************************************
 *  Developer: Ahmed E. Khaled, CISE-UF 
 *  Date: June 9th, 2016
 *  This object is Multi-instance, 
 *  Optional for Atlas enity with Thing 
 *
 *  =====================================================================
 *          Object              |  ID  | M-Instances | Mandatoty |
 *  Atlas_entity_relationship   | 8005 |    Yes      |    No    |
 *  =====================================================================
 *  Ressources            Supported    Multiple
 *  Name           | ID | Operations | Instances | Mandatory |  Type   | Range | Units |               Description                  |
 *  rshiptype      |  0 |    R/W     |    No     |    Yes    | String  |       |       |  Name of Thing Vendor                      |
 *  rshipmate      |  1 |    R/W     |    No     |    Yes    | String  |       |       |  Type of Thing (Bit, SW, Thing of Things)  |
 *  =====================================================================
*/

#include "liblwm2m.h"
#include "objectHandler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


// Object Id
#define ATLAS_ENTITY_RELATIONSHIP_OBJECT_ID    8005
// Resource Id's:
#define RES_O_RSHIP_TYPE                0
#define RES_O_RSHIP_MATE                1

#define PRV_TLV_BUFFER_SIZE 64

typedef struct _prv_instance_{
    struct _prv_instance_ * next;   // matches lwm2m_list_t::next
    uint16_t shortID;               // matches lwm2m_list_t::id
    char* rship_type;
    char* rship_mate;
} prv_instance_t;


static uint8_t prv_read(uint16_t instanceId, int * numDataP,
                        lwm2m_data_t ** dataArrayP, lwm2m_object_t * objectP) {

    prv_instance_t * targetP;
    int i;

    targetP = (prv_instance_t *)lwm2m_list_find(objectP->instanceList, instanceId);
    if (NULL == targetP) 
       return COAP_404_NOT_FOUND;

    if (*numDataP == 0){
        *dataArrayP = lwm2m_data_new(2);
        if (*dataArrayP == NULL) return COAP_500_INTERNAL_SERVER_ERROR;
        *numDataP = 2;
        (*dataArrayP)[0].id = RES_O_RSHIP_TYPE;
        (*dataArrayP)[1].id = RES_O_RSHIP_MATE;
    }

    for (i = 0 ; i < *numDataP ; i++){
        switch ((*dataArrayP)[i].id){
        case RES_O_RSHIP_TYPE:
            lwm2m_data_encode_string(targetP->rship_type, *dataArrayP + i);
            break;
        case RES_O_RSHIP_MATE:
            lwm2m_data_encode_string(targetP->rship_mate, *dataArrayP + i);
            break;
        default:
            return COAP_404_NOT_FOUND;
        }
    }
    return COAP_205_CONTENT;
}

static uint8_t prv_discover(uint16_t instanceId, int * numDataP,
                            lwm2m_data_t ** dataArrayP, lwm2m_object_t * objectP){

    int i;
    if (*numDataP == 0){  // is the server asking for the full object ?
        *dataArrayP = lwm2m_data_new(2);
        if (*dataArrayP == NULL) return COAP_500_INTERNAL_SERVER_ERROR;
        *numDataP = 2;
        (*dataArrayP)[0].id = RES_O_RSHIP_TYPE;
        (*dataArrayP)[1].id = RES_O_RSHIP_MATE;
    }else{
        for (i = 0; i < *numDataP; i++){
            switch ((*dataArrayP)[i].id){
	        case RES_O_RSHIP_TYPE:
	            break;
	        case RES_O_RSHIP_MATE:
	            break;
            default:
                return COAP_404_NOT_FOUND;
            }
        }
    }
    return COAP_205_CONTENT;
}

static uint8_t prv_write(uint16_t instanceId, int numData,
                         lwm2m_data_t * dataArray, lwm2m_object_t * objectP){

    prv_instance_t * targetP;
    int i;
    uint8_t result;

    targetP = (prv_instance_t *)lwm2m_list_find(objectP->instanceList, instanceId);
    if (NULL == targetP) {
            return COAP_404_NOT_FOUND;
    }

    i=0;
    do{
        switch (dataArray[i].id) {
	        case RES_O_RSHIP_TYPE:
    			strncpy (targetP->rship_type,
	        	        (char*)dataArray[i].value.asBuffer.buffer, 
	        	        dataArray[i].value.asBuffer.length);
                result = COAP_204_CHANGED;
            	break;
            case RES_O_RSHIP_MATE:
    			strncpy (targetP->rship_mate,
	        	        (char*)dataArray[i].value.asBuffer.buffer, 
	        	        dataArray[i].value.asBuffer.length);
                result = COAP_204_CHANGED;
                break;
            default:
	            return COAP_404_NOT_FOUND;
        }
        i++;
    } while (i < numData && result == COAP_204_CHANGED);
            
    return result;
}

static uint8_t prv_delete(uint16_t id, lwm2m_object_t * objectP){
    prv_instance_t * targetP;
    objectP->instanceList = lwm2m_list_remove(objectP->instanceList, id, (lwm2m_list_t **)&targetP);
    if (NULL == targetP) return COAP_404_NOT_FOUND;
    lwm2m_free(targetP);
    return COAP_202_DELETED;
}

static uint8_t prv_create(uint16_t instanceId, int numData,
                          lwm2m_data_t * dataArray, lwm2m_object_t * objectP) {

    prv_instance_t * targetP;
    uint8_t result;

    targetP = (prv_instance_t *)lwm2m_malloc(sizeof(prv_instance_t));
    if (NULL == targetP) 
    	return COAP_500_INTERNAL_SERVER_ERROR;
    memset(targetP, 0, sizeof(prv_instance_t));

    targetP->shortID = instanceId;

    char empx[] = "";
    targetP->rship_type = (char*)malloc(sizeof(char)* (sizeof(empx)+1));
    strcpy(targetP->rship_type, empx);
    targetP->rship_mate = (char*)malloc(sizeof(char)* (sizeof(empx)+1));
    strcpy(targetP->rship_mate, empx);

    objectP->instanceList = LWM2M_LIST_ADD(objectP->instanceList, targetP);

    result = prv_write(instanceId, numData, dataArray, objectP);

    if (result != COAP_204_CHANGED) {
        (void)prv_delete(instanceId, objectP);
    } else{
        result = COAP_201_CREATED;
    }
    return result;
}

static uint8_t prv_exec(uint16_t instanceId, uint16_t resourceId,
                        uint8_t * buffer, int length,
                        lwm2m_object_t * objectP) {

    if (NULL == lwm2m_list_find(objectP->instanceList, instanceId)) return COAP_404_NOT_FOUND;

    /*
    switch (resourceId)
    {
    case 1:
        return COAP_405_METHOD_NOT_ALLOWED;
    case 2:
        fprintf(stdout, "\r\n-----------------\r\n"
                        "Execute on %hu/%d/%d\r\n"
                        " Parameter (%d bytes):\r\n",
                        objectP->objID, instanceId, resourceId, length);
        prv_output_buffer((uint8_t*)buffer, length);
        fprintf(stdout, "-----------------\r\n\r\n");
        return COAP_204_CHANGED;
    case 3:
        return COAP_405_METHOD_NOT_ALLOWED;
    default:
        return COAP_404_NOT_FOUND;
    }
    */
    return COAP_404_NOT_FOUND;
}

void display_atlas_entity_relationship_object(lwm2m_object_t * object) {
    fprintf(stdout, "  /%u: Atlas entity Relationship object, instances:\r\n", object->objID);
    prv_instance_t * instance = (prv_instance_t *)object->instanceList;
    while (instance != NULL) {
        fprintf(stdout, "%u, %u, %s, %s\r\n",
                object->objID, instance->shortID,
                instance->rship_type, instance->rship_mate);
        instance = (prv_instance_t *)instance->next;
    }
}

lwm2m_object_t * get_atlas_entity_relationship_object(void) {

    lwm2m_object_t * testObj;
    testObj = (lwm2m_object_t *)lwm2m_malloc(sizeof(lwm2m_object_t));

    if (NULL != testObj) {
        int i;
        prv_instance_t * targetP;

        memset(testObj, 0, sizeof(lwm2m_object_t));
        char typex[] = "drives";
        char matex[] = "temp-sens";
        testObj->objID = ATLAS_ENTITY_RELATIONSHIP_OBJECT_ID;

        for (i=0 ; i < 3 ; i++) {
            targetP = (prv_instance_t *)lwm2m_malloc(sizeof(prv_instance_t));
            if (NULL == targetP) return NULL;
            memset(targetP, 0, sizeof(prv_instance_t));

            targetP->shortID = 10 + i;

            targetP->rship_type = (char*)malloc(sizeof(char)* (sizeof(typex)+1));
            strcpy(targetP->rship_type, typex);

            targetP->rship_mate = (char*)malloc(sizeof(char)* (sizeof(matex)+1));
            strcpy(targetP->rship_mate, matex);

            testObj->instanceList = LWM2M_LIST_ADD(testObj->instanceList, targetP);
        }

        testObj->readFunc = prv_read;
        testObj->discoverFunc = prv_discover;
        testObj->writeFunc = prv_write;
        testObj->executeFunc = prv_exec;
        testObj->createFunc = prv_create;
        testObj->deleteFunc = prv_delete;
    }
    return testObj;
}

void free_atlas_entity_relationship_object(lwm2m_object_t * object) {
    LWM2M_LIST_FREE(object->instanceList);
    if (object->userData != NULL) {
        lwm2m_free(object->userData);
        object->userData = NULL;
    }
    lwm2m_free(object);
}

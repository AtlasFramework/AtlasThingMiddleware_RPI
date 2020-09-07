/*******************************************************************************
 *
 * Copyright (c) 2014 Bosch Software Innovations GmbH, Germany.
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse Public License v1.0
 * and Eclipse Distribution License v1.0 which accompany this distribution.
 *
 * The Eclipse Public License is available at
 *    http://www.eclipse.org/legal/epl-v10.html
 * The Eclipse Distribution License is available at
 *    http://www.eclipse.org/org/documents/edl-v10.php.
 *
 * Contributors:
 *    Bosch Software Innovations GmbH - Please refer to git log
 *
 *******************************************************************************/
/*
 * lwm2mclient.h
 *
 *  General functions of lwm2m test client.
 *
 *  Created on: 22.01.2015
 *  Author: Achim Kraus
 *  Copyright (c) 2015 Bosch Software Innovations GmbH, Germany. All rights reserved.
 */

#ifndef OBJECTHANDLER_H_
#define OBJECTHANDLER_H_

#include "liblwm2m.h"
extern int g_reboot;
extern char *intRes;


//Object Repository  ------------ these functions are to be exported/visible to the architecture.
extern 	void create_Default_Atlas_Thing_DesMetadata_Object();
void 	create_Atlas_Thing_DesMetadata_Object_par(const char* vend,const char* own,const char* name,const char* desc,const char* type);
void 	Update_Atlas_Thing_DesMetadata_Object(const char* vend,const char* own,const char* name,const char* desc,const char* type);


extern 	void create_Default_Atlas_Thing_AdminMetadata_Object();
void 	create_Atlas_Thing_AdminMetadata_Object_par(const char* vend,const char* own,const char* name,const char* desc,const char* type);
void 	Update_Atlas_Thing_AdminMetadata_Object(const char* vend,const char* own,const char* name,const char* desc,const char* type);


extern 	void create_Default_Atlas_Entity_DesMetadata_Object();
void 	create_Atlas_Entity_DesMetadata_Object_par(const char* vend,const char* own,const char* name,const char* desc,const char* type);
void 	Update_Atlas_Entity_DesMetadata_Object(const char* vend,const char* own,const char* name,const char* desc,const char* type);


extern 	void create_Default_Atlas_Attachment_DataRepository_Object();
void 	create_Atlas_Attachment_DataRepository_Object_par(const char* vend,const char* own,const char* name,const char* desc,const char* type,const char* typex);
void 	Update_Atlas_Attachment_DataRepository_Object(const char* vend,const char* own,const char* name,const char* desc,const char* type,const char* typex);


extern 	int connectServer(const char* thingID,const char* serverIP,const char* serverPort);
extern 	void delete_Atlas_Objects();
extern 	void display_Atlas_Objects();




//(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((
//(((((((((((((((((((((((((OMA standard Objects((((((((((((((((((((((((((((
//(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((

/*object_device.c */
lwm2m_object_t * 	get_object_device(void);
void 			free_object_device(lwm2m_object_t * objectP);
uint8_t 		device_change(lwm2m_data_t * dataArray, lwm2m_object_t * objectP);
void 			display_device_object(lwm2m_object_t * objectP);


/* object_firmware.c*/
lwm2m_object_t * 	get_object_firmware(void);
void 			free_object_firmware(lwm2m_object_t * objectP);
void 			display_firmware_object(lwm2m_object_t * objectP);


/* object_location.c */
lwm2m_object_t * get_object_location(void);
void free_object_location(lwm2m_object_t * object);
void display_location_object(lwm2m_object_t * objectP);


/* object_test.c */
#define TEST_OBJECT_ID 1024
lwm2m_object_t * get_test_object(void);
void free_test_object(lwm2m_object_t * object);
void display_test_object(lwm2m_object_t * objectP);


/* object_server.c */
lwm2m_object_t * get_server_object(int serverId, const char* binding, int lifetime, bool storing);
void clean_server_object(lwm2m_object_t * object);
void display_server_object(lwm2m_object_t * objectP);
void copy_server_object(lwm2m_object_t * objectDest, lwm2m_object_t * objectSrc);


/* object_connectivity_moni.c */
lwm2m_object_t * get_object_conn_m(void);
void free_object_conn_m(lwm2m_object_t * objectP);
uint8_t connectivity_moni_change(lwm2m_data_t * dataArray, lwm2m_object_t * objectP);

/* object_connectivity_stat.c */
extern lwm2m_object_t * get_object_conn_s(void);
void free_object_conn_s(lwm2m_object_t * objectP);
extern void conn_s_updateTxStatistic(lwm2m_object_t * objectP, uint16_t txDataByte, bool smsBased);
extern void conn_s_updateRxStatistic(lwm2m_object_t * objectP, uint16_t rxDataByte, bool smsBased);

/* object_access_control.c */
lwm2m_object_t* acc_ctrl_create_object(void);
void acl_ctrl_free_object(lwm2m_object_t * objectP);
bool  acc_ctrl_obj_add_inst (lwm2m_object_t* accCtrlObjP, uint16_t instId, uint16_t acObjectId, uint16_t acObjInstId, uint16_t acOwner);
bool  acc_ctrl_oi_add_ac_val(lwm2m_object_t* accCtrlObjP, uint16_t instId, uint16_t aclResId, uint16_t acValue);

/* lwm2mclient.c */
void handle_value_changed(lwm2m_context_t* lwm2mH, lwm2m_uri_t* uri, const char * value, size_t valueLength);

/* system_api.c */
void init_value_change(lwm2m_context_t * lwm2m);
void system_reboot(void);

/* object_security.c */
lwm2m_object_t * get_security_object(int serverId, const char* serverUri, char * bsPskId, char * psk, uint16_t pskLen, bool isBootstrap);
void clean_security_object(lwm2m_object_t * objectP);
char * get_server_uri(lwm2m_object_t * objectP, uint16_t secObjInstID);
void display_security_object(lwm2m_object_t * objectP);
void copy_security_object(lwm2m_object_t * objectDest, lwm2m_object_t * objectSrc);






//(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((
//(((((((((((((((((((((((((((((Atlas Objects(((((((((((((((((((((((((((((((
//(((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((((


/////////////////////////////////// AtlasThing_DescriptiveMetadata Object  ////////////////////////////


/* AtlasThing_DescriptiveMetadata.c  */
lwm2m_object_t * 	get_object_Atlas_Thing_Descriptive_Metadata(void);
void 			display_Atlas_Thing_Descriptive_Metadata_object(lwm2m_object_t * objectP);
uint8_t 		Atlas_Thing_Descriptive_Metadata_change(lwm2m_data_t * dataArray, lwm2m_object_t * objectP);
void 			free_object_Atlas_Thing_Descriptive_Metadata(lwm2m_object_t * objectP);

	/* Object Repository - these functions are to be exported/visible to the architecture */
extern 	void create_Default_Atlas_Thing_DesMetadata_Object();
void 	create_Atlas_Thing_DesMetadata_Object_par(const char* vend,const char* own,const char* name,const char* desc,const char* type);
void 	Update_Atlas_Thing_DesMetadata_Object(const char* vend,const char* own,const char* name,const char* desc,const char* type);






/* AtlasThing_AdministrativeMetadata.c */
lwm2m_object_t * 	get_object_Atlas_Thing_Administrative_Metadata(void);
void 			display_Atlas_Thing_Administrative_Metadata_object(lwm2m_object_t * objectP);
uint8_t 		Atlas_Thing_Administrative_Metadata_change(lwm2m_data_t * dataArray, lwm2m_object_t * objectP);
void 			free_object_Atlas_Thing_Administrative_Metadata(lwm2m_object_t * objectP);

	/*Object Repository  ------------ these functions are to be exported/visible to the architecture */
extern 	void create_Default_Atlas_Thing_AdminMetadata_Object();
void 	create_Atlas_Thing_AdminMetadata_Object_par(const char* vend,const char* own,const char* name,const char* desc,const char* type);
void 	Update_Atlas_Thing_AdminMetadata_Object(const char* vend,const char* own,const char* name,const char* desc,const char* type);









/*
* AtlasEntity_DescriptiveMetadata.c
*/
lwm2m_object_t * 	get_object_Atlas_Entity_Descriptive_Metadata(void);
void 			display_Atlas_Entity_Descriptive_Metadata_object(lwm2m_object_t * objectP);
uint8_t 		Atlas_Entity_Descriptive_Metadata_change(lwm2m_data_t * dataArray, lwm2m_object_t * objectP);
void 			free_object_Atlas_Entity_Descriptive_Metadata(lwm2m_object_t * objectP);


/*
* AtlasEntity_ResouceService
*/
lwm2m_object_t * get_atlas_entity_resourceservice_object(void);
void display_atlas_entity_resourceservice_object(lwm2m_object_t * objectP);
void free_atlas_entity_resourceservice_object(lwm2m_object_t * objectP);




/*
* AtlasEntity_Relationship.c
*/
lwm2m_object_t * get_atlas_entity_relationship_object(void);
void display_atlas_entity_relationship_object(lwm2m_object_t * objectP);
void free_atlas_entity_relationship_object(lwm2m_object_t * objectP);

/*
* AtlasAttachment_VendorOnlineDoc.c
*/
lwm2m_object_t * get_Atlas_Attachment_VendorOnlineDoc_object(void);
void display_Atlas_Attachment_VendorOnlineDoc_object(lwm2m_object_t * objectP);
void free_Atlas_Attachment_VendorOnlineDoc_object(lwm2m_object_t * objectP);


/*
* AtlasAttachment_DataRepository.c
*/
lwm2m_object_t * get_Atlas_Attachment_DataRepository_object(void);
void display_Atlas_Attachment_DataRepository_object(lwm2m_object_t * objectP);
void free_Atlas_Attachment_DataRepository_object(lwm2m_object_t * objectP);



#endif

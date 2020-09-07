# Provides WAKAAMA_SOURCES_DIR and WAKAAMA_SOURCES and WAKAAMA_DEFINITIONS variables.
# Add LWM2M_WITH_LOGS to compile definitions to enable logging.
# Set LWM2M_LITTLE_ENDIAN to FALSE or TRUE according to your destination platform or leave
# it unset to determine endianess automatically.

set(WAKAAMA_SOURCES_DIR ${CMAKE_CURRENT_LIST_DIR})

set(EXT_SOURCES 
    ${WAKAAMA_SOURCES_DIR}/er-coap-13/er-coap-13.c)

set(CORE_HEADERS
    ${WAKAAMA_SOURCES_DIR}/liblwm2m.h
    ${WAKAAMA_SOURCES_DIR}/objectHandler.h)

set(WAKAAMA_SOURCES
    ${WAKAAMA_SOURCES_DIR}/liblwm2m.c
    ${WAKAAMA_SOURCES_DIR}/objectHandler.c
    ${WAKAAMA_SOURCES_DIR}/uri.c
    ${WAKAAMA_SOURCES_DIR}/utils.c
    ${WAKAAMA_SOURCES_DIR}/objects.c
    ${WAKAAMA_SOURCES_DIR}/tlv.c
    ${WAKAAMA_SOURCES_DIR}/data.c
    ${WAKAAMA_SOURCES_DIR}/list.c
    ${WAKAAMA_SOURCES_DIR}/packet.c
    ${WAKAAMA_SOURCES_DIR}/transaction.c
    ${WAKAAMA_SOURCES_DIR}/registration.c
    ${WAKAAMA_SOURCES_DIR}/bootstrap.c
    ${WAKAAMA_SOURCES_DIR}/management.c
    ${WAKAAMA_SOURCES_DIR}/observe.c
    ${WAKAAMA_SOURCES_DIR}/json.c
    ${WAKAAMA_SOURCES_DIR}/discover.c
    ${WAKAAMA_SOURCES_DIR}/system_api.c
    ${WAKAAMA_SOURCES_DIR}/object_security.c
    ${WAKAAMA_SOURCES_DIR}/object_server.c
    ${WAKAAMA_SOURCES_DIR}/object_device.c
    ${WAKAAMA_SOURCES_DIR}/object_firmware.c
    ${WAKAAMA_SOURCES_DIR}/object_location.c
    ${WAKAAMA_SOURCES_DIR}/object_connectivity_moni.c
    ${WAKAAMA_SOURCES_DIR}/object_connectivity_stat.c
    ${WAKAAMA_SOURCES_DIR}/object_access_control.c
    ${WAKAAMA_SOURCES_DIR}/test_object.c
    ${WAKAAMA_SOURCES_DIR}/AtlasAttachment_DataRepository.c
    ${WAKAAMA_SOURCES_DIR}/AtlasAttachment_VendorOnlineDoc.c
    ${WAKAAMA_SOURCES_DIR}/AtlasEntity_DescriptiveMetadata.c
    ${WAKAAMA_SOURCES_DIR}/AtlasEntity_Relationship.c
    ${WAKAAMA_SOURCES_DIR}/AtlasEntity_ResourceService.c
    ${WAKAAMA_SOURCES_DIR}/AtlasThing_Attachment.c
    ${WAKAAMA_SOURCES_DIR}/AtlasThing_DescriptiveMetadata.c
    ${WAKAAMA_SOURCES_DIR}/AtlasThing_AdministrativeMetadata.c
    ${EXT_SOURCES})

# This will not work for multi project cmake generators like the Visual Studio Generator
if(CMAKE_BUILD_TYPE MATCHES Debug)
   set(WAKAAMA_DEFINITIONS ${WAKAAMA_DEFINITIONS} -DLWM2M_WITH_LOGS)
endif()

# Automatically determine endianess. This can be overwritten by setting LWM2M_LITTLE_ENDIAN
# accordingly in a cross compile toolchain file.
if(NOT DEFINED LWM2M_LITTLE_ENDIAN)
    include(TestBigEndian)
    TEST_BIG_ENDIAN(LWM2M_BIG_ENDIAN)
    if (LWM2M_BIG_ENDIAN)
         set(LWM2M_LITTLE_ENDIAN FALSE)
    else()
         set(LWM2M_LITTLE_ENDIAN TRUE)
    endif()
endif()
if (LWM2M_LITTLE_ENDIAN)
    set(WAKAAMA_DEFINITIONS ${WAKAAMA_DEFINITIONS} -DLWM2M_LITTLE_ENDIAN)
endif()


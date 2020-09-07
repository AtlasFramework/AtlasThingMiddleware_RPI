cmake_minimum_required (VERSION 3.0)

project (Atlas)

set(CMAKE_CXX_FLAGS "-w -pthread -Wno-write-strings -lcurl -std=c++14 -Wall -fPIC -I/usr/local/include/cppmicroservices3/ -I.")

option(DTLS "Enable DTLS" OFF)

include(${CMAKE_CURRENT_LIST_DIR}/../lib/OMALib/core/wakaama.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../lib/OMALib/shared/shared.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../Architecture/architect.cmake)
#include(${CMAKE_CURRENT_LIST_DIR}/../lib/CRYPTOPPLib/cryptopp.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../lib/PahoMQTT/PAHOMQTT.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../lib/CoAPLib/COAPLIB.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/../lib/tinyxml/tinyxml2.cmake)

add_definitions(-DLWM2M_CLIENT_MODE -DLWM2M_BOOTSTRAP -DLWM2M_SUPPORT_JSON)
add_definitions(${SHARED_DEFINITIONS} ${WAKAAMA_DEFINITIONS})

include_directories (${WAKAAMA_SOURCES_DIR} ${SHARED_INCLUDE_DIRS} ${TINYXML2_SOURCES_DIR} ${ARCHITECT_SOURCES_DIR} ${PAHOMQTT_SOURCES_DIR} ${COAPLIB_SOURCES_DIR})

include_directories (${CMAKE_CURRENT_LIST_DIR}/../lib/JSON/include)

SET(SOURCES
    AtlasMain.cpp
    )

add_executable(${PROJECT_NAME} ${SOURCES} ${WAKAAMA_SOURCES} ${SHARED_SOURCES} ${TINYXML2_SOURCES} ${ARCHITECT_SOURCES} ${PAHOMQTT_SOURCES} ${COAPLIB_SOURCES})

# Add WITH_LOGS to debug variant
target_link_libraries(${PROJECT_NAME} -Wl,-rpath,. -lCppMicroServicesd)
target_link_libraries(${PROJECT_NAME} -lcurl)
set_property(TARGET ${PROJECT_NAME} APPEND PROPERTY COMPILE_DEFINITIONS $<$<CONFIG:Debug>:WITH_LOGS>)

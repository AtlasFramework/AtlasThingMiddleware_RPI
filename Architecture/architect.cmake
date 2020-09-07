
set(ARCHITECT_SOURCES_DIR ${CMAKE_CURRENT_LIST_DIR})

set(ARCHITECT_HEADERS
    ${ARCHITECT_SOURCES_DIR}/API_Generator.h
    ${ARCHITECT_SOURCES_DIR}/System_Functions.h
    ${ARCHITECT_SOURCES_DIR}/Attachment_Engine.h
    ${ARCHITECT_SOURCES_DIR}/Communication_Manager.h
    ${ARCHITECT_SOURCES_DIR}/Identity_Parser.h
    ${ARCHITECT_SOURCES_DIR}/IoTDDL_Parser.h
    ${ARCHITECT_SOURCES_DIR}/Knowledge_Tweeting_Manager.h
    ${ARCHITECT_SOURCES_DIR}/Objects_Engine.h)

set(ARCHITECT_SOURCES
    ${ARCHITECT_SOURCES_DIR}/API_Generator.cpp
    ${ARCHITECT_SOURCES_DIR}/System_Functions.cpp
    ${ARCHITECT_SOURCES_DIR}/Attachment_Engine.cpp
    ${ARCHITECT_SOURCES_DIR}/Communication_Manager.cpp
    ${ARCHITECT_SOURCES_DIR}/Identity_Parser.cpp
    ${ARCHITECT_SOURCES_DIR}/IoTDDL_Parser.cpp
    ${ARCHITECT_SOURCES_DIR}/Knowledge_Tweeting_Manager.cpp
    ${ARCHITECT_SOURCES_DIR}/Objects_Engine.cpp
    ${EXT_SOURCES})



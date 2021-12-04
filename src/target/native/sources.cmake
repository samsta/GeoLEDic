set(THISDIR "${CMAKE_CURRENT_SOURCE_DIR}/src/target/native")
list(APPEND CMAKE_MODULE_PATH "${THISDIR}/cmake/Modules")

include_directories(.)

add_subdirectory(${THISDIR}/FastLED)
add_subdirectory(${THISDIR}/OpenGL)
add_subdirectory(${THISDIR}/${CMAKE_SYSTEM_NAME})

set(GENERATED_CONTROLLER_INFO ${CMAKE_BINARY_DIR}/generated/ControllerInfo.cpp)
add_custom_command(OUTPUT ${GENERATED_CONTROLLER_INFO}
                   COMMAND python ${CMAKE_SOURCE_DIR}/conf/makeControllerInfo.py ${GENERATED_CONTROLLER_INFO}
                   DEPENDS ${CMAKE_SOURCE_DIR}/conf/makeControllerInfo.py
                           ${CMAKE_SOURCE_DIR}/conf/programs.py)


set(TARGET_SOURCES ${TARGET_SOURCES}
                   ${THISDIR}/ImGui.cpp
                   ${THISDIR}/LaunchPad.cpp
                   ${THISDIR}/MidiMenu.cpp
                   ${THISDIR}/Piano.cpp
                   ${THISDIR}/Serial.cpp
                   ${GENERATED_CONTROLLER_INFO})

#set(TARGET_LIBRARIES ${TARGET_LIBRARIES} PARENT_SCOPE)

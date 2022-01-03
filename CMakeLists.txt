cmake_minimum_required (VERSION 3.0)
project(GeoLEDic VERSION 1.0.1)
cmake_policy(SET CMP0087 NEW) # use generator expression in install(CODE)
cmake_policy(SET CMP0072 NEW) # Use GLVND if available

set(VERSION_EXTRA "" CACHE STRING "Extra version information, e.g. commit hash")
set(FASTLED_DIR "${CMAKE_CURRENT_SOURCE_DIR}/FastLED" CACHE PATH "Path to FastLED installation")

add_definitions(-std=c++11
                -DWITH_GFX=1
                -Werror
                -Wall)

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
   add_definitions(-Wno-deprecated-register)
endif()

include_directories(.
                    src
                    src/target/native
                    src/target/native/FastLED
                    src/target/native/OpenGL
                    ${CMAKE_CURRENT_BINARY_DIR})
include_directories(SYSTEM "${FASTLED_DIR}/src")

include(src/target/native/sources.cmake)
add_subdirectory(src/programs)

# The core sources are the ones built for unit tests as well
set(GEOLEDIC_CORE_SOURCES src/Controls.cpp
                          src/CRGB_iterator.cpp
                          src/DecayingShapesFromNotes.cpp
                          src/Edge.cpp
                          src/MidiMessage.cpp
                          src/Notes.cpp
                          src/ShapesFromNotes.cpp
                          src/Triangle.cpp
                          src/Vector.cpp
                          src/Vertex.cpp)
add_library(geoledic-core STATIC ${GEOLEDIC_CORE_SOURCES})


set(GEOLEDIC_SOURCES ${TARGET_SOURCES}
                     src/Dome.cpp
                     src/GeoLEDic.cpp
                     src/main-pc.cpp)

source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${GEOLEDIC_SOURCES})

if (APPLE)
   set(MACOSX_BUNDLE_ICON_FILE Icon.icns)
   set(MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}${VERSION_EXTRA}")
   set(ICON ${CMAKE_CURRENT_SOURCE_DIR}/img/Icon.icns)
   set_source_files_properties(${ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
endif()

add_executable(GeoLEDic MACOSX_BUNDLE ${ICON} ${GEOLEDIC_SOURCES})

target_link_libraries(GeoLEDic gfx
                               programs
                               ${TARGET_LIBRARIES}
                               FastLED
                               geoledic-core)


add_custom_target(regenerate-dome COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/gen_dome.py ${CMAKE_CURRENT_SOURCE_DIR}/src
                                  SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/gen_dome.py)

find_package(GTest)

if(GTest_FOUND)
   message(STATUS "Building tests")
   enable_testing()
   list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")
   add_subdirectory(test)
else()
   message(STATUS "NOT building tests")
   return()
endif()


if(APPLE)
   set(APP "$<TARGET_FILE:GeoLEDic>")  # paths to executables
   set(DIRS "")   # directories to search for prerequisites
   install(CODE "
      include(BundleUtilities)
      fixup_bundle(\"${APP}\"   \"\"   \"${DIRS}\")
      " COMPONENT Runtime)
   install(TARGETS GeoLEDic
           BUNDLE DESTINATION "./" COMPONENT Runtime
           RUNTIME DESTINATION bin COMPONENT Runtime)
   set(CPACK_GENERATOR "DragNDrop")
   set(CPACK_PACKAGE_VERSION "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}${VERSION_EXTRA}")
   include(CPack)
endif()

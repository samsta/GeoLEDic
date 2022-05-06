execute_process(COMMAND python ${CMAKE_SOURCE_DIR}/conf/makeProgramBaseClasses.py ${CMAKE_BINARY_DIR}/generated --sources
                OUTPUT_VARIABLE GENERATED_BASE_CLASS_SOURCES)
execute_process(COMMAND mkdir -p ${CMAKE_BINARY_DIR}/generated)

foreach(source IN LISTS GENERATED_BASE_CLASS_SOURCES)
    string(REGEX REPLACE ".generated.cpp" ".hpp" header ${source})
    add_custom_command(OUTPUT ${source} ${header}
                       COMMAND python ${CMAKE_SOURCE_DIR}/conf/makeProgramBaseClasses.py ${source} 
                       DEPENDS ${CMAKE_SOURCE_DIR}/conf/makeProgramBaseClasses.py
                               ${CMAKE_SOURCE_DIR}/conf/programs.py)
endforeach()

set(GENERATED_FACTORY ${CMAKE_BINARY_DIR}/generated/ProgramFactory.cpp)
add_custom_command(OUTPUT ${GENERATED_FACTORY}
                   COMMAND python ${CMAKE_SOURCE_DIR}/conf/makeProgramFactory.py ${GENERATED_FACTORY}
                   DEPENDS ${CMAKE_SOURCE_DIR}/conf/makeProgramFactory.py
                           ${CMAKE_SOURCE_DIR}/conf/programs.py)

source_group(TREE "${CMAKE_BINARY_DIR}" FILES ${GENERATED_BASE_CLASS_SOURCES})

include_directories(${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/)
add_library(programs STATIC
            ${CMAKE_CURRENT_SOURCE_DIR}/SparklesAndTriangles.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/DefaultRainbow.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/Diagnostic.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/BetterEdgy.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/Edgy.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/Fire.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/FlexibleNoise.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/MovingRainbow.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/Lightning.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/Noise.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/OrganicLine.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/WarpDrive.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/WarpDriveMandala.cpp
            ${GENERATED_BASE_CLASS_SOURCES}
            ${GENERATED_FACTORY})

include_directories(
   ${GLFW_INCLUDE_DIR}
   ${GLM_INCLUDE_DIR}
   ../lib/FastLED
   gfx
   .
   )
include_directories(SYSTEM ${FASTLED_DIR}/src)

add_definitions(
   -std=c++11
   -DGLFW_INCLUDE_GLCOREARB=1
   -DGLFW_INCLUDE_GLEXT=1
   -Werror
   -Wall
   )

if("${CMAKE_SYSTEM_NAME}" STREQUAL "Darwin")
   add_definitions(
      # the following two are warnings about deprecations
      # on mac OS. Potentially some work required in the future...
      -DGL_SILENCE_DEPRECATION=1
      -Wno-deprecated-register
      )
endif()

add_subdirectory(gfx)

add_executable(manualtest_triangle test/manualtest_triangle.cpp)
# Disable annoying #pragma messages.
set_source_files_properties(test/manualtest_triangle.cpp PROPERTIES COMPILE_FLAGS " -DFASTLED_INTERNAL")
target_link_libraries(manualtest_triangle gfx FastLED)

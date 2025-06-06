project(msft_proxy_samples)

file(GLOB_RECURSE SOURCES "*.cpp")

foreach(SOURCE ${SOURCES})
  file(RELATIVE_PATH REL_PATH ${CMAKE_SOURCE_DIR} ${SOURCE})
  get_filename_component(DIR ${REL_PATH} DIRECTORY)
  string(REPLACE "/" "_" DIR_UNDERSCORE ${DIR})
  get_filename_component(EXECUTABLE_NAME ${SOURCE} NAME_WE)
  set(FULL_EXECUTABLE_NAME "${DIR_UNDERSCORE}_${EXECUTABLE_NAME}")
  add_executable(${FULL_EXECUTABLE_NAME} ${SOURCE})
  target_link_libraries(${FULL_EXECUTABLE_NAME} PRIVATE msft_proxy)
  if (MSVC)
    target_compile_options(${FULL_EXECUTABLE_NAME} PRIVATE /W4)
  elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${FULL_EXECUTABLE_NAME} PRIVATE -Wall -Wextra -Wpedantic -Wno-c++2b-extensions)
  else()
    target_compile_options(${FULL_EXECUTABLE_NAME} PRIVATE -Wall -Wextra -Wpedantic)
  endif()
endforeach()

project (SYN)
set (SYN_VERSION_MAJOR 1)
set (SYN_VERSION_MINOR 0)

set(CMAKE_CXX_FLAGS "-g -Wall")
set(CMAKE_CXX_FLAGS_DEBUG  "-O0 -g")

cmake_minimum_required (VERSION 2.8)

include_directories(${JAVA_HOME}/include ${JAVA_HOME}/include/linux ${SAFE_HOME}/include /usr/local/include /usr/include)
link_directories(${SYN_SOURCES} ${JAVA_HOME}/jre/lib/${OS_ARCH} ${JAVA_HOME}/jre/lib/${OS_ARCH}/client ${JAVA_HOME}/jre/lib/${OS_ARCH}/server ${SAFE_HOME}/lib/c++/LINUX /usr/lib /usr/local/lib)

file(GLOB COMMON_SOURCES src/main/cpp/common/*.cpp)
file(GLOB SYN_SOURCES src/main/cpp/syn/*.cpp)
file(GLOB SYN_TEST_SOURCES src/test/cpp/syn/*.cpp)

add_executable(syn ${SYN_SOURCES} ${COMMON_SOURCES})
add_executable(syn_test ${SYN_TEST_SOURCES} ${COMMON_SOURCES})

target_link_libraries(syn netcdf netcdf_c++ Safe Xfdu xerces-c xalan-c)
target_link_libraries(syn_test netcdf netcdf_c++ Safe Xfdu cppunit xerces-c xalan-c)

set_property(TARGET syn PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
set_property(TARGET syn_test PROPERTY RUNTIME_OUTPUT_DIRECTORY target)

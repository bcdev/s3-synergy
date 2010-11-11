project (SYN)
set (SYN_VERSION_MAJOR 1)
set (SYN_VERSION_MINOR 0)

cmake_minimum_required (VERSION 2.8)

include_directories(${JAVA_HOME}/include ${JAVA_HOME}/include/linux ${SAFE_HOME}/include /usr/local/include /usr/include)
link_directories(${SYN_SOURCES} ${JAVA_HOME}/jre/lib/${OS_ARCH} ${JAVA_HOME}/jre/lib/${OS_ARCH}/client ${JAVA_HOME}/jre/lib/${OS_ARCH}/server ${SAFE_HOME}/lib/c++/LINUX /usr/lib /usr/local/lib)

file(GLOB COMMON_SOURCES src/main/c++/common/*.cpp)
file(GLOB SYN_SOURCES src/main/c++/syn/*.cpp)
file(GLOB SYN_TEST_SOURCES src/test/c++/syn/*.cpp)

add_executable(syn ${SYN_SOURCES} ${COMMON_SOURCES})
add_executable(syn_test ${SYN_TEST_SOURCES} ${COMMON_SOURCES})

target_link_libraries(syn netcdf netcdf_c++ Safe xerces-c-3.1)
target_link_libraries(syn_test cppunit xerces-c-3.1)

set_property(TARGET syn PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
set_property(TARGET syn_test PROPERTY RUNTIME_OUTPUT_DIRECTORY target)

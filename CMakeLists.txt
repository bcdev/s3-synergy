project (s3-synergy)
set (SYN_VERSION_MAJOR 1)
set (SYN_VERSION_MINOR 0)

set(CMAKE_CXX_FLAGS "-O3 -Wall -DBOOST_FILESYSTEM_VERSION=2")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -Wall -DBOOST_FILESYSTEM_VERSION=2")

cmake_minimum_required (VERSION 2.8)

##
# Project source files
#
file(GLOB_RECURSE CORE_MAIN_SOURCES src/main/cpp/core/*.cpp)
file(GLOB_RECURSE CORE_TEST_SOURCES src/test/cpp/core/*.cpp)
file(GLOB_RECURSE MODULES_MAIN_SOURCES src/main/cpp/modules/*.cpp)
file(GLOB_RECURSE MODULES_TEST_SOURCES src/test/cpp/modules/*.cpp)
file(GLOB_RECURSE UTIL_MAIN_SOURCES src/main/cpp/util/*.cpp)
file(GLOB_RECURSE UTIL_TEST_SOURCES src/test/cpp/util/*.cpp)
file(GLOB_RECURSE SYN_MAIN_SOURCES src/main/cpp/syn/*.cpp)
file(GLOB_RECURSE SYN_TEST_SOURCES src/test/cpp/syn/*.cpp)

##
# Sytem include directories
#
include_directories(
	${JAVA_HOME}/${JAVA_INC_DIR}
	${SAFE_HOME}/${SAFE_INC_DIR}
)

include_directories(SYSTEM /usr/local/include /usr/include)

##
# System link directories
#
link_directories(
	${JAVA_HOME}/${JAVA_LIB_DIR}
	${JAVA_HOME}/${JAVA_JVM_DIR}
	${SAFE_HOME}/${SAFE_LIB_DIR}
)

link_directories(/usr/lib /usr/local/lib)

##
# Project test targets
#
add_executable(sy_unt_gen
    src/test/cpp/SyUntGenRunner.cpp
	${CORE_TEST_SOURCES}
	${CORE_MAIN_SOURCES}
	${UTIL_TEST_SOURCES}
	${UTIL_MAIN_SOURCES}
)
target_link_libraries(sy_unt_gen
	cppunit
	netcdf
	Xfdu
	xerces-c
	xqilla
	boost_system-mt
	boost_filesystem-mt
)
set_property(TARGET sy_unt_gen PROPERTY RUNTIME_OUTPUT_DIRECTORY target)


##
# Project main targets
#
add_executable(syn
	${SYN_MAIN_SOURCES}
	${MODULES_MAIN_SOURCES}
	${CORE_MAIN_SOURCES}
	${UTIL_MAIN_SOURCES}
)
target_link_libraries(syn
	netcdf
	Safe
	Xfdu
	xerces-c
	xqilla
	boost_system-mt
	boost_filesystem-mt
)
set_property(TARGET syn PROPERTY RUNTIME_OUTPUT_DIRECTORY target)

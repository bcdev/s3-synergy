project (s3-synergy)
set (SYN_VERSION_MAJOR 1)
set (SYN_VERSION_MINOR 0)

set(CMAKE_CXX_FLAGS "-O3 -Wall -DBOOST_FILESYSTEM_VERSION=2")
set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -Wall -DBOOST_FILESYSTEM_VERSION=2")

cmake_minimum_required (VERSION 2.8)

##
# SYN source files
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
# SYN link directories
#
#link_directories(
#	${CORE_MAIN_SOURCES} 
#	${CORE_TEST_SOURCES}
#	${MODULES_MAIN_SOURCES}
#	${MODULES_TEST_SOURCES}
#	${UTIL_MAIN_SOURCES}
#	${SYN_MAIN_SOURCES}
#)

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
# SYN test targets
#
add_executable(core_test
	${CORE_TEST_SOURCES}
	${CORE_MAIN_SOURCES}
	${UTIL_MAIN_SOURCES}
)
target_link_libraries(core_test 
	cppunit 
	netcdf 
	Xfdu 
	xerces-c 
	xqilla 
	boost_system-mt 
	boost_filesystem-mt
)
set_property(TARGET core_test PROPERTY RUNTIME_OUTPUT_DIRECTORY target)


add_executable(util_test
	${UTIL_MAIN_SOURCES}
	${UTIL_TEST_SOURCES}
	${CORE_MAIN_SOURCES}
)
target_link_libraries(util_test
	cppunit
	netcdf
	Xfdu
	xerces-c
	xqilla
	boost_system-mt
	boost_filesystem-mt
)
set_property(TARGET util_test PROPERTY RUNTIME_OUTPUT_DIRECTORY target)


#add_executable(modules_test 
#	${MODULES_MAIN_SOURCES}
#	${MODULES_TEST_SOURCES}
#	${UTIL_MAIN_SOURCES}
#	${UTIL_TEST_SOURCES}
#	${CORE_MAIN_SOURCES}
#)
#target_link_libraries(modules_test 
#	cppunit 
#	netcdf
#	Xfdu
#	xerces-c
#	xqilla
#	boost_system-mt
#	boost_filesystem-mt
#)
#set_property(TARGET modules_test PROPERTY RUNTIME_OUTPUT_DIRECTORY target)


#add_executable(syn_test
#	${SYN_TEST_SOURCES}
#	${UTIL_MAIN_SOURCES}
#)
#target_link_libraries(syn_test
#	netcdf
#	Safe
#	Xfdu
#	cppunit
#	xerces-c
#	xqilla
#	)
#set_property(TARGET syn_test PROPERTY RUNTIME_OUTPUT_DIRECTORY target)


##
# SYN main targets
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

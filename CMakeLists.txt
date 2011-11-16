project (s3-synergy)
set (s3-synergy_VERSION_MAJOR 1)
set (s3-synergy_VERSION_MINOR 0)

configure_file (
  "src/main/cpp/core/Config.h.in"
  "src/main/cpp/core/Config.h"
)
  
set(CMAKE_CXX_FLAGS "-fopenmp -O3 -std=c++98 -DBOOST_FILESYSTEM_VERSION=2")
set(CMAKE_CXX_FLAGS_DEBUG "-fopenmp -g -O0 -std=c++98 -Wall -DBOOST_FILESYSTEM_VERSION=2")

cmake_minimum_required (VERSION 2.8)

##
# Project source files
#
file(GLOB_RECURSE CORE_MAIN_SOURCES src/main/cpp/core/*.cpp)
file(GLOB_RECURSE CORE_TEST_SOURCES src/test/cpp/core/*.cpp)
file(GLOB_RECURSE READER_MAIN_SOURCES src/main/cpp/reader/*.cpp)
file(GLOB_RECURSE READER_TEST_SOURCES src/test/cpp/reader/*.cpp)
file(GLOB_RECURSE MODULES_MAIN_SOURCES src/main/cpp/modules/*.cpp)
file(GLOB_RECURSE ACO_TEST_SOURCES src/test/cpp/modules/AcoTest.cpp)
file(GLOB_RECURSE AEI_TEST_SOURCES src/test/cpp/modules/AeiTest.cpp)
file(GLOB_RECURSE AER_TEST_SOURCES src/test/cpp/modules/AerTest.cpp)
file(GLOB_RECURSE AVE_TEST_SOURCES src/test/cpp/modules/AveTest.cpp)
file(GLOB_RECURSE COL_TEST_SOURCES src/test/cpp/modules/ColTest.cpp)
file(GLOB_RECURSE PCL_TEST_SOURCES src/test/cpp/modules/PclTest.cpp)
file(GLOB_RECURSE WRITER_MAIN_SOURCES src/main/cpp/writer/*.cpp)
file(GLOB_RECURSE WRITER_TEST_SOURCES src/test/cpp/writer/*.cpp)
file(GLOB_RECURSE UTIL_MAIN_SOURCES src/main/cpp/util/*.cpp)
file(GLOB_RECURSE UTIL_TEST_SOURCES src/test/cpp/util/*.cpp)

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
	boost_regex-mt
	boost_system-mt
	boost_filesystem-mt
)
set_property(TARGET sy_unt_gen PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(sy_unt_sre
    src/test/cpp/SyUntSreRunner.cpp
	${CORE_MAIN_SOURCES}
	${UTIL_MAIN_SOURCES}
	${READER_MAIN_SOURCES}
	${WRITER_MAIN_SOURCES}
	${READER_TEST_SOURCES}
)
target_link_libraries(sy_unt_sre
	cppunit
	netcdf
	Xfdu
	xerces-c
	xqilla
	boost_regex-mt
	boost_system-mt
	boost_filesystem-mt
)
set_property(TARGET sy_unt_sre PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(sy_unt_swr
    src/test/cpp/SyUntSwrRunner.cpp
	${CORE_MAIN_SOURCES}
	${UTIL_MAIN_SOURCES}
	${WRITER_MAIN_SOURCES}
	${WRITER_TEST_SOURCES}
)
target_link_libraries(sy_unt_swr
	cppunit
	netcdf
	Xfdu
	xerces-c
	xqilla
	boost_regex-mt
	boost_system-mt
	boost_filesystem-mt
)
set_property(TARGET sy_unt_swr PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(sy_unt_col
    src/test/cpp/SyUntColRunner.cpp
	${CORE_MAIN_SOURCES}
	${UTIL_MAIN_SOURCES}
	${READER_MAIN_SOURCES}
	${WRITER_MAIN_SOURCES}
	${MODULES_MAIN_SOURCES}
	${COL_TEST_SOURCES}
)
target_link_libraries(sy_unt_col
	cppunit
	netcdf
	Xfdu
	xerces-c
	xqilla
	boost_regex-mt
	boost_system-mt
	boost_filesystem-mt
)
set_property(TARGET sy_unt_col PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(sy_unt_pcl
    src/test/cpp/SyUntPclRunner.cpp
	${CORE_MAIN_SOURCES}
	${UTIL_MAIN_SOURCES}
	${READER_MAIN_SOURCES}
	${WRITER_MAIN_SOURCES}
	${MODULES_MAIN_SOURCES}
	${PCL_TEST_SOURCES}
)
target_link_libraries(sy_unt_pcl
	cppunit
	netcdf
	Xfdu
	xerces-c
	xqilla
	boost_regex-mt
	boost_system-mt
	boost_filesystem-mt
)
set_property(TARGET sy_unt_pcl PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(sy_unt_aco
    src/test/cpp/SyUntAcoRunner.cpp
	${CORE_MAIN_SOURCES}
	${UTIL_MAIN_SOURCES}
	${READER_MAIN_SOURCES}
	${WRITER_MAIN_SOURCES}
	${MODULES_MAIN_SOURCES}
	${ACO_TEST_SOURCES}
)
target_link_libraries(sy_unt_aco
	cppunit
	netcdf
	Xfdu
	xerces-c
	xqilla
	boost_regex-mt
	boost_system-mt
	boost_filesystem-mt
)
set_property(TARGET sy_unt_aco PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(sy_unt_aei
    src/test/cpp/SyUntAeiRunner.cpp
	${CORE_MAIN_SOURCES}
	${UTIL_MAIN_SOURCES}
	${READER_MAIN_SOURCES}
	${WRITER_MAIN_SOURCES}
	${MODULES_MAIN_SOURCES}
	${AEI_TEST_SOURCES}
)
target_link_libraries(sy_unt_aei
	cppunit
	netcdf
	Xfdu
	xerces-c
	xqilla
	boost_regex-mt
	boost_system-mt
	boost_filesystem-mt
)
set_property(TARGET sy_unt_aei PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(sy_unt_aer
    src/test/cpp/SyUntAerRunner.cpp
	${CORE_MAIN_SOURCES}
	${UTIL_MAIN_SOURCES}
	${READER_MAIN_SOURCES}
	${WRITER_MAIN_SOURCES}
	${MODULES_MAIN_SOURCES}
	${AER_TEST_SOURCES}
)
target_link_libraries(sy_unt_aer
	cppunit
	netcdf
	Xfdu
	xerces-c
	xqilla
	boost_regex-mt
	boost_system-mt
	boost_filesystem-mt
)
set_property(TARGET sy_unt_aer PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(sy_unt_ave
    src/test/cpp/SyUntAveRunner.cpp
	${CORE_MAIN_SOURCES}
	${UTIL_MAIN_SOURCES}
	${READER_MAIN_SOURCES}
	${WRITER_MAIN_SOURCES}
	${MODULES_MAIN_SOURCES}
	${AVE_TEST_SOURCES}
)
target_link_libraries(sy_unt_ave
	cppunit
	netcdf
	Xfdu
	xerces-c
	xqilla
	boost_regex-mt
	boost_system-mt
	boost_filesystem-mt
)
set_property(TARGET sy_unt_ave PROPERTY RUNTIME_OUTPUT_DIRECTORY target)

##
# Project main targets
#
#add_executable(syl2
#	src/main/cpp/tasks/*.cpp
#	${READER_MAIN_SOURCES}
#	${MODULES_MAIN_SOURCES}
#	${WRITER_MAIN_SOURCES}
#	${CORE_MAIN_SOURCES}
#	${UTIL_MAIN_SOURCES}
#)
#target_link_libraries(syl2
#	netcdf
#	Safe
#	Xfdu
#	xerces-c
#	xqilla
#	boost_system-mt
#	boost_filesystem-mt
#)
#set_property(TARGET syl2 PROPERTY RUNTIME_OUTPUT_DIRECTORY target)

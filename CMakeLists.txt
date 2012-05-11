#
# Copyright (C) 2012  Brockmann Consult GmbH (info@brockmann-consult.de)
#
# This program is free software; you can redistribute it and/or modify it
# under the terms of the GNU General Public License as published by the
# Free Software Foundation. This program is distributed in the hope it will
# be useful, but WITHOUT ANY WARRANTY; without even the implied warranty
# of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA
#

project (s3-synergy)
set(s3-synergy_VERSION_MAJOR 1)
set(s3-synergy_VERSION_MINOR 0)

cmake_minimum_required (VERSION 2.6)

configure_file (
  "src/main/cpp/core/Config.h.in"
  "src/main/cpp/core/Config.h"
)

configure_file (
  "src/main/bin/syl2.sh.in"
  "src/main/bin/syl2.sh"
)

configure_file (
  "src/main/bin/vgtp.sh.in"
  "src/main/bin/vgtp.sh"
)

configure_file (
  "src/main/bin/vgts.sh.in"
  "src/main/bin/vgts.sh"
)

configure_file (
  "src/test/bin/runalltests.sh.in"
  "src/test/bin/runalltests.sh"
)

configure_file (
  "src/test/bin/memcheck.sh.in"
  "src/test/bin/memcheck.sh"
)

configure_file (
  "src/test/bin/profiler.sh.in"
  "src/test/bin/profiler.sh"
)

set(CMAKE_CXX_FLAGS "-fopenmp -O3 -march=core2 -std=c++98")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O  -march=core2 -std=c++98 -Wall")

##
# Project source files
#
file(GLOB_RECURSE CORE_MAIN_SOURCES src/main/cpp/core/*.cpp)
file(GLOB_RECURSE CORE_TEST_SOURCES src/test/cpp/core/*.cpp)
file(GLOB_RECURSE READER_MAIN_SOURCES src/main/cpp/reader/*.cpp)
file(GLOB_RECURSE READER_TEST_SOURCES src/test/cpp/reader/*.cpp)
file(GLOB_RECURSE MODULES_MAIN_SOURCES src/main/cpp/modules/*.cpp)
file(GLOB_RECURSE WRITER_MAIN_SOURCES src/main/cpp/writer/*.cpp)
file(GLOB_RECURSE UTIL_MAIN_SOURCES src/main/cpp/util/*.cpp)
file(GLOB_RECURSE UTIL_TEST_SOURCES src/test/cpp/util/*.cpp)

file(GLOB_RECURSE ACO_TEST_SOURCES src/test/cpp/modules/AcoTest.cpp)
file(GLOB_RECURSE AEI_TEST_SOURCES src/test/cpp/modules/AeiTest.cpp)
file(GLOB_RECURSE AER_TEST_SOURCES src/test/cpp/modules/AerTest.cpp)
file(GLOB_RECURSE AVE_TEST_SOURCES src/test/cpp/modules/AveTest.cpp)
file(GLOB_RECURSE COL_TEST_SOURCES src/test/cpp/modules/ColTest.cpp)
file(GLOB_RECURSE PCL_TEST_SOURCES src/test/cpp/modules/PclTest.cpp)
file(GLOB_RECURSE VBM_TEST_SOURCES src/test/cpp/modules/VbmTest.cpp)
file(GLOB_RECURSE VFL_TEST_SOURCES src/test/cpp/modules/VflTest.cpp)
file(GLOB_RECURSE VPR_TEST_SOURCES src/test/cpp/modules/VprTest.cpp)
file(GLOB_RECURSE VAC_TEST_SOURCES src/test/cpp/modules/VacTest.cpp)
file(GLOB_RECURSE VCO_TEST_SOURCES src/test/cpp/modules/VcoTest.cpp)
file(GLOB_RECURSE SYL2_1_INTEGRATION_TEST_SOURCES src/test/cpp/integration/Syl2_1_Test.cpp)
file(GLOB_RECURSE SYL2_2_INTEGRATION_TEST_SOURCES src/test/cpp/integration/Syl2_2_Test.cpp)
file(GLOB_RECURSE VGTP_1_INTEGRATION_TEST_SOURCES src/test/cpp/integration/VgtP_1_Test.cpp)
file(GLOB_RECURSE VGTP_2_INTEGRATION_TEST_SOURCES src/test/cpp/integration/VgtP_2_Test.cpp)
file(GLOB_RECURSE VGTS_1_INTEGRATION_TEST_SOURCES src/test/cpp/integration/VgtS_1_Test.cpp)
file(GLOB_RECURSE VGTS_2_INTEGRATION_TEST_SOURCES src/test/cpp/integration/VgtS_2_Test.cpp)

##
# Project libraries
#
find_library(BOOST_REGEX
    NAMES boost_regex boost_regex-mt 
    PATHS ~/usr/lib /usr/local/lib /usr/lib 
    NO_DEFAULT_PATH
    DOC "The path to the Boost Regular Expression shared library."
)
find_library(BOOST_SYSTEM
    NAMES boost_system boost_system-mt
    PATHS ~/usr/lib /usr/local/lib /usr/lib 
    NO_DEFAULT_PATH
    DOC "The path to the Boost System shared library."
)
find_library(BOOST_FILESYSTEM
    NAMES boost_filesystem boost_filesystem-mt
    PATHS ~/usr/lib /usr/local/lib /usr/lib 
    NO_DEFAULT_PATH
    DOC "The path to the Boost Filesystem shared library."
)
find_library(CPPUNIT
    NAMES cppunit
    PATHS ~/usr/lib /usr/local/lib /usr/lib 
    DOC "The path to the CppUnit shared library."
)
find_library(NETCDF
    NAMES netcdf
    PATHS ~/usr/lib /usr/local/lib /usr/lib 
    NO_DEFAULT_PATH
    DOC "The path to the netCDF shared library."
)
find_library(XERCES
    NAMES xerces-c
    PATHS ~/usr/lib /usr/local/lib /usr/lib 
    NO_DEFAULT_PATH
    DOC "The path to the Xerces-c shared library."
)
find_library(XQILLA
    NAMES xqilla
    PATHS ~/usr/lib /usr/local/lib /usr/lib 
    NO_DEFAULT_PATH
    DOC "The path to the XQilla shared library."
)
find_program(MD5_SUM_EXECUTABLE NAMES md5sum gmd5sum)

##
# Project include directories
#
include_directories(before
    ~/usr/include
)
#include_directories(before
#    ${JAVA_HOME}/${JAVA_INC_DIR}
#    ${SAFE_HOME}/${SAFE_INC_DIR}
#)

##
# Project link directories
#
#link_directories(
#    ${JAVA_HOME}/${JAVA_LIB_DIR}
#    ${JAVA_HOME}/${JAVA_JVM_DIR}
#    ${SAFE_HOME}/${SAFE_LIB_DIR}
#)

##
# Project test targets
#
add_executable(sy_unt_gen src/test/cpp/TestRunner.cpp
	${CORE_TEST_SOURCES}
	${CORE_MAIN_SOURCES}
	${UTIL_TEST_SOURCES}
	${UTIL_MAIN_SOURCES}
	src/main/cpp/writer/SegmentWriter.cpp src/test/cpp/writer/SegmentWriterTest.cpp src/test/cpp/writer/MapSegmentProvider.cpp
)
target_link_libraries(sy_unt_gen
    ${BOOST_FILESYSTEM}
    ${BOOST_REGEX}
    ${BOOST_SYSTEM}
	${CPPUNIT}
	${NETCDF}
	${XERCES}
	${XQILLA}
)
set_property(TARGET sy_unt_gen PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(sy_unt_sre src/test/cpp/TestRunner.cpp
	${CORE_MAIN_SOURCES}
	${UTIL_MAIN_SOURCES}
	${READER_MAIN_SOURCES}
	${WRITER_MAIN_SOURCES}
	${READER_TEST_SOURCES}
)
target_link_libraries(sy_unt_sre
    ${BOOST_FILESYSTEM}
    ${BOOST_REGEX}
    ${BOOST_SYSTEM}
    ${CPPUNIT}
    ${NETCDF}
    ${XERCES}
    ${XQILLA}
)
set_property(TARGET sy_unt_sre PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(sy_unt_swr src/test/cpp/TestRunner.cpp
	${CORE_MAIN_SOURCES}
	${UTIL_MAIN_SOURCES}
	${WRITER_MAIN_SOURCES}
    src/test/cpp/writer/SynL2WriterTest.cpp  src/test/cpp/writer/SynL2SegmentProvider.cpp
)
target_link_libraries(sy_unt_swr
    ${BOOST_FILESYSTEM}
    ${BOOST_REGEX}
    ${BOOST_SYSTEM}
    ${CPPUNIT}
    ${NETCDF}
    ${XERCES}
    ${XQILLA}
)
set_property(TARGET sy_unt_swr PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(sy_unt_col src/test/cpp/TestRunner.cpp
	${CORE_MAIN_SOURCES}
	${UTIL_MAIN_SOURCES}
	${READER_MAIN_SOURCES}
	${WRITER_MAIN_SOURCES}
	${MODULES_MAIN_SOURCES}
	${COL_TEST_SOURCES}
)
target_link_libraries(sy_unt_col
    ${BOOST_FILESYSTEM}
    ${BOOST_REGEX}
    ${BOOST_SYSTEM}
    ${CPPUNIT}
    ${NETCDF}
    ${XERCES}
    ${XQILLA}
)
set_property(TARGET sy_unt_col PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(sy_unt_pcl src/test/cpp/TestRunner.cpp
	${CORE_MAIN_SOURCES}
	${UTIL_MAIN_SOURCES}
	${READER_MAIN_SOURCES}
	${WRITER_MAIN_SOURCES}
	${MODULES_MAIN_SOURCES}
	${PCL_TEST_SOURCES}
)
target_link_libraries(sy_unt_pcl
    ${BOOST_FILESYSTEM}
    ${BOOST_REGEX}
    ${BOOST_SYSTEM}
    ${CPPUNIT}
    ${NETCDF}
    ${XERCES}
    ${XQILLA}
)
set_property(TARGET sy_unt_pcl PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(sy_unt_ave src/test/cpp/TestRunner.cpp
    ${CORE_MAIN_SOURCES}
    ${UTIL_MAIN_SOURCES}
    ${READER_MAIN_SOURCES}
    ${WRITER_MAIN_SOURCES}
    ${MODULES_MAIN_SOURCES}
    ${AVE_TEST_SOURCES}
)
target_link_libraries(sy_unt_ave
    ${BOOST_FILESYSTEM}
    ${BOOST_REGEX}
    ${BOOST_SYSTEM}
    ${CPPUNIT}
    ${NETCDF}
    ${XERCES}
    ${XQILLA}
)
set_property(TARGET sy_unt_ave PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(sy_unt_aer src/test/cpp/TestRunner.cpp
	${CORE_MAIN_SOURCES}
	${UTIL_MAIN_SOURCES}
	${READER_MAIN_SOURCES}
	${WRITER_MAIN_SOURCES}
	${MODULES_MAIN_SOURCES}
	${AER_TEST_SOURCES}
)
target_link_libraries(sy_unt_aer
    ${BOOST_FILESYSTEM}
    ${BOOST_REGEX}
    ${BOOST_SYSTEM}
    ${CPPUNIT}
    ${NETCDF}
    ${XERCES}
    ${XQILLA}
)
set_property(TARGET sy_unt_aer PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(sy_unt_aei src/test/cpp/TestRunner.cpp
    ${CORE_MAIN_SOURCES}
    ${UTIL_MAIN_SOURCES}
    ${READER_MAIN_SOURCES}
    ${WRITER_MAIN_SOURCES}
    ${MODULES_MAIN_SOURCES}
    ${AEI_TEST_SOURCES}
)
target_link_libraries(sy_unt_aei
    ${BOOST_FILESYSTEM}
    ${BOOST_REGEX}
    ${BOOST_SYSTEM}
    ${CPPUNIT}
    ${NETCDF}
    ${XERCES}
    ${XQILLA}
)
set_property(TARGET sy_unt_aei PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(sy_unt_aco src/test/cpp/TestRunner.cpp
    ${CORE_MAIN_SOURCES}
    ${UTIL_MAIN_SOURCES}
    ${READER_MAIN_SOURCES}
    ${WRITER_MAIN_SOURCES}
    ${MODULES_MAIN_SOURCES}
    ${ACO_TEST_SOURCES}
)
target_link_libraries(sy_unt_aco
    ${BOOST_FILESYSTEM}
    ${BOOST_REGEX}
    ${BOOST_SYSTEM}
    ${CPPUNIT}
    ${NETCDF}
    ${XERCES}
    ${XQILLA}
)
set_property(TARGET sy_unt_aco PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(sy_unt_vbm src/test/cpp/TestRunner.cpp
    ${CORE_MAIN_SOURCES}
    ${UTIL_MAIN_SOURCES}
    ${READER_MAIN_SOURCES}
    ${WRITER_MAIN_SOURCES}
    ${MODULES_MAIN_SOURCES}
    ${VBM_TEST_SOURCES}
)
target_link_libraries(sy_unt_vbm
    ${BOOST_FILESYSTEM}
    ${BOOST_REGEX}
    ${BOOST_SYSTEM}
    ${CPPUNIT}
    ${NETCDF}
    ${XERCES}
    ${XQILLA}
)
set_property(TARGET sy_unt_vbm PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(sy_unt_vfl src/test/cpp/TestRunner.cpp
    ${CORE_MAIN_SOURCES}
    ${UTIL_MAIN_SOURCES}
    ${READER_MAIN_SOURCES}
    ${WRITER_MAIN_SOURCES}
    ${MODULES_MAIN_SOURCES}
    ${VFL_TEST_SOURCES}
)
target_link_libraries(sy_unt_vfl
    ${BOOST_FILESYSTEM}
    ${BOOST_REGEX}
    ${BOOST_SYSTEM}
    ${CPPUNIT}
    ${NETCDF}
    ${XERCES}
    ${XQILLA}
)
set_property(TARGET sy_unt_vfl PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(sy_unt_vpr src/test/cpp/TestRunner.cpp
    ${CORE_MAIN_SOURCES}
    ${UTIL_MAIN_SOURCES}
    ${READER_MAIN_SOURCES}
    ${WRITER_MAIN_SOURCES}
    ${MODULES_MAIN_SOURCES}
    ${VPR_TEST_SOURCES}
)
target_link_libraries(sy_unt_vpr
    ${BOOST_FILESYSTEM}
    ${BOOST_REGEX}
    ${BOOST_SYSTEM}
    ${CPPUNIT}
    ${NETCDF}
    ${XERCES}
    ${XQILLA}
)
set_property(TARGET sy_unt_vpr PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(sy_unt_vac src/test/cpp/TestRunner.cpp
    ${CORE_MAIN_SOURCES}
    ${UTIL_MAIN_SOURCES}
    ${READER_MAIN_SOURCES}
    ${WRITER_MAIN_SOURCES}
    ${MODULES_MAIN_SOURCES}
    ${VAC_TEST_SOURCES}
)
target_link_libraries(sy_unt_vac
    ${BOOST_FILESYSTEM}
    ${BOOST_REGEX}
    ${BOOST_SYSTEM}
    ${CPPUNIT}
    ${NETCDF}
    ${XERCES}
    ${XQILLA}
)
set_property(TARGET sy_unt_vac PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(sy_unt_vco src/test/cpp/TestRunner.cpp
    ${CORE_MAIN_SOURCES}
    ${UTIL_MAIN_SOURCES}
    ${READER_MAIN_SOURCES}
    ${WRITER_MAIN_SOURCES}
    ${MODULES_MAIN_SOURCES}
    ${VCO_TEST_SOURCES}
)
target_link_libraries(sy_unt_vco
    ${BOOST_FILESYSTEM}
    ${BOOST_REGEX}
    ${BOOST_SYSTEM}
    ${CPPUNIT}
    ${NETCDF}
    ${XERCES}
    ${XQILLA}
)
set_property(TARGET sy_unt_vco PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(sy_int_syl2_1 src/test/cpp/TestRunner.cpp
    ${CORE_MAIN_SOURCES}
    ${UTIL_MAIN_SOURCES}
    ${SYL2_1_INTEGRATION_TEST_SOURCES}
)
target_link_libraries(sy_int_syl2_1
    ${BOOST_FILESYSTEM}
    ${BOOST_REGEX}
    ${BOOST_SYSTEM}
    ${CPPUNIT}
    ${NETCDF}
    ${XERCES}
    ${XQILLA}
)
set_property(TARGET sy_int_syl2_1 PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#

add_executable(sy_unt_vwr src/test/cpp/TestRunner.cpp
	${CORE_MAIN_SOURCES}
	${UTIL_MAIN_SOURCES}
	${WRITER_MAIN_SOURCES}
    src/test/cpp/writer/VgtWriterTest.cpp src/test/cpp/writer/VgtSegmentProvider.cpp
)
target_link_libraries(sy_unt_vwr
    ${BOOST_FILESYSTEM}
    ${BOOST_REGEX}
    ${BOOST_SYSTEM}
    ${CPPUNIT}
    ${NETCDF}
    ${XERCES}
    ${XQILLA}
)
set_property(TARGET sy_unt_vwr PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(sy_int_syl2_2 src/test/cpp/TestRunner.cpp
    ${CORE_MAIN_SOURCES}
    ${UTIL_MAIN_SOURCES}
    ${READER_MAIN_SOURCES}
    ${WRITER_MAIN_SOURCES}
    ${MODULES_MAIN_SOURCES}
    ${SYL2_2_INTEGRATION_TEST_SOURCES}
)
target_link_libraries(sy_int_syl2_2
    ${BOOST_FILESYSTEM}
    ${BOOST_REGEX}
    ${BOOST_SYSTEM}
    ${CPPUNIT}
    ${NETCDF}
    ${XERCES}
    ${XQILLA}
)
set_property(TARGET sy_int_syl2_2 PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(sy_int_vgtp_1 src/test/cpp/TestRunner.cpp
    ${CORE_MAIN_SOURCES}
    ${UTIL_MAIN_SOURCES}
    ${VGTP_1_INTEGRATION_TEST_SOURCES}
)
target_link_libraries(sy_int_vgtp_1
    ${BOOST_FILESYSTEM}
    ${BOOST_REGEX}
    ${BOOST_SYSTEM}
    ${CPPUNIT}
    ${NETCDF}
    ${XERCES}
    ${XQILLA}
)
set_property(TARGET sy_int_vgtp_1 PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(sy_int_vgtp_2 src/test/cpp/TestRunner.cpp
    ${CORE_MAIN_SOURCES}
    ${UTIL_MAIN_SOURCES}
    ${READER_MAIN_SOURCES}
	${WRITER_MAIN_SOURCES}
    ${MODULES_MAIN_SOURCES}
    ${VGTP_2_INTEGRATION_TEST_SOURCES}
)
target_link_libraries(sy_int_vgtp_2
    ${BOOST_FILESYSTEM}
    ${BOOST_REGEX}
    ${BOOST_SYSTEM}
    ${CPPUNIT}
    ${NETCDF}
    ${XERCES}
    ${XQILLA}
)
set_property(TARGET sy_int_vgtp_2 PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(sy_int_vgts_1 src/test/cpp/TestRunner.cpp
    ${CORE_MAIN_SOURCES}
    ${UTIL_MAIN_SOURCES}
    ${MODULES_MAIN_SOURCES}
    ${VGTS_1_INTEGRATION_TEST_SOURCES}
)
target_link_libraries(sy_int_vgts_1
    ${BOOST_FILESYSTEM}
    ${BOOST_REGEX}
    ${BOOST_SYSTEM}
    ${CPPUNIT}
    ${NETCDF}
    ${XERCES}
    ${XQILLA}
)
set_property(TARGET sy_int_vgts_1 PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(sy_int_vgts_2 src/test/cpp/TestRunner.cpp
    ${CORE_MAIN_SOURCES}
    ${UTIL_MAIN_SOURCES}
    ${READER_MAIN_SOURCES}
    ${WRITER_MAIN_SOURCES}
    ${MODULES_MAIN_SOURCES}
    ${VGTS_2_INTEGRATION_TEST_SOURCES}
)
target_link_libraries(sy_int_vgts_2
    ${BOOST_FILESYSTEM}
    ${BOOST_REGEX}
    ${BOOST_SYSTEM}
    ${CPPUNIT}
    ${NETCDF}
    ${XERCES}
    ${XQILLA}
)
set_property(TARGET sy_int_vgts_2 PROPERTY RUNTIME_OUTPUT_DIRECTORY target)

##
# Project main targets
#
add_executable(syl2 src/main/cpp/tasks/syl2.cpp
    ${CORE_MAIN_SOURCES}
    ${UTIL_MAIN_SOURCES}
    ${READER_MAIN_SOURCES}
    ${WRITER_MAIN_SOURCES}
    ${MODULES_MAIN_SOURCES}
)
target_link_libraries(syl2
    ${BOOST_FILESYSTEM}
    ${BOOST_REGEX}
    ${BOOST_SYSTEM}
    ${NETCDF}
    ${XERCES}
    ${XQILLA}
)
set_property(TARGET syl2 PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(vgtp src/main/cpp/tasks/vgtp.cpp
    ${CORE_MAIN_SOURCES}
    ${UTIL_MAIN_SOURCES}
    ${READER_MAIN_SOURCES}
    ${WRITER_MAIN_SOURCES}
    ${MODULES_MAIN_SOURCES}
)
target_link_libraries(vgtp
    ${BOOST_FILESYSTEM}
    ${BOOST_REGEX}
    ${BOOST_SYSTEM}
    ${NETCDF}
    ${XERCES}
    ${XQILLA}
)
set_property(TARGET vgtp PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(vgts src/main/cpp/tasks/vgts.cpp
    ${CORE_MAIN_SOURCES}
    ${UTIL_MAIN_SOURCES}
    ${READER_MAIN_SOURCES}
    ${WRITER_MAIN_SOURCES}
    ${MODULES_MAIN_SOURCES}
)
target_link_libraries(vgts
    ${BOOST_FILESYSTEM}
    ${BOOST_REGEX}
    ${BOOST_SYSTEM}
    ${NETCDF}
    ${XERCES}
    ${XQILLA}
)
set_property(TARGET vgts PROPERTY RUNTIME_OUTPUT_DIRECTORY target)

find_package(Doxygen)
if(DOXYGEN_FOUND)
add_custom_target(doc
    ${DOXYGEN_EXECUTABLE} src/main/doxygen/doxygen.properties WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Generating API documentation" VERBATIM
)
endif(DOXYGEN_FOUND)

##
# Project installation
#
install(TARGETS syl2 vgtp vgts
  sy_int_syl2_1
  sy_int_syl2_2
  sy_int_vgtp_1
  sy_int_vgtp_2
  sy_int_vgts_1
  sy_int_vgts_2
  sy_unt_gen
  sy_unt_sre
  sy_unt_swr
  sy_unt_col
  sy_unt_pcl
  sy_unt_ave
  sy_unt_aer
  sy_unt_aei
  sy_unt_aco
  sy_unt_vbm
  sy_unt_vfl
  sy_unt_vpr
  sy_unt_vwr
  sy_unt_vac
  sy_unt_vco
  RUNTIME DESTINATION bin
  CONFIGURATIONS Release Debug
)

install(DIRECTORY src/main/bin/ DESTINATION bin
  FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
  FILES_MATCHING PATTERN "*.sh"
)

install(DIRECTORY src/test/bin/ DESTINATION bin
  FILE_PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
  FILES_MATCHING PATTERN "*.sh"
)

install(DIRECTORY src/main/resources DESTINATION src/main
  FILE_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
  PATTERN ".DS_Store" EXCLUDE
)

install(DIRECTORY src/test/resources DESTINATION src/test
  FILE_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
  PATTERN ".DS_Store" EXCLUDE
)

install(DIRECTORY testdata/SY1 DESTINATION testdata
  FILE_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
  PATTERN ".DS_Store" EXCLUDE
)

install(DIRECTORY files/ DESTINATION files
  FILE_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
  PATTERN ".DS_Store" EXCLUDE
)

install(DIRECTORY src/test/resources/config/ DESTINATION config
  FILE_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
  PATTERN ".DS_Store" EXCLUDE
)

install(DIRECTORY src/test/resources/jobs/ DESTINATION jobs
  FILE_PERMISSIONS OWNER_READ OWNER_WRITE GROUP_READ WORLD_READ
  FILES_MATCHING PATTERN "JobOrder.SY_SYS_*.xml"
)

if(DOXYGEN_FOUND)
install(DIRECTORY doc/ DESTINATION doc
)
endif(DOXYGEN_FOUND)

install(DIRECTORY tmp/ DESTINATION tmp
)

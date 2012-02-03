project (s3-synergy)
set(s3-synergy_VERSION_MAJOR 1)
set(s3-synergy_VERSION_MINOR 0)

cmake_minimum_required (VERSION 2.8)

configure_file (
  "src/main/cpp/core/Config.h.in"
  "src/main/cpp/core/Config.h"
)

configure_file (
  "src/test/bin/runalltests.sh.in"
  "src/test/bin/runalltests.sh"
)

set(CMAKE_CXX_FLAGS "-fopenmp -O3 -std=c++98")
set(CMAKE_CXX_FLAGS_DEBUG "-fopenmp -g -O0 -std=c++98 -Wall")

##
# Project source files
#
file(GLOB_RECURSE CORE_MAIN_SOURCES src/main/cpp/core/*.cpp)
file(GLOB_RECURSE CORE_TEST_SOURCES src/test/cpp/core/*.cpp)
file(GLOB_RECURSE READER_MAIN_SOURCES src/main/cpp/reader/*.cpp)
file(GLOB_RECURSE READER_TEST_SOURCES src/test/cpp/reader/*.cpp)
file(GLOB_RECURSE MODULES_MAIN_SOURCES src/main/cpp/modules/*.cpp)
file(GLOB_RECURSE WRITER_MAIN_SOURCES src/main/cpp/writer/*.cpp)
file(GLOB_RECURSE SYN_WRITER_TEST_SOURCES src/test/cpp/writer/SynL2WriterTest.cpp  src/test/cpp/writer/SynL2SegmentProvider.cpp)
file(GLOB_RECURSE VGT_WRITER_TEST_SOURCES src/test/cpp/writer/VgtWriterTest.cpp src/test/cpp/writer/VgtSegmentProvider.cpp)
file(GLOB_RECURSE SEGMENT_WRITER_TEST_SOURCES src/test/cpp/writer/SegmentWriterTest.cpp src/test/cpp/writer/MapSegmentProvider.cpp src/test/cpp/writer/AbstractWriterTest.cpp)
file(GLOB_RECURSE UTIL_MAIN_SOURCES src/main/cpp/util/*.cpp)
file(GLOB_RECURSE UTIL_TEST_SOURCES src/test/cpp/util/*.cpp)

file(GLOB_RECURSE ACO_TEST_SOURCES src/test/cpp/modules/AcoTest.cpp)
file(GLOB_RECURSE AEI_TEST_SOURCES src/test/cpp/modules/AeiTest.cpp)
file(GLOB_RECURSE AER_TEST_SOURCES src/test/cpp/modules/AerTest.cpp)
file(GLOB_RECURSE AVE_TEST_SOURCES src/test/cpp/modules/AveTest.cpp)
file(GLOB_RECURSE COL_TEST_SOURCES src/test/cpp/modules/ColTest.cpp)
file(GLOB_RECURSE PCL_TEST_SOURCES src/test/cpp/modules/PclTest.cpp)
file(GLOB_RECURSE VAC_TEST_SOURCES src/test/cpp/modules/VacTest.cpp)
file(GLOB_RECURSE VBM_TEST_SOURCES src/test/cpp/modules/VbmTest.cpp)
file(GLOB_RECURSE VFL_TEST_SOURCES src/test/cpp/modules/VflTest.cpp)
file(GLOB_RECURSE VPR_TEST_SOURCES src/test/cpp/modules/VprTest.cpp)

file(GLOB_RECURSE SYL2_1_INTEGRATION_TEST_SOURCES src/test/cpp/integration/Syl2_1_Test.cpp)
file(GLOB_RECURSE SYL2_2_INTEGRATION_TEST_SOURCES src/test/cpp/integration/Syl2_2_Test.cpp)

file(GLOB_RECURSE VGTP_1_INTEGRATION_TEST_SOURCES src/test/cpp/integration/VgtP_1_Test.cpp)
file(GLOB_RECURSE VGTP_2_INTEGRATION_TEST_SOURCES src/test/cpp/integration/VgtP_2_Test.cpp)

if(${BOOST_MT})
   set(BOOST_REGEX boost_regex-mt) 
   set(BOOST_SYSTEM boost_system-mt) 
   set(BOOST_FILESYSTEM boost_filesystem-mt)
else(${BOOST_MT})
   set(BOOST_REGEX boost_regex) 
   set(BOOST_SYSTEM boost_system) 
   set(BOOST_FILESYSTEM boost_filesystem)
endif(${BOOST_MT})
 
##
# Project include directories
#
include_directories(before
    ${BOOST_HOME}/include
    ${NETCDF_HOME}/include
    ${XERCES_HOME}/include
    ${XQILLA_HOME}/include
)
include_directories(before
    ${JAVA_HOME}/${JAVA_INC_DIR}
    ${SAFE_HOME}/${SAFE_INC_DIR}
)

##
# Project link directories
#
link_directories(
    ${BOOST_HOME}/lib
    ${NETCDF_HOME}/lib
    ${XERCES_HOME}/lib
    ${XQILLA_HOME}/lib
)
link_directories(
    ${JAVA_HOME}/${JAVA_LIB_DIR}
    ${JAVA_HOME}/${JAVA_JVM_DIR}
    ${SAFE_HOME}/${SAFE_LIB_DIR}
)

##
# Project test targets
#
add_executable(sy_unt_gen src/test/cpp/SyUntGenRunner.cpp
	${CORE_TEST_SOURCES}
	${CORE_MAIN_SOURCES}
	${UTIL_TEST_SOURCES}
	${UTIL_MAIN_SOURCES}
	${WRITER_MAIN_SOURCES}
	${SEGMENT_WRITER_TEST_SOURCES}
)
target_link_libraries(sy_unt_gen
	cppunit
	netcdf
	xerces-c
	xqilla
	${BOOST_REGEX}
	${BOOST_SYSTEM}
	${BOOST_FILESYSTEM}
)
set_property(TARGET sy_unt_gen PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(sy_unt_sre src/test/cpp/SyUntSreRunner.cpp
	${CORE_MAIN_SOURCES}
	${UTIL_MAIN_SOURCES}
	${READER_MAIN_SOURCES}
	${WRITER_MAIN_SOURCES}
	${READER_TEST_SOURCES}
)
target_link_libraries(sy_unt_sre
	cppunit
	netcdf
	xerces-c
	xqilla
	${BOOST_REGEX}
	${BOOST_SYSTEM}
	${BOOST_FILESYSTEM}
)
set_property(TARGET sy_unt_sre PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(sy_unt_swr src/test/cpp/SyUntSwrRunner.cpp
	${CORE_MAIN_SOURCES}
	${UTIL_MAIN_SOURCES}
	${WRITER_MAIN_SOURCES}
	${SYN_WRITER_TEST_SOURCES}
)
target_link_libraries(sy_unt_swr
	cppunit
	netcdf
	xerces-c
	xqilla
	${BOOST_REGEX}
	${BOOST_SYSTEM}
	${BOOST_FILESYSTEM}
)
set_property(TARGET sy_unt_swr PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(sy_unt_col src/test/cpp/SyUntColRunner.cpp
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
	xerces-c
	xqilla
	${BOOST_REGEX}
	${BOOST_SYSTEM}
	${BOOST_FILESYSTEM}
)
set_property(TARGET sy_unt_col PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(sy_unt_pcl src/test/cpp/SyUntPclRunner.cpp
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
	xerces-c
	xqilla
	${BOOST_REGEX}
	${BOOST_SYSTEM}
	${BOOST_FILESYSTEM}
)
set_property(TARGET sy_unt_pcl PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(sy_unt_ave src/test/cpp/SyUntAveRunner.cpp
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
    xerces-c
    xqilla
    ${BOOST_REGEX}
    ${BOOST_SYSTEM}
    ${BOOST_FILESYSTEM}
)
set_property(TARGET sy_unt_ave PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(sy_unt_aer src/test/cpp/SyUntAerRunner.cpp
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
	xerces-c
	xqilla
	${BOOST_REGEX}
	${BOOST_SYSTEM}
	${BOOST_FILESYSTEM}
)
set_property(TARGET sy_unt_aer PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(sy_unt_aei src/test/cpp/SyUntAeiRunner.cpp
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
    xerces-c
    xqilla
    ${BOOST_REGEX}
    ${BOOST_SYSTEM}
    ${BOOST_FILESYSTEM}
)
set_property(TARGET sy_unt_aei PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(sy_unt_aco src/test/cpp/SyUntAcoRunner.cpp
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
    xerces-c
    xqilla
    ${BOOST_REGEX}
    ${BOOST_SYSTEM}
    ${BOOST_FILESYSTEM}
)
set_property(TARGET sy_unt_aco PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(sy_unt_vbm src/test/cpp/SyUntVbmRunner.cpp
    ${CORE_MAIN_SOURCES}
    ${UTIL_MAIN_SOURCES}
    ${READER_MAIN_SOURCES}
    ${WRITER_MAIN_SOURCES}
    ${MODULES_MAIN_SOURCES}
    ${VBM_TEST_SOURCES}
)
target_link_libraries(sy_unt_vbm
    cppunit
    netcdf
    xerces-c
    xqilla
    ${BOOST_REGEX}
    ${BOOST_SYSTEM}
    ${BOOST_FILESYSTEM}
)
set_property(TARGET sy_unt_vbm PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(sy_unt_vfl src/test/cpp/SyUntVflRunner.cpp
    ${CORE_MAIN_SOURCES}
    ${UTIL_MAIN_SOURCES}
    ${READER_MAIN_SOURCES}
    ${WRITER_MAIN_SOURCES}
    ${MODULES_MAIN_SOURCES}
    ${VFL_TEST_SOURCES}
)
target_link_libraries(sy_unt_vfl
    cppunit
    netcdf
    xerces-c
    xqilla
    ${BOOST_REGEX}
    ${BOOST_SYSTEM}
    ${BOOST_FILESYSTEM}
)
set_property(TARGET sy_unt_vfl PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(sy_unt_vpr src/test/cpp/SyUntVprRunner.cpp
    ${CORE_MAIN_SOURCES}
    ${UTIL_MAIN_SOURCES}
    ${READER_MAIN_SOURCES}
    ${WRITER_MAIN_SOURCES}
    ${MODULES_MAIN_SOURCES}
    ${VPR_TEST_SOURCES}
)
target_link_libraries(sy_unt_vpr
    cppunit
    netcdf
    xerces-c
    xqilla
    ${BOOST_REGEX}
    ${BOOST_SYSTEM}
    ${BOOST_FILESYSTEM}
)
set_property(TARGET sy_unt_vpr PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(sy_unt_vac src/test/cpp/SyUntVacRunner.cpp
    ${CORE_MAIN_SOURCES}
    ${UTIL_MAIN_SOURCES}
    ${READER_MAIN_SOURCES}
    ${WRITER_MAIN_SOURCES}
    ${MODULES_MAIN_SOURCES}
    ${VAC_TEST_SOURCES}
)
target_link_libraries(sy_unt_vac
    cppunit
    netcdf
    xerces-c
    xqilla
    ${BOOST_REGEX}
    ${BOOST_SYSTEM}
    ${BOOST_FILESYSTEM}
)
set_property(TARGET sy_unt_vac PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(sy_int_syl2_1 src/test/cpp/SyIntSyl2_1_Runner.cpp
    ${CORE_MAIN_SOURCES}
    ${UTIL_MAIN_SOURCES}
    ${SYL2_1_INTEGRATION_TEST_SOURCES}
)
target_link_libraries(sy_int_syl2_1
    cppunit
    netcdf
    xerces-c
    xqilla
    ${BOOST_REGEX}
    ${BOOST_SYSTEM}
    ${BOOST_FILESYSTEM}
)
set_property(TARGET sy_int_syl2_1 PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#

add_executable(sy_unt_vwr src/test/cpp/SyUntVwrRunner.cpp
	${CORE_MAIN_SOURCES}
	${UTIL_MAIN_SOURCES}
	${WRITER_MAIN_SOURCES}
	${VGT_WRITER_TEST_SOURCES}
)
target_link_libraries(sy_unt_vwr
	cppunit
	netcdf
	xerces-c
	xqilla
    ${BOOST_REGEX}
    ${BOOST_SYSTEM}
    ${BOOST_FILESYSTEM}
)
set_property(TARGET sy_unt_vwr PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(sy_int_syl2_2 src/test/cpp/SyIntSyl2_2_Runner.cpp
    ${CORE_MAIN_SOURCES}
    ${UTIL_MAIN_SOURCES}
    ${READER_MAIN_SOURCES}
    ${WRITER_MAIN_SOURCES}
    ${MODULES_MAIN_SOURCES}
    ${SYL2_2_INTEGRATION_TEST_SOURCES}
)
target_link_libraries(sy_int_syl2_2
    cppunit
    netcdf
    xerces-c
    xqilla
    ${BOOST_REGEX}
    ${BOOST_SYSTEM}
    ${BOOST_FILESYSTEM}
)
set_property(TARGET sy_int_syl2_2 PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(sy_int_vgtp_1 src/test/cpp/SyIntVgtP_1_Runner.cpp
    ${CORE_MAIN_SOURCES}
    ${UTIL_MAIN_SOURCES}
    ${VGTP_1_INTEGRATION_TEST_SOURCES}
)
target_link_libraries(sy_int_vgtp_1
    cppunit
    netcdf
    xerces-c
    xqilla
    ${BOOST_REGEX}
    ${BOOST_SYSTEM}
    ${BOOST_FILESYSTEM}
)
set_property(TARGET sy_int_vgtp_1 PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#
add_executable(sy_int_vgtp_2 src/test/cpp/SyIntVgtP_2_Runner.cpp
    ${CORE_MAIN_SOURCES}
    ${UTIL_MAIN_SOURCES}
    ${READER_MAIN_SOURCES}
	${WRITER_MAIN_SOURCES}
    ${MODULES_MAIN_SOURCES}
    ${VGTP_2_INTEGRATION_TEST_SOURCES}
)
target_link_libraries(sy_int_vgtp_2
    cppunit
    netcdf
    xerces-c
    xqilla
    ${BOOST_REGEX}
    ${BOOST_SYSTEM}
    ${BOOST_FILESYSTEM}
)
set_property(TARGET sy_int_vgtp_2 PROPERTY RUNTIME_OUTPUT_DIRECTORY target)
#

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
    netcdf
    xerces-c
    xqilla
    ${BOOST_REGEX}
    ${BOOST_SYSTEM}
    ${BOOST_FILESYSTEM}
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
    netcdf
    xerces-c
    xqilla
    ${BOOST_REGEX}
    ${BOOST_SYSTEM}
    ${BOOST_FILESYSTEM}
)
set_property(TARGET vgtp PROPERTY RUNTIME_OUTPUT_DIRECTORY target)

##
# Project installation
#
install(TARGETS syl2 vgtp
  sy_int_syl2_1
  sy_int_syl2_2
  sy_int_vgtp_1
  sy_int_vgtp_2
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
  RUNTIME DESTINATION bin
  CONFIGURATIONS Release
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

install(DIRECTORY tmp/ DESTINATION tmp
)

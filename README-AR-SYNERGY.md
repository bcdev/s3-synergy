1. Change to the home directory
===============================

  Type cd /data/sentinel3

2. Unpack the software package
==============================

  Type tar -xzvf s3-synergy.tgz

3. Change to the software directory
===================================

  Type cd /data/sentinel3/s3-synergy

4. Create the CMake files
=========================

  Type cmake .

5. Configure the CMake files
============================

  Type cmake -i

  When prompted you can hit return, except for:
   
    Variable Name: CMAKE_BUILD_TYPE        type 'Release' and hit return
    Variable Name: CMAKE_INSTALL_PREFIX    type the path to the installation directory and hit return
   
  But check that the following variables are set correctly:
   
    Variable Name: BOOST_FILESYSTEM = /data/sentinel3/usr/lib/libboost_filesystem.so
    Variable Name: BOOST_REGEX = /data/sentinel3/usr/lib/libboost_regex.so
    Variable Name: BOOST_SYSTEM = /data/sentinel3/usr/lib/libboost_system.so
    Variable Name: NETCDF = /data/sentinel3/usr/lib/libnetcdf.so
    Variable Name: XERCES = /data/sentinel3/usr/lib/libxerces-c.so
    Variable Name: XQILLA = /data/sentinel3/usr/lib/libxqilla.so

6. Make and install the software  
================================

  Type make -j 12 install
 
7. Change to the installation directory
=======================================

  Type cd /path/to/the/installation/directory
 
8. Run the tests
================

 Type ./bin/runalltests.sh
 
API Documentation
=================

The API documentation (HTML) will be installed into the 'doc' directory of the installation directory.
 

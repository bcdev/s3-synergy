About
=====

The Sentinel-3 Synergy prototype processor aims at prototyping production
algorithms used for the SYN branch of ESA's Sentinel-3 Mission.

The Synergy processor can produce the following products:

1. Aerosol and surface directional reflectance retrieved from both OLCI and
   SLSTR observations
2. VGT-Continuity VGT-P
3. VGT-Continuity VGT-S


Coding Conventions
==================

1. Use camel-case style for class and variable names, where the former begin
   with a capital letter and the latter begin with a lower case letter
2. Declare pointer variables as 'type* name' not 'type *name' or 'type * name'
3. Do not use global 'using <namespace>' directives; use specific
   'using <namespace>::<class>' directives instead
4. Include system header files before project header files
5. Order include directives alphabetically
6. Order 'using ...' directives alphabetically
7. In class declarations the public part comes before the private part
8. Write '(x)' but not '( x )'
9. Write 'a = b', 'a + b' etc. but not 'a=b', 'a+b' etc.


Dependencies
============

The software requires the following libraries, which might not be installed by default:

1.  Boost 1.4.8
2.  Apache Xerces-C 3.1.1
3.  XQilla 2.2.4
4.  NetCDF 4.1.3
5.  CppUnit 1.12
6.  CMake 2.8.2

It is required that the libraries are installed to '/usr', '/usr/local' or '${HOME}/usr'. 

Build Instructions
==================

Install any library needed as dependency, if not already installed. Change to a directory
where you want to host a copy of the source tree. Then type

  git clone git://github.com/bcdev/s3-synergy.git

The source tree, including auxiliary data, will be copied from the repository. Then move
the Sentinel-3 L1c test data into the testdata directory of the source tree. Now you are
ready for building the software.

Linux
-----

For the first build:

1. Change to the project root directory
2. Type 'cmake .'
3. Type 'cmake -i' and adapt the prompted variables to your needs 
4. Type 'make install'

For subsequent builds simply type 'make install'.

Mac OS X
--------

1. Change to the project root directory
2. Type 'cmake -G Xcode .'
3. Type 'cmake -i' and adapt the prompted variables to your needs 

Then use Xcode for building. Alternatively, you may also use GNU make for
building in a manner analogous to the build instruction for Linux. 


Contact Information
===================

For further information please contact:

Ralf Quast
Brockmann Consult
21502 Geesthacht, Germany
ralf.quast@brockmann-consult.de

Thomas Storm
Brockmann Consult
21502 Geesthacht, Germany
thomas.storm@brockmann-consult.de

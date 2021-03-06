HPS Data Summary Tape
=====================

Overview
--------

The HPS Data Summary Tape (DST) maker uses the LCIO C++ API to read collections
of physics objects (e.g. ReconstructedParticle, Track) from reconstructed LCIO
files and convert them to ROOT data structures. The resulting ROOT files can be 
analyzed using the HpsEvent API in combination with the ROOT data analysis 
framework.

Requirements
------------

In order to build the project, the following build tools are required:
* [GCC >= 4.8](https://gcc.gnu.org/install/)
* [CMake >= 2.8.12](http://www.cmake.org/cmake/help/install.html)

The project has the following dependencies: 
* [The LCIO C++ API](http://lcio.desy.de/v02-04-03/doc/manual_html/manual.html#SECTION00030000000000000000)
* [ROOT data analysis framework](http://root.cern.ch/drupal/content/installing-root-source)
** Compatible with both ROOT versions 5 and 6 if build with cmake.

##### Recommended Packages #####

In order the build the documentation, the following package is also required:
* [Doxygen](http://www.stack.nl/~dimitri/doxygen/manual/install.html) 

Installation
------------

##### Getting the Source Code #####

###### Getting a Tagged Release ######

Unless you are an active developer on the project, it is best to use a tagged 
release. A release can be downloaded from the projects 
[release page](https://github.com/JeffersonLab/hps-dst/releases).  The project
can be downloaded as a zip file or tag.gz file.

Alternatively, a release can be downloaded and untarred by issuing the 
following commands from a terminal

    wget https://github.com/JeffersonLab/hps-dst/archive/v0.10.tar.gz
    tar zxvf v0.10.tar.gz
    cd hps-dst-0.10

Once downloaded, the package can be built in the usual way.

###### Cloning the Repository from GitHub ######

The project is stored in a public github repository.  The code can be 
"cloned" i.e. copied to a users local machine by issuing the following commands
from a terminal

	cd /path/to/workdir
	git clone https://github.com/JeffersonLab/hps-dst.git

A github account is not required to clone the source code.

##### Building the Project #####

Before building the project, the following environmental variables need to be set:

	ROOTSYS=/path/to/root
	LCIO=/path/to/lcio

The project can then be built as follows:

	cd hps-dst
	mkdir build; cd build
	cmake ..
	make

In addition, you can specify the install directory if you which to install hps-dst
in a different location from "build" or the hps-dst source directory. This follows
the standard cmake convention, so:

        cd build
        cmake -DCMAKE_INSTALL_PREFIX=/path/to/installation/location
	make
	make install

If you add the switch: "-DHPS_INSTALL_ALL_LIBS=ON", cmake will install all
the libraries and not only libHpsEvent.

For more information on the cmake variables used you can use:

        cd build
        cmake -LH ..

This will create the binaries (in the build/bin directory) along with the shared
library HpsEvent.so (in the build/lib directory) which contains the ROOT 
dictionary and HpsEvent API.  

If Doxygen is installed, the API documentation can be generated as follows:

	make doc

This will generate both LaTex and html documentation in the directory hps-dst/doc.


Maintainers
-----------

* Omar Moreno (SLAC National Accelerator Laboratory) 


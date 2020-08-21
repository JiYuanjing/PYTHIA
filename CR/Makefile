#===============================================================================
#   File: Makefile
#
#   Example Makefile for Pythia8 written for HF in STAR
#   Author: Thomas Ullrich
#   Last modified: September 9, 2008
#
#   This needs 3 packages installed: Pythia8, LHAPDF, and ROOT
#
#   This is setup for Mac OS X 10.5.4 but should be easy to 
#   adapt for other (Unix) platforms. In principle changing the
#   program name (PROGRAM), the location of PYTHIA (PYTHIAPATH),
#   and the LHAPDF libraries (LHAPDFPATH) should do the job.
#   Note that the environment variable ROOTSYS needs to be set.
#   Otherwise define it here in the makefile.
#===============================================================================
PROGRAM  =  pmainJpsi
# PYTHIAPATH   = /data2/weikai/tools/pythia8157
# PYTHIAPATH   = /data2/ytwu/Software/pythia8219
# PYTHIAPATH   = /data2/jiyj/PYTHIASIM/pythia8_2/pythia8235
PYTHIAPATH   = /opt/pythia
PYTHIA_LIB   = $(PYTHIAPATH)/lib
LHAPDFPATH   = /data2/jiyj/PYTHIASIM/LHAPDF/lib
ROOTLIBS = $(shell root-config --libs)
CXX      =  g++
# CXX      =  gcc
CXXFLAGS =  -m64 -fno-inline -O  -W -Wall -pthread -std=c++11 -Wno-deprecated-declarations 
CPPFLAGS = -I$(PYTHIAPATH)/include -I$(ROOTSYS)/include
LDFLAGS  = -L$(PYTHIA_LIB) -Wl,-rpath,$(PYTHIA_LIB) -lpythia8 -ldl  -L$(LHAPDFPATH) -lLHAPDF  $(ROOTLIBS) 
# LDFLAGS  = -L$(PYTHIA_LIB) -Wl,-rpath,$(PYTHIA_LIB) -lpythia8 -ldl -lLHAPDF  $(ROOTLIBS) 

$(PROGRAM):	$(PROGRAM).cpp Makefile
		$(CXX) $(CXXFLAGS) $(PROGRAM).cpp $(CPPFLAGS) $(LDFLAGS) -o $(PROGRAM) 


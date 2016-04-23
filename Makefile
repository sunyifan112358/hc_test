all:
	hcc `hcc-config --cxxflags --ldflags` -O0 -g saxpy.cc -o saxpy


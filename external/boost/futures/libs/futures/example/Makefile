CXX= icc -I/usr/local/include
CXX= xlC_r -I/home/b/bzazschu/download/boost_1_32_0 -qsrcmsg
CXX= /opt/gcc-4.0.0/bin/g++
CXX= g++ -pthread -g
CXXFLAGS= -I../../.. -Wall -O0 -fno-inline
LDFLAGS=-L../../../bin/boost/libs/thread/build/libboost_thread.so/gcc/debug/shared-linkable-true/threading-multi/ 
LDFLAGS+=-lboost_thread-gcc-mt-d-1_33 -lrt -Wl,-rpath-link,.
all:
	$(CXX) $(CXXFLAGS) main.cpp -o testx $(LDFLAGS)

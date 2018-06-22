AM_CPPFLAGS = $(CXX11_FLAGS) -I$(srcdir)/../src
AM_CXXFLAGS = $(PICKY_CXXFLAGS)
LDADD = ../src/libsourdough.a -lpthread

bin_PROGRAMS = tcpclient tcpserver natserver natclient

tcpclient_SOURCES = tcpclient.cc

tcpserver_SOURCES = tcpserver.cc

natserver_SOURCES = natserver.cc

natclient_SOURCES = natclient.cc

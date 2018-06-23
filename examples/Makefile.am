AM_CPPFLAGS = $(CXX14_FLAGS) -I$(srcdir)/../src
AM_CXXFLAGS = $(PICKY_CXXFLAGS)
LDADD = ../src/libsourdough.a -lpthread

bin_PROGRAMS = tcpclient tcpserver natserver natclient echoserver echoclient

tcpclient_SOURCES = tcpclient.cc

tcpserver_SOURCES = tcpserver.cc

natserver_SOURCES = natserver.cc

natclient_SOURCES = natclient.cc

echoserver_SOURCES = echoserver.cc

echoclient_SOURCES = echoclient.cc

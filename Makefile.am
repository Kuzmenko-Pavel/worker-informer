ACLOCAL_AMFLAGS = -I m4

SRC_ALL=src/base64.cpp \
		src/Core.cpp \
		src/Log.cpp \
		src/BaseCore.cpp \
		src/DataBase.cpp \
		src/Informer.cpp \
		src/main.cpp \
		src/Server.cpp \
		src/CgiService.cpp \
		src/EBranch.cpp \
		src/UrlParser.cpp \
		src/Config.cpp \
		src/Params.cpp \
		src/Cookie.cpp \
		src/ParentDB.cpp \
		src/BoostHelpers.cpp \
		src/Core_DataBase.cpp \
		src/CpuStat.cpp 

CMN_CPPFLAGS=-std=c++11 $(AMQP_CFLAGS) $(SQL_CFLAGS) $(MONGO_CFLAGS) -Iinclude
CMN_LDADD = @LIBS@ $(BOOST_DATE_TIME_LIBS) $(BOOST_FILESYSTEM_LIBS) $(BOOST_THREAD_LIBS) $(BOOST_SYSTEM_LIBS) $(BOOST_REGEX_LIBS) $(MONGO_LIBS) $(SQL_LIBS) $(AMQP_LIBS)

if DEBUG
bin_PROGRAMS = worker-informer-dev
worker_informer_dev_SOURCES=$(SRC_ALL)
worker_informer_dev_CPPFLAGS=$(CMN_CPPFLAGS)
worker_informer_dev_LDADD=$(CMN_LDADD)
else
bin_PROGRAMS = worker-informer
worker_informer_SOURCES=$(SRC_ALL)
worker_informer_CPPFLAGS=$(CMN_CPPFLAGS)
worker_informer_LDADD=$(CMN_LDADD)
endif

if DEBUG
AM_CXXFLAGS = -ggdb3 -DDEBUG -Wall -pipe
else
AM_CXXFLAGS = -m64 -Os -march=native -pipe
endif


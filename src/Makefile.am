AUTOMAKE_OPTIONS=foreign
COMPILE_FLAGS=-g -W0
LIBS=-ltag
LIBDIR=-L/usr/local/lib
INCLUDES=-I/usr/local/include
MYFLAGS=$(COMPILE_FLAGS) $(LIBS) $(INCLUDES) $(LIBDIR)
CFLAGS= $(MYFLAGS)
CXXFLAGS=$(MYFLAGS)
CPPFLAGS=$(MYFLAGS)

bin_PROGRAMS=lyricFetcher test
lyricFetcher_SOURCES=fileCtrl.cpp fileCtrl.h lrcFch.cpp lrcFch.h main.cpp stringConv.cpp stringConv.h thread.h threadpool.h threadpool.cpp audioTag.cpp
test_SOURCES=test.cpp fileCtrl.cpp fileCtrl.h lrcFch.cpp lrcFch.h stringConv.cpp stringConv.h thread.h threadpool.h threadpool.cpp audioTag.cpp

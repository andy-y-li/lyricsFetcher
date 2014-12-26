AUTOMAKE_OPTIONS=foreign
COMPILE_FLAGS=-g -DDEBUG 
LIBDIR=-L/usr/local/lib
MYINCLUDES=-I/usr/local/include
MYFLAGS=$(COMPILE_FLAGS)  $(MYINCLUDES) $(LIBDIR)
CFLAGS= $(MYFLAGS)
CXXFLAGS=$(MYFLAGS)
CPPFLAGS=$(MYFLAGS)

bin_PROGRAMS=lyricsFetcher test
lyricsFetcher_SOURCES=fileCtrl.cpp fileCtrl.h lrcFch_qianqian.cpp lrcFch_qianqian.h lrcFch_ting.h lrcFch_ting.cpp main.cpp stringConv.cpp stringConv.h thread.h threadpool.h threadpool.cpp audioTag.cpp socketTool.h socketTool.cpp
test_SOURCES=test.cpp fileCtrl.cpp fileCtrl.h lrcFch_qianqian.cpp lrcFch_qianqian.h lrcFch_ting.h lrcFch_ting.cpp  stringConv.cpp stringConv.h thread.h threadpool.h threadpool.cpp audioTag.cpp socketTool.h socketTool.cpp

lyricsFetcher_LDADD=-ltag
test_LDADD=-ltag



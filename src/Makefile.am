LIBDIR=-L/usr/local/lib
MYINCLUDES=-I/usr/local/include
MYFLAGS=$(COMPILE_FLAGS)  $(MYINCLUDES) $(LIBDIR)
CFLAGS= $(MYFLAGS)
AM_CXXFLAGS=$(MYFLAGS)
AM_CPPFLAGS=$(MYFLAGS)

bin_PROGRAMS=lyricsFetcher 

lyricsFetcher_SOURCES=main.cpp fileCtrl.cpp fileCtrl.h lrcFch_qianqian.cpp lrcFch_qianqian.h lrcFch_ting.h lrcFch_ting.cpp  stringConv.cpp stringConv.h thread.h threadpool.h threadpool.cpp audioTag.cpp socketTool.h socketTool.cpp


lyricsFetcher_LDADD=-ltag


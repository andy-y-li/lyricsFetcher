cc=clang++
CPPFLAGS=-g -O0  -std=gnu++11 -stdlib=libc++

allObjects=fileCtrl.o lrcFch.o main.o stringConv.o 
allTestObjects=test.o

##################################################

all:$(allObjects)
	$(cc)  $(allObjects)   -o lyricsFetcher

clean:
	rm $(allObjects)


.PHONY:clean
.PHONY:test


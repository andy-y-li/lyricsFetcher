//
//  mainTest.cpp
//  lyricsFetcher
//
//  Created by liaogang on 7/7/14.
//  Copyright (c) 2014 gang.liao. All rights reserved.
//

#include <stdio.h>

#define TEST 1
#include <assert.h>
#include "stringConv.h"
#include "lrcFch.h"
#include "stringConv.h"
#include <arpa/inet.h>


#include "audioTag.h"

SearchLyric s;

bool isBigEndian()
{
    int t = 1;
    int t2 = htons(t);

    return t == t2;
}


void test()
{
   std::string result ;
    
 
  if(isBigEndian() )
	  printf("isBigEndian network byte order");
  else
	  printf("isLittleEndian");



    char test1[] = "camillabroken";
    result =  str2UnicodeCode(test1,sizeof(test1)/sizeof(test1[0]) );
    assert(result.compare("630061006D0069006C006C006100620072006F006B0065006E00") == 0 );
    
    
    const    char test2[] = "邓紫棋泡沫";

    
    result =  str2UnicodeCode(test2,strlen(test2) );
    
    assert(result.compare("93902B7DCB68E16CAB6C") == 0 );
    
    
    
   // char test3[] = "刀剑如梦";
   // result =  str2UnicodeCode(test3,sizeof(test3)/sizeof(test3[0]) );
    

    
}


#include "fileCtrl.h"
void testIterFiles()
{
    IterFiles(string("/Users/shijunhe/Download2"), string("/Users/shijunhe/Download2") , nullptr , 0);
}


void  testFunctionInTaglib()
{
    char artist[128] ={0};
    char title[128]={0};
    //const char filename[] =  "/Users/shijunhe/Music/Music/Beat It.mp3";
    const char filename[] =  "/Users/shijunhe/Music/iTunes/iTunes\ Media/Music/信乐团/海阔天空/海阔天空.mp3";
    
    getId3Info(filename , artist ,title );
    
    printf("歌手: %s , 标题 : %s\n", artist ,  title );
    
}


void * testFunctionInTaglib2(const char *file , void *arg)
{
   char artist[128] ={0};
   char title[128]={0};
   //const char filename[] =  "/Users/shijunhe/Music/Music/Beat It.mp3";
   const char filename[] =  "/Users/shijunhe/Music/iTunes/iTunes\ Media/Music/信乐团/海阔天空/海阔天空.mp3";

   getId3Info(filename , artist ,title );

   printf("歌手: %s , 标题 : %s\n", artist ,  title );
    
    return nullptr;
}


//argv is utf-8 format code string.
int main(int argc, const char * argv[]) {
    
    //testFunctionInTaglib();


    //test();
    
    //testIterFiles();
    
    
     const char *artist = "江美琪";
     const char *title = "爱哭鬼";
//    const char *artist = "邓紫棋";
//    const char *title = "泡沫";
    
    
        if(s.Search(artist,title) )
        {
        
        printf("Now , choose a index to download please : ");
        int index=-1;
        char a = getchar();
        
        //index = getw(stdin);
        
        sprintf( (char*)&index,"%c", a);
        index = 1;
        
        printf("You choosed %d\n",index);
        
        
        
        char *saveFile = "/Users/shijunhe/Download2/a.lrc";
        s.Download(index-1,saveFile);
    
        }
    return 0;
}

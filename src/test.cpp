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

SearchLyric s;


void test()
{
   std::string result ;
    
    
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






//argv is utf-8 format code string.
int main(int argc, const char * argv[]) {
    
    
    
    test();
    
    testIterFiles();
    
    return 0;
    
    
    
    
    
    
    
    if (argc < 3 ) {
        printf("Input a artist and title to search\n");
        return 0;
    }
    
    printf("Searching %s  - %s ...\n ",argv[1],argv[2]);
    //printf("dump hex: %x\n%x",argv[1].argv[2]);
    
    const char *artist = argv[1];
    const char *title = argv[2];
    
    /*
    if(s.Init() )
    {
        s.Search(artist,title);
        s.ParseResult();
        
        
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
    */
    
    
    return 0;
}

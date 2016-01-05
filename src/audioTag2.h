#include <taglib/taglib.h>

#import <Cocoa/Cocoa.h>

bool getId3Info(const char * filename , char *artist , char * title  ) ;

bool getId3Info(const char * filename , char *artist , char * title  ,char *album, char *genre, char *year);

/// Reg gbk,@return true if is a valide audio file
bool getID3Info(const char * filename , NSMutableString *artist , NSMutableString * title  ,NSMutableString *album, NSMutableString *genre, NSMutableString *year);

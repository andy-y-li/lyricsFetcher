
#include <string>
#include <fcntl.h>
#include <dirent.h>

using namespace std;

bool IsDirectory(char *pszName);


///return -1 if error . 0 for success.
int IterFiles(string srcPath, string destPath , void * (*doSomeThing)(const char *file , void *arg) , void *arg );

char *getHome();







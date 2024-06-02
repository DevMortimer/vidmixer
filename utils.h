#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <stdlib.h>

const char *findFormat(const char *text);
const char *getBaseName(const char *text);
int stringContains(char **array, int length, const char *target);
char* getVideosPath();

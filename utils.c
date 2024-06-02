#include "utils.h"
#include <stdio.h>

const char *findFormat(const char *text) {
  const char *extension = strrchr(text, '.');
  if (extension) {
    return extension + 1;
  }
  return "";
}

const char *getBaseName(const char *text) {
  const char *lastTrail = strrchr(text, '/');
  return lastTrail + 1;
}

int stringContains(char *array[], int length, const char *target) {
  for (int i = 0; i < length; i++) {
    if (strcmp(array[i], target) == 0) {
      return 1; // Found
    }
  }
  return 0; // Not found
}

#ifdef _WIN32
#include <windows.h>
char *getVideosPath() {
  char videosPath[MAX_PATH];
  if (SHGetFolderPath(NULL, CSIDL_VIDEOS, NULL, 0, videosPath) == S_OK) {
    return _strdup(videosPath);
  } else {
    return NULL;
  }
}
#else
char *getVideosPath() {
  struct passwd *pw = getpwuid(getuid());
  char *home = pw->pw_dir;
  return home;
}
#endif

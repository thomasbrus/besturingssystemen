#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

int main(int argc, char *argv[]) {
  const char *guid = "/nieuws/titel-van-dit-bericht";
  char *last_slash = strrchr(guid, '/');
  printf("%s", last_slash + 1);
    
  return 0;
}
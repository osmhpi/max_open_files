
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <libgen.h>
#include <sys/stat.h>
#include <sys/resource.h>

#define BASE_PATH "files/file.%08i"

int
main (void)
{
  // create the directory if necessary
  char *dir = strdup(BASE_PATH);
  if (dir == NULL)
    {
      perror("strdup");
      return -1;
    }
  if (mkdir(dirname(dir), 0777) == -1)
    {
      if (errno == EEXIST)
        errno = 0;
      else
        {
          perror("mkdir");
          return -1;
        }
    }
  free(dir);

  // open files until failure
  int i = 0;
  int maxfd = 0;
  while (1)
    {
      i++;

      // create a buffer holding the file name
      size_t len = snprintf(NULL, 0, BASE_PATH, i);
      char *path = malloc(sizeof(*path) * (len + 1));
      if (path == NULL)
        {
          perror("malloc");
          return -1;
        }
      snprintf(path, len + 1, BASE_PATH, i);

      // attempt to open the file
      int fd = open(path, O_CREAT | O_RDWR, 0666);
      if (fd < 0)
        {
          // on error, break.
          perror("open");
          errno = 0;
          break;
        }
      if (fd > maxfd)
        maxfd = fd;

      putchar('.');
      fflush(stdout);

      free(path);
    }

  // report results
  printf("successfully opened files before failure:\n  %i\n", i);
  printf("max file descriptor:\n  %i\n", maxfd);

  // query and compare with the resource limit
  struct rlimit rlp = { 0 };
  int res = getrlimit(RLIMIT_NOFILE, &rlp);
  if (res != 0)
    {
      perror("getrlimit");
      return res;
    }

  printf("getrlimit reported (soft) limit:\n  %lu\n", rlp.rlim_cur);
  return 0;
}

#include <errno.h>
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
 
 
/* Execute the file FD refers to, overlaying the running program image.
   ARGV and ENVP are passed to the new program, as for `execve'.  */
int
fexecve (fd, argv, envp)
     int fd;
     char *const argv[];
     char *const envp[];
{
  if (fd < 0 || argv == NULL || envp == NULL)
    {
      __set_errno (EINVAL);
      return -1;
    }
 
  /* We use the /proc filesystem to get the information.  If it is not
     mounted we fail.  */
  char buf[sizeof "/proc/self/fd/" + sizeof (int) * 3];
  __snprintf (buf, sizeof (buf), "/proc/self/fd/%d", fd);
 
  /* We do not need the return value.  */
  __execve (buf, argv, envp);
 
  int save = errno;
 
  /* We come here only if the 'execve' call fails.  Determine whether
     /proc is mounted.  If not we return ENOSYS.  */
  struct stat st;
  if (stat ("/proc/self/fd", &st) != 0 && errno == ENOENT)
    save = ENOSYS;
 
  __set_errno (save);
 
  return -1;
}
 

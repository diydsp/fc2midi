


#include<iostream>

#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <assert.h>

#include <stdint.h>


#include "libfc14audiodecoder-1.0.3/src/FC.h"

size_t getFilesize(const char* filename) {
    struct stat st;
    stat(filename, &st);
    return st.st_size;
}


int main(int argc, char *argv[] )
{
  size_t filesize = getFilesize( argv[ 1 ] );

  using namespace std;
  
  cout << "filesize is: " << filesize << endl;

  //Open file
  int fd = open(argv[1], O_RDONLY, 0);
  assert(fd != -1);
  //Execute mmap
  //  void *mmap(void *addr, size_t length, int prot, int flags,
  //	     int fd, off_t offset);
  void* mmappedData = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, fd, 0);
  //void* mmappedData = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE | MAP_POPULATE, fd, 0);
  assert(mmappedData != MAP_FAILED);
  //Write the mmapped data to stdout (= FD #1)
  //write(1, mmappedData, filesize);
  
  // decode!

  FC fc;

  fc.init( mmappedData, filesize, 0, 10 );

  while(1)
  {
    fc.run();
  }

  
  return 0;
}

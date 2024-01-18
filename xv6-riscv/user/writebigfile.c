#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define O_RDONLY  0x000
#define O_WRONLY  0x001
#define O_RDWR    0x002
#define O_CREATE  0x200
#define O_TRUNC   0x400

#define BSIZE 1024  // block size

#define NDIRECT 11
#define NINDIRECT (BSIZE / sizeof(uint))


#define NDOUBLY_INDIRECT NINDIRECT*NINDIRECT
#define MAXFILE (NDIRECT + NINDIRECT + NDOUBLY_INDIRECT)


int main(int argc, char **argv)
{
  char buf[512];
  int fd, i, sectors;
  printf("\nMAXFILE: %d\n",MAXFILE);
  fd = open("big.file", O_CREATE | O_WRONLY);
  if(fd < 0){
    printf("big: cannot open big.file for writing\n");
   return 0;
  }

  sectors = 0;
  while(sectors<MAXFILE){
    *(int*)buf = sectors;
    int cc = write(fd, buf, sizeof(buf));
    if(cc <= 0)
      break;
    sectors++;
	if (sectors % 658 == 0)
		printf("-");
  }

  printf("\nwrote: %d \n", sectors);

  close(fd);
  fd = open("big.file", O_RDONLY);
  if(fd < 0){
    printf("big: cannot re-open big.file for reading\n");
     return 0;
  }
  for(i = 0; i < sectors; i++){
    int cc = read(fd, buf, sizeof(buf));
    if(cc <= 0){
      printf("big: read error at sector %d\n", i);
      return 0;
    }
    if(*(int*)buf != i){
      printf("big: read the wrong data (%d) for sector %d\n",
             *(int*)buf, i);
       return 0;
    }

  }
  unlink("big.file");
  exit(1);
}

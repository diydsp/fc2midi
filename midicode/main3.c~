#include <stdio.h>
#include "main.h"
#include "it2midi.h"

int main(int argc, char *argv[])
{
  FILE *itfile,*midifile;

  if(argc!=3){print_help();exit(0);}

  printf("argv[1]: %s.\n",argv[1]);
  printf("argv[2]: %s.\n",argv[2]);

  if((midifile=fopen(argv[1],"rb"))==NULL)
  {
    printf("sorry kid.  You fucked up.  No such input file.\n");
    exit(0);
  }

  if((itfile=fopen(argv[2],"wb"))==NULL)
  {
    printf("sorry kid.  You fucked up.  No such output file.\n");
    exit(0);
  }

  // Process it!
  midi2it(midifile,itfile);

  // close files
  fclose(itfile);
  fclose(midifile);

  return(1);
}

void print_help(void)
{
  printf("midi2it <input MIDI file> <output IT file>\n");
}


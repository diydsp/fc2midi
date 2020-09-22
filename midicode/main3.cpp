#include <stdio.h>
#include <stdlib.h>
#include "examidi.h"
#include "main.h"  // for print_help

//#include "it2midi.h"

int main(int argc, char *argv[])
{
  FILE *itfile,*midifile;

  if(argc!=2){print_help();exit(0);}

  printf("argv[1]: %s.\n",argv[1]);

  if((midifile=fopen(argv[1],"rb"))==NULL)
  {
    printf("sorry kid.  You fucked up.  No such input file.\n");
    exit(0);
  }

  // Process it!
  examidi(midifile);

  // close files
  fclose(midifile);

  return(1);
}

void print_help(void)
{
  printf("examidi <input MIDI file>\n");
}


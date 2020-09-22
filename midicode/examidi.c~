#include <stdio.h>
#include "useful.h"
#include "midi2it.h"
#include "it_types.h"
#include "it_read.h"
#include "it_disp.h"
#include "midi_types.h"

int midi2it(FILE *mf,FILE *itf)
{
  MIDIHdr mh;
  MIDITrk trk;
  int i;
  
  midi_readHdr(mf,&mh);
  midi_dispHdr(&mh);

  for(i=0;i<mh.NumTracks;i++)
  {
    printf("Track #%d. (zero-based)\n",i);
    printf("----------\n");
    midi_initTrk(&trk);
    midi_readTrk(mf,&trk);
    midi_dispTrk(&trk);
  }
  
  return(1);
}

#include <stdio.h>
#include "useful.h"
#include "midi_types.h"
#include "midi_read.h"
#include "midi_disp.h"
#include "midi_write.h"

int examidi(FILE *mf)
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

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "useful.h"
#include "midi_types.h"
#include "midi_read.h"

int midi_readHdr(FILE *mf,MIDIHdr *mh)
{
  byte buf[8],*buf2;

  // Read identifier - MThd
  fread(buf,4,1,mf);buf[4]=0x0; // for strcmp
  if(strcmp((char*)buf,"MThd"))
  {
    printf("not a midi file.\n");
    printf("no MThd header found. (%s)\n",buf);
    return(0);
  }

  // Read length, will probably be 6.
  fread(buf,4,1,mf);
  mh->len = Biglongat( (unsigned char *) &buf [0 ] );

  // Read out data fields
  buf2 = (byte *)malloc(mh->len*sizeof(byte));
  if(buf2==NULL)
  {
    printf("not enough mem to allocate %ld bytes to read midi header.\n",
	   mh->len);
    return(0);
  }
  
  fread(buf2,mh->len,1,mf);
  mh->Format    = Bigintat( (unsigned byte *)&buf2[0]);
  mh->NumTracks = Bigintat( (unsigned char *)&buf2[2]);
  mh->PPQN      = Bigintat( (unsigned char *)&buf2[4]);

  free(buf2);
  return(1);

}

int midi_readTrk(FILE *mf,MIDITrk *trk)
{
  byte buf[8];

  // verify its a track
  fread(buf,4,1,mf);buf[4]=0x0; // for strcmp
  if(strcmp((char*)buf,"MTrk"))
  {
    printf("MTrk track header not found where expected.\n");
    return(0);
  }

  // get length
  fread(buf,4,1,mf);
  trk->len=Biglongat( (unsigned char *) &buf[0] );
  
  // read the data
  trk->data = (unsigned char *)malloc(trk->len*sizeof(byte));
  if(trk->data==NULL)
  {
    printf("not enough memory to read in MIDI Track.\n");
    printf("(Trk length = 0x%04lx.\n",trk->len);
    return(0);
  }
  fread(trk->data,trk->len,1,mf);
    
  return(1);
}

// reads from an array, not a file
int midi_readDeltaTime(byte *ptr,unsigned long *deltatime)
{
  unsigned long val;
  byte c;
  int len=1,index=0;

  if( (val=*(ptr++))&0x80)
  {
    val&=0x7F;
    do
    {
      val=(val<<7) + ((c=*ptr++)&0x7F);
      len++;
    } while (c & 0x80);
  }
  *deltatime=val;
  return(len);
}


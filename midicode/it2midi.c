#include <stdio.h>
#include "useful.h"
#include "it2midi.h"
#include "it_types.h"
#include "it_read.h"
#include "it_disp.h"
#include "midi_types.h"
#include "midi_write.h"

int it2midi(FILE *itf,FILE *mf)
{
  Ithdr ithdr;
  MIDIHdr mhdr;
  MIDITrk Trk;
  int i,ch;
  
  // Read IT file
  it_readhdr(&ithdr,itf);
  it_disphdr(&ithdr);

  // Initialize midifile header
  mhdr.Format    = 0x0001;   // multiple sequences
  mhdr.NumTracks = 0x0002;   // 2 for now
  mhdr.PPQN      = 0x0180;   // 384 

  // write dummy header in midifile
  midi_writeHdr(&mhdr,mf);

  // Write Track Zero with time sig, tempo, and sequence name
  midi_initTrk      (&Trk);
  midi_writeTrkHdr  (mf,&Trk);

  midi_writeDeltaTime(mf,&Trk,0x00000000);
  midi_writeTimeSig (mf,&Trk,0x04,0x02,0x18,0x08); // 4/4,8x1/32 notes<->24 F8
  midi_writeDeltaTime(mf,&Trk,0x00000000);
  midi_writeTempo   (mf,&Trk,ithdr.IT);
  midi_writeDeltaTime(mf,&Trk,0x00000000);
  midi_writeText    (mf,&Trk,SEQTRKNAME,"SEQ00001");
  midi_writeDeltaTime(mf,&Trk,0x00000000);
  midi_writeEndofTrk(mf,&Trk);

  midi_writeTrkLen  (mf,&Trk);

  // Read patterns out of IT file
  for(i=0;i<ithdr.PatNum;i++)
  {
    it_readpatt(&ithdr.patt[i],itf);

    // scan the pattern once for each channel
    it_disppatt(&ithdr.patt[i]);
    for(ch=0;ch<1;ch++)
    {
      midi_initTrk(&Trk);
      midi_writeTrkHdr(mf,&Trk);

      // Fill in some program change msgs.
      midi_writeDeltaTime(mf,&Trk,0x00000000);
      midi_writeEvent(mf,&Trk,CONTROL,ch,0x00,0x00); // bank select (fine) 0

      midi_writeDeltaTime(mf,&Trk,0x00000000);
      midi_writeEvent(mf,&Trk,CONTROL,ch,0x20,0x00); // bank select (coarse) 0

      midi_writeDeltaTime(mf,&Trk,0x00000000);
      midi_writeEvent(mf,&Trk,PROGRAM,ch,0x01,0xFF); // Prg change to 1

      midi_writeDeltaTime(mf,&Trk,0x00000000);
      midi_writeEvent(mf,&Trk,CONTROL,ch,0x07,0x7f); // Volume full

      midi_writeDeltaTime(mf,&Trk,0x00000000);
      midi_writeEvent(mf,&Trk,CONTROL,ch,0x0a,0x40); // Pan middle

      midi_writeDeltaTime(mf,&Trk,0x00000000);
      midi_writeEvent(mf,&Trk,CONTROL,ch,0x01,0x00); // Reset Mod Wheel

      midi_writeDeltaTime(mf,&Trk,0x00000000);
      midi_writeEvent(mf,&Trk,CONTROL,ch,0x0b,0x7f); // Expression full

      midi_writeDeltaTime(mf,&Trk,0x00000000);
      midi_writeEvent(mf,&Trk,CONTROL,ch,0x46,0x00); // Sound vary none

      midi_writeDeltaTime(mf,&Trk,0x00000000);
      midi_writeEvent(mf,&Trk,CONTROL,ch,0x00,0x40); // reset pitch wheel

      patt2Trk(&ithdr.patt[i],mf,&Trk,ch);

      midi_writeDeltaTime(mf,&Trk,0x00000000);
      midi_writeEndofTrk(mf,&Trk);

      midi_writeTrkLen(mf,&Trk);
    }
  }

  // write header in midifile
  //midi_writeHdr(&mhdr,mf);
}

// based on it_disp.c/it_disppatt(), this function
// unpacks IT patterns, extracts events relating to
// a particular channel, and adds them to a midi file
int patt2Trk(Itpatt *itpatt,FILE *mf,MIDITrk *trk,int ch)
{
  unsigned int index=0;
  int channelvar,maskvar,prevmaskvar,channel;
  int oldpos=0,pos=0;
  byte note;
  unsigned long miditime=0;

  printf("Extracting channel %d.\n",ch);

  do
  {
    printf("Pos: 0x%02x.\n",pos);
    channelvar=itpatt->data[index++];
 
    if(channelvar==0)
    {
      pos++;
      continue;
    }

    channel=(channelvar-1)&0x3f;
    printf("channel = 0x%02x.\n",channel);

    if( (channelvar&0x80) == 0x80 )  // get new maskvar
    {
      maskvar = itpatt->data[index++];
    }
    
    if( (maskvar&0x01) == 0x01 )     // read note
    {
      note=itpatt->data[index++];
      printf("Note   : 0x%02x\n",note);
      if(channel==ch)
      {
	printf("scribing to MIDIfile.\n");
	midi_writeDeltaTime(mf,trk,pos-oldpos);oldpos=pos;
	midi_writeEvent(mf,trk,NOTEON,0x00,note,0x7f);
      }
    }
    if( (maskvar&0x02) == 0x02 )     // read instr
    {
      printf("Inst   : 0x%02x\n",itpatt->data[index++]);
    }
    if( (maskvar&0x04) == 0x04 )     // read vol/pan
    {
      printf("Vol/Pan: 0x%02x\n",itpatt->data[index++]);
    }
    if( (maskvar&0x08) == 0x08 )     // read comm & comm val
    {
      printf("Comm/Val: 0x%02x, 0x%02x\n",itpatt->data[index++],
	                                itpatt->data[index++]);
    }
    if( (maskvar&0x10) == 0x10 )     // note = lastnote for channel
    {
      printf("Same note.\n");
      if(channel==ch)
      {
	printf("scribing to MIDIfile.\n");
	midi_writeDeltaTime(mf,trk,pos-oldpos);oldpos=pos;
	midi_writeEvent(mf,trk,NOTEON,0x00,note,0x7f);
      }
    }
    if( (maskvar&0x20) == 0x20 )     // inst = lastinstr for chan
    {
      printf("Same inst.\n");
    }
    if( (maskvar&0x40) == 0x40 )     // vol/pan = last vol / pan
    {
      printf("Same vol/pan.\n");
    }
    if( (maskvar&0x80) == 0x80 )     // comm = last comm
    {
      printf("Same vol/pan.\n");
    }
  } while (index < itpatt->length);
}



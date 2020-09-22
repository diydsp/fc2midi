#include <stdio.h>
#include "useful.h"
#include "it_types.h"
#include "it_disp.h"

void it_disphdr(Ithdr *ithdr)
{
  int i;

  printf("Song Name: %s.\n",ithdr->Name);
  printf("OrdNum:    %d.\n",ithdr->OrdNum);
  printf("InsNum:    %d.\n",ithdr->InsNum);
  printf("SmpNum:    %d.\n",ithdr->SmpNum);
  printf("PatNum:    %d.\n",ithdr->PatNum);

  printf("\n");
  printf("Cwt   :    %d.%d\n",ithdr->Cwty, ithdr->Cwtx);
  printf("Cmwt  :    %d.%d\n",ithdr->Cmwty,ithdr->Cmwtx);

  printf("\n");
  printf("Flags :    0x%04x.\n",ithdr->Flags);
  it_dispflags(ithdr->Flags);
  printf("Special:   0x%04x.\n",ithdr->Special);
  it_dispspecial(ithdr->Special);

  printf("\n");
  printf("GV    :    %d.\n",ithdr->GV);
  printf("MV    :    %d.\n",ithdr->MV);
  printf("IS    :    %d.\n",ithdr->IS);
  printf("IT    :    %d.\n",ithdr->IT);
  printf("Sep   :    %d.\n",ithdr->Sep);
  printf("PWD   :    %d.\n",ithdr->PWD);

  printf("\n");
  printf("MsgLgth:   %d.\n",ithdr->MsgLgth);
  printf("MsgOffset: %08x.\n",ithdr->MsgOffset);

  printf("\n");
  printf("Channel Pan:\n");
  bin_disp(ithdr->pan,64);

  printf("Channel Volume:\n");
  bin_disp(ithdr->vol,64);

  printf("Orders:\n");
  bin_disp(ithdr->Orders,ithdr->OrdNum);

  printf("\n");
  printf("OffsetInst:\n");
  for(i=0;i<ithdr->InsNum;i++)
    printf("\t0x%08x.\n",ithdr->inst[i].offset);

  printf("OffsetSamp:\n");
  for(i=0;i<ithdr->SmpNum;i++)
    printf("\t0x%08x.\n",ithdr->samp[i].offset);

  printf("OffsetPatt:\n");
  for(i=0;i<ithdr->PatNum;i++)
    printf("\t0x%08x.\n",ithdr->patt[i].offset);
}

void it_dispinst(Itinst *itinst)
{
}

void it_dispsamp(Itsamp *itsamp)
{
}

void it_disppatt(Itpatt *itpatt)
{
  unsigned int index=0;
  int channelvar,maskvar,prevmaskvar,channel;
  int pos=0;

  printf("Pattern Data\n");
  printf("------------\n");
  printf("offset    :0x%04x.\n",itpatt->offset);
  printf("length    :0x%04x.\n",itpatt->length);
  printf("rows      :0x%02x.\n",itpatt->rows);
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
      printf("Note   : 0x%02x\n",itpatt->data[index++]);
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
  bin_disp(itpatt->data,itpatt->length);
}

void it_dispflags(unsigned char flags)
{
  if( (flags&0x01) == 0x01)
  {
    printf("\tStereo\n");
  } else 
  {
    printf("\tMono\n");
  }

  if( (flags&0x02) == 0x02)
  {
    printf("\tVol0MixOptimiztions: On\n");
  } else 
  {
    printf("\tVol0MixOptimiztions: Off\n");
  }

  if( (flags&0x04) == 0x04)
  {
    printf("\tUse instruments\n");
  } else 
  {
    printf("\tUse samples\n");
  }

  if( (flags&0x08) == 0x08)
  {
    printf("\tLinear Slides\n");
  } else 
  {
    printf("\tAmiga Slides\n");
  }

  if( (flags&0x10) == 0x10)
  {
    printf("\tOld Effects\n");
  } else 
  {
    printf("\tIT Effects\n");
  }

  if( (flags&0x20) == 0x20)
  {
    printf("\tLink Effect G's mem with E/F\n");
  } else 
  {
    printf("\tNo link FX G mem w/ E/F\n");
  }

  if( (flags&0x40) == 0x40)
  {
    printf("\tUse MIDI pitch controller\n");
  } else 
  {
    printf("\tDo not use MIDI pitch controller\n");
  }

  if( (flags&0x80) == 0x80)
  {
    printf("\tRequest embedded MIDI config\n");
  } else 
  {
    printf("\tDo not request embedded MIDI config\n");
  }
}
void it_dispspecial(unsigned char special)
{
  if( (special&0x01) == 0x01)
  {
    printf("\tSong message attached.\n");
  } else 
  {
    printf("\tno song message\n");
  }

  if( (special&0x02) == 0x02)
  {
    printf("\t.\n");
  } else 
  {
    printf("\t.\n");
  }

  if( (special&0x04) == 0x04)
  {
    printf("\t.\n");
  } else 
  {
    printf("\t.\n");
  }

  if( (special&0x08) == 0x08)
  {
    printf("\tMIDI config embedded\n");
  } else 
  {
    printf("\tNo MIDI config embedded\n");
  }
}

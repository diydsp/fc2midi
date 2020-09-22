#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "useful.h"
#include "midi_types.h"
#include "midi_write.h"

int midi_writeHdr(MIDIHdr *mhdr, FILE *fi)
{
  byte buf[9];

  sprintf((char*)buf,"MThd%c%c%c%c",0x00,0x00,0x00,0x06);
  fwrite(buf,8,1,fi);

  buf[0]=(mhdr->Format    >>8)&0xff;
  buf[1]=(mhdr->Format       )&0xff;
  buf[2]=(mhdr->NumTracks >>8)&0xff;
  buf[3]=(mhdr->NumTracks    )&0xff;
  buf[4]=(mhdr->PPQN      >>8)&0xff;
  buf[5]=(mhdr->PPQN         )&0xff;
  fwrite(buf,6,1,fi);
  return(1);
}

// save pos in file, go back, fill in len, return to pos when called.
int midi_writeTrkLen(FILE *fi,MIDITrk *mt)
{
  long bookmark=ftell(fi);
  byte buf[4];
  byte len[4];

  printf("from writeTrkLen,len_offset is %ld.\n",mt->len_offset);

  // zip file back to correct place
  fseek(fi,mt->len_offset,SEEK_SET);
  splitup32(mt->len,len);
  sprintf((char*)buf,"%c%c%c%c",len[0],len[1],len[2],len[3]);
  fwrite(buf,4,1,fi);

  // return file pointer to where it was before this call
  fseek(fi,0L,SEEK_END);
  
  return(1);
}

int midi_writeTrkHdr(FILE *fi,MIDITrk *trk)
{
  byte buf[8];

  // write header - fill in length later
  sprintf((char*)buf,"MTrk");
  fwrite(buf,4,1,fi);

  // fill in length later
  trk->len_offset=ftell(fi);
  printf("len_offset is %ld.\n",trk->len_offset);
  sprintf((char*)buf,"%c%c%c%c",0x00,0x00,0x00,0x00);
  fwrite(buf,4,1,fi);
}

int midi_writeDeltaTime(FILE *fi,MIDITrk *trk,unsigned long val)
{
  unsigned long buf;
  int len=0;

  buf=val&0x7f;

  // shift out all bits into buf
  while( (val>>=7))
  {
    buf<<=8;
    buf |= ((val&0x7f) | 0x80);
  }

  // write necessary bytes
  while(1)
  {
    putc(buf,fi); len++;
    if(buf&0x80)buf >>=8;
    else break;
  }
  trk->len+=len;
  printf("delta time len:%d.\n",len);
  return(len);
}

void midi_initTrk(MIDITrk *trk)
{
  trk->len=0;
  trk->status=0x00;
  trk->len_offset=0x0;
  trk->data=NULL;
}

int midi_writeSeqNum(FILE *fi,MIDITrk *trk,unsigned int SeqNum)
{
  byte buf[5];
  
  buf[0] = 0xff;
  buf[1] = 0x00;
  buf[2] = 0x02;
  buf[3] = SeqNum>>8;
  buf[4] = SeqNum&0xff;
  fwrite(buf,5,1,fi);
  trk->len+=5;
  return(1);
}

int midi_writeText(FILE *fi,MIDITrk *trk,byte type,char *txt)
{
  byte buf[3];
  int16_t len;

  len=strlen(txt);

  buf[0] = 0xff;
  buf[1] = type;
  buf[2] = len;
  fwrite(buf,3,1,fi);
  fwrite(txt,len,1,fi);
  trk->len+=(3+len);
  return(1);
}


int midi_writeMIDICh(FILE *fi,MIDITrk *trk,byte ch)
{
  byte buf[4];

  buf[0] = 0xff;
  buf[1] = 0x20;
  buf[2] = 0x01;
  buf[3] = ch;
  fwrite(buf,4,1,fi);
  trk->len+=4;
  return(1);
}

int midi_writeMIDIPort(FILE *fi,MIDITrk *trk,byte port)
{
  byte buf[4];

  buf[0] = 0xff;
  buf[1] = 0x21;
  buf[2] = 0x01;
  buf[3] = port;
  fwrite(buf,4,1,fi);
  trk->len+=4;
  return(1);
}

int midi_writeEndofTrk(FILE *fi,MIDITrk *trk)
{
  byte buf[3];

  buf[0] = 0xff;
  buf[1] = 0x2f;
  buf[2] = 0x00;
  fwrite(buf,3,1,fi);
  trk->len+=3;
  return(1);
}

int midi_writeTempo(FILE *fi,MIDITrk *trk,int bpm)
{
  byte buf[6];
  unsigned long val=60000000/bpm;
  
  buf[0] = 0xff;
  buf[1] = 0x51;
  buf[2] = 0x03;
  buf[3] = (val>>16)&0xff;
  buf[4] = (val>> 8)&0xff;
  buf[5] = (val    )&0xff;
  fwrite(buf,6,1,fi);
  trk->len+=6;
  return(1);
}

// writing midi events
int midi_writeEvent(FILE *fi,MIDITrk *trk,
		    byte status,byte ch,byte data1,byte data2)
{
  int len=0;
  byte buf[3];

  if(trk->status != status)
  {
    buf[0]=status+ch;
    fwrite(buf,1,1,fi);
    trk->len+=1;
    trk->status=status;
  }
  buf[0]=data1;  len++;
  if( (status!=0xC0) && (status!=0xD0) )
  {
    buf[1]=data2;  len++;
  }
  fwrite(buf,len,1,fi);
  trk->len+=len;
  printf("event len:%d.\n",len);
}

int midi_writeTimeSig(FILE *mf,MIDITrk *trk,byte nn,byte dd,byte cc,byte bb)
{
  byte buf[8];
  
  buf[0] = 0xff;
  buf[1] = 0x58;
  buf[2] = 0x04;
  buf[3] = nn;
  buf[4] = dd;
  buf[5] = cc;
  buf[6] = bb;
  fwrite(buf,7,1,mf);
  trk->len+=7;
  return(1);
}



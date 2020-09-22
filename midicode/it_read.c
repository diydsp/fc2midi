#include <stdio.h>
#include "useful.h"
#include "it_types.h"
#include "it_read.h"

int it_readhdr(Ithdr *ithdr,FILE *fi)
{
  int i;
  byte header[192];

  // read header block 
  bzero(header,sizeof(header));
  fread(&header[0],192,1,fi);

  // verify IMPM file
  if( header[0] != 'I' ||
      header[1] != 'M' ||
      header[2] != 'P' ||
      header[3] != 'M') 
    {
      printf("not Impulse Tracker file.\n");
      return(0);
    }

  // Song Name
  strcpy(ithdr->Name,&header[4]);

  // General Fields
  ithdr->OrdNum  = intat(&header[0x20]);
  ithdr->InsNum  = intat(&header[0x22]);
  ithdr->SmpNum  = intat(&header[0x24]);
  ithdr->PatNum  = intat(&header[0x26]);

  ithdr->Cwtx    = header[0x28];
  ithdr->Cwty    = header[0x29];
  ithdr->Cmwtx   = header[0x2a];
  ithdr->Cmwty   = header[0x2b];

  ithdr->Flags   = intat(&header[0x2c]);
  ithdr->Special = intat(&header[0x2e]);

  ithdr->GV      = header[0x30];
  ithdr->MV      = header[0x31];
  ithdr->IS      = header[0x32];
  ithdr->IT      = header[0x33];
  ithdr->Sep     = header[0x34];
  ithdr->PWD     = header[0x35];

  ithdr->MsgLgth   = intat(&header[0x36]);
  ithdr->MsgOffset = longat(&header[0x38]);

  for(i=0;i<64;i++)ithdr->pan[i] = header[0x40+i];
  for(i=0;i<64;i++)ithdr->vol[i] = header[0x80+i];

  ithdr->Orders=(byte *)malloc(ithdr->OrdNum*sizeof(byte));
  if(ithdr->Orders==NULL)
  {
    printf("out of memory #1.\n");
    exit(0);
  }

  for(i=0;i<ithdr->OrdNum;i++)
  {
    fread(&header[0],1,1,fi);
    ithdr->Orders[i]=header[0];
  }

  ithdr->inst = (Itinst *)malloc(ithdr->InsNum*sizeof(Itinst));
  if(ithdr->inst==NULL){printf("mem crap.\n");return(0);}
  for(i=0;i<ithdr->InsNum;i++)
  {
    fread(&header[0],4,1,fi);
    ithdr->inst[i].offset=longat(&header[0x00]);
  }

  ithdr->samp = (Itsamp *)malloc(ithdr->SmpNum*sizeof(Itsamp));
  if(ithdr->samp==NULL){printf("mem crap.\n");return(0);}
  for(i=0;i<ithdr->SmpNum;i++)
  {
    fread(&header[0],4,1,fi);
    ithdr->samp[i].offset=longat(&header[0x00]);
  }

  ithdr->patt = (Itpatt *)malloc(ithdr->PatNum*sizeof(Itpatt));
  if(ithdr->patt==NULL){printf("mem crap.\n");return(0);}
  for(i=0;i<ithdr->PatNum;i++)
  {
    fread(&header[0],4,1,fi);
    ithdr->patt[i].offset=longat(&header[0x00]);
  }
  
  return(1);
}

int it_readinst(Itinst *itinst,FILE *fi)
{
}

int it_readsamp(Itsamp *itsamp,FILE *fi)
{
}

int it_readpatt(Itpatt *itpatt,FILE *fi)
{
  byte buf[80];

  fseek(fi,itpatt->offset,SEEK_SET);

  // get length & rows
  fread(&buf[0],8,1,fi);
  itpatt->length = intat(&buf[0]);
  itpatt->rows   = buf[2];

  itpatt->data = (byte *)malloc(itpatt->length*sizeof(byte));
  if(itpatt->data == NULL)
  {
    printf("not enough mem.\n");
    return(0);
  }
  fread(itpatt->data,itpatt->length,1,fi);

}



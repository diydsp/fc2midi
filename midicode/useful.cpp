#include <stdio.h>
#include "useful.h"
#include "it_types.h"

void bin_disp(byte *header,int len)
{
  int pos,i;
  
  for(pos=0,i=0;i<len;i++)
  {
    if(pos==0)
    {
      printf("%04x: ",i);
    }

    printf("%02x ",header[i]);
    pos++;if(pos==16){pos=0;printf("\n");}
  }
  printf("\n");
}

unsigned int Bigintat(unsigned char *ptr)
{
  return( (ptr[0]<<8) + ptr[1] );
}

unsigned int intat(unsigned char *ptr)
{
  return(256*ptr[1]+ptr[0]);
}

unsigned long longat(unsigned char *ptr)
{
  return( (ptr[3]<<24) + (ptr[2]<<16) + (ptr[1]<<8) + ptr[0] );
}

unsigned long Biglongat(unsigned char *ptr)
{
  return( (ptr[0]<<24) + (ptr[1]<<16) + (ptr[2]<<8) + ptr[3] );
}

void splitup32(long val,unsigned char digit[])
{
  digit[0] = (val>>24)&0xff;
  digit[1] = (val>>16)&0xff;
  digit[2] = (val>> 8)&0xff;
  digit[3] = (val    )&0xff;
}

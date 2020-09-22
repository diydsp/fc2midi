



#include<iostream>

#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <assert.h>
#include <stdint.h>
#include <iostream>
#include <iomanip>

#include "libfc14audiodecoder-1.0.3/src/FC.h"

#include "fc2midi.h"

using namespace std;

size_t getFilesize(const char* filename) {
    struct stat st;
    stat(filename, &st);
    return st.st_size;
}


void print_help(void)
{
  printf("examidi <input Future Composer file>\n");
}

int main(int argc, char *argv[] )
{
  FILE *fi;
  
  if(argc!=2){print_help();exit(0);}

  printf("argv[1]: %s.\n",argv[1]);

  if((fi=fopen(argv[1],"rb"))==NULL)
  {
    printf("sorry kid.  You fucked up.  No such input file.\n");
    exit(0);
  }

  size_t filesize = getFilesize( argv[ 1 ] );

  
  cout << "filesize is: " << filesize << endl;

  //Open file
  int fd = open(argv[1], O_RDONLY, 0);
  assert(fd != -1);
  //Execute mmap
  //  void *mmap(void *addr, size_t length, int prot, int flags,
  //	     int fd, off_t offset);
  void* mmappedData = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, fd, 0);
  //void* mmappedData = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE | MAP_POPULATE, fd, 0);
  assert(mmappedData != MAP_FAILED);
  //Write the mmapped data to stdout (= FD #1)
  //write(1, mmappedData, filesize);
  
  // decode!

  FC fc;
  fc_chan_t fc_chan[ 4 ];  // FC files always 4 voices
  for( auto idx = 0; idx < 4; idx++ )
  {
    fc_chan_clear( &fc_chan[ 0 ] );
  }
  
  fc.init( mmappedData, filesize, 0, 10 );

  while(1)
  {
   

    //fc.run();
    
    printf("count,speed=(%d,%d)\n",fc._admin.count,fc._admin.speed);	

    //if( fc._admin.count == 0 )  // it reloaded/ran NextNote()
    if( fc._admin.count == 1 )  // it reloaded/ran NextNote()
    { 
      // for each voice    
      for( auto idx=0; idx<4; idx++ )
      {
        //cout << "xxx " << fc._CHdata[ idx ].pattPos << endl;
        printf("voice %d:",idx);                            
        struct FC::CHdata *p_CHdata = &fc._CHdata[ idx ];
      
        if(  p_CHdata->dmaMask==1 &&
 	  p_CHdata->pattPos==0)
        {
          //printf("voice %d:",idx);
          cout << endl;
          cout << "[Step = " << hex << setw(4) << setfill('0') << p_CHdata->trackPos/ FC::TRACKTAB_ENTRY_LENGTH;
          cout << " | " << hex << setw(5) << setfill('0') << (int)p_CHdata->trackStart << ", " << (int)(p_CHdata->trackStart + p_CHdata->trackPos) << ", " << (int)p_CHdata->trackEnd << ']' << endl;
          udword tmp = p_CHdata->trackStart + p_CHdata->trackPos;
          cout << '[';
          for (int t = 0; t < 13; ++t)
	  {
	    cout << hex << setw(2) << setfill('0') << (int)fc.fcBuf[tmp++] << ", ";
	    //cout << hex << setw(2) << setfill('0') << (int)fcBuf[tmp++] << ' ';
	  }
          cout << ']' << endl;
          //cout << endl;
        }

        // Get offset to (or address of) current pattern position.
        udword pattOffs = p_CHdata->pattStart + p_CHdata->pattPos;
        cout << '{' << hex << setw(2) << setfill('0') << (int)fc.fcBuf[pattOffs] << ", " 
	     << setw(2) << (int)fc.fcBuf[pattOffs+1];
        if (p_CHdata->dmaMask != 8)
	  cout << " | ";
        cout << '}';
      }
    }
    
    //fc.run();        
  }
  
  return 0;
}

void fc_chan_clear( fc_chan_t *fc_chan)
{
  fc_chan->pitch_transpose = 0;
  fc_chan->sound_transpose = 0;
  fc_chan->prev_fc_note   = 0;
  fc_chan->prev_midi_note = 0;
}

void fc_chan_config( fc_chan_t *fc_chan, int8_t pitch_transpose, int8_t sound_transpose )
{
  fc_chan->pitch_transpose = pitch_transpose;
  fc_chan->sound_transpose = sound_transpose;
}

void fc_chan_row_process( fc_chan_t *fc_chan, int8_t note, int8_t sound_num )
{
  int8_t midi_note_num;
  int8_t instr_num;
  
  switch( fc_chan->prev_fc_note )
  {
  case 0: // will make a note on
    midi_note_num  = fc_chan->pitch_transpose + note;
    instr_num      = fc_chan->sound_transpose + sound_num;
    cout << "note on, ch=" << instr_num << ", note=" << midi_note_num << endl;
    fc_chan->prev_fc_note   = note;
    fc_chan->prev_midi_note = midi_note_num;
  break;

  default:
    // make note off for old note,
    cout << "note off, ch=" << instr_num << ", note=" << fc_chan->prev_midi_note << endl;

    if( 0 != note )  // make note on for new one
    {
      midi_note_num  = fc_chan->pitch_transpose + note;
      instr_num      = fc_chan->sound_transpose + sound_num;
      cout << "note on, ch=" << instr_num << ", note=" << midi_note_num << endl;
      fc_chan->prev_fc_note   = note;
      fc_chan->prev_midi_note = midi_note_num;
    } 
  }
}

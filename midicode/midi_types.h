// Note: MIDI files use Big Endian format

typedef struct
{
  unsigned int Format;
  unsigned int NumTracks;
  unsigned int PPQN;
  unsigned long len;
} MIDIHdr;

#define NONE       0x00
#define NOTEOFF    0x80
#define NOTEON     0x90
#define AFTERTOUCH 0xA0
#define CONTROL    0xB0
#define PROGRAM    0xC0
#define PRESSURE   0xD0
#define PITCHWHEEL 0xE0

typedef struct
{
  unsigned int len;
  byte status;
  unsigned long len_offset;
  byte *data;
} MIDITrk;

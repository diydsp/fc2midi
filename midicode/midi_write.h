// General MIDI file functions
int midi_writeHdr(MIDIHdr *,FILE *);

// MIDI track operations
void midi_initTrk(MIDITrk *trk);
int midi_writeTrkHdr(FILE*,MIDITrk *mt);
int midi_writeTrkLen(FILE*,MIDITrk *mt);

// Useful
int midi_writeDeltaTime(FILE *fi,MIDITrk *trk,unsigned long val);

// writing MIDI Events
//int midi_writeNoteOn(FILE *,MIDITrk *trk,char ch,char notenum,char vel);

// writing special non-MIDI events
int midi_writeSeqNum(FILE *,MIDITrk *trk,unsigned int SeqNum);

#define TEXT       0x01
#define COPYRIGHT  0x02
#define SEQTRKNAME 0x03
#define INSTRUMENT 0x04
#define LYRIC      0x05
#define MARKER     0x06
#define CUEPOINT   0x07

int midi_writeText  (FILE *,MIDITrk *,char,char *);
int midi_writeMIDICh(FILE *,MIDITrk *,char);
int midi_writeMIDIPort(FILE *,MIDITrk *,char);
int midi_writeEndofTrk(FILE *,MIDITrk *);
int midi_writeTempo(FILE *,MIDITrk *,int);
int midi_writeTimeSig(FILE *,MIDITrk *,char,char,char,char);



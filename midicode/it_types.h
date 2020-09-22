// Note: IT files use Little Endian format

typedef struct {
  char blah[64];
} Itenv;

typedef struct {
  unsigned long offset;
  char DOSFileName[13];
  byte NNA,DCT,DCA;
  int FadeOut;
  byte PPS,PPC,GbV,DfP,RV,RP;
  int TrkVerse;
  byte NoS,x;
  char InstName[26];
  byte IFC,IFR,MCh,MPr;
  int MIDIBnk;
  byte Table[240];
  Itenv *env;
} Itinst;

typedef struct {
  unsigned long offset;
} Itsamp;

typedef struct {
  int number;  
  unsigned long offset;
  unsigned int length;
  int rows;
  byte *data;
} Itpatt;

typedef struct {
  char Name[26];
  unsigned int OrdNum,InsNum,SmpNum,PatNum;
  byte Cwty,Cwtx,Cmwty,Cmwtx;
  unsigned int Flags,Special;
  unsigned int GV,MV,IS,IT,Sep,PWD,MsgLgth,MsgOffset;
  byte pan[64],vol[64];
  byte *Orders;
  
  Itinst *inst;
  Itsamp *samp;
  Itpatt *patt;
} Ithdr;

int it_readhdr  (Ithdr  *, FILE *);
int it_readinst (Itinst *, FILE *);
int it_readsamp (Itsamp *, FILE *);
int it_readpatt (Itpatt *, FILE *);

void it_disphdr  (Ithdr  *);
void it_dispflags(unsigned char);
void it_dispspecial(unsigned char);
void it_dispinst (Itinst *);
void it_dispsamp (Itsamp *);
void it_disppatt (Itpatt *);



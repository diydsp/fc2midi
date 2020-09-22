ROOT=/home/haus/shifty

LIBDIR=$(ROOT)/lib
INCDIR=$(ROOT)/include

MIDIOBJS=midi_read.o midi_write.o midi_disp.o companies.o controlnum.o
ITOBJS=it_read.o it_write.o it_disp.o

IT2MIDIOBJS = it2midi.o $(MIDIOBJS) $(ITOBJS) main.o useful.o
MIDI2ITOBJS = midi2it.o $(MIDIOBJS) $(ITOBJS) main2.o useful.o 
EXAMIDIOBJS = examidi.o $(MIDIOBJS) main3.o useful.o

FLAGS = -c -g

# ------------------------------------
# Targets
# ------------------------------------
all:	midilib itlib it2midi midi2it examidi

clean:
	rm -f *.o

install:
	cp libmidifile.a     $(LIBDIR)
	cp libimpulsetracker $(LIBDIR)

midilib: $(MIDIOBJS)
	ar -cvr libmidifile.a $(MIDIOBJS)

itlib:   $(ITOBJS)
	ar -cvr libitfile.a $(ITOBJS)

it2midi: $(IT2MIDIOBJS)
	gcc $(IT2MIDIOBJS) -o it2midi

midi2it: $(MIDI2ITOBJS)
	gcc $(MIDI2ITOBJS) -o midi2it

examidi: $(EXAMIDIOBJS)
	gcc $(EXAMIDIOBJS) -o examidi

# ------------------------------------
# Impulse Tracker Objects
# ------------------------------------
it_read.o:
	gcc $(FLAGS) it_read.c

it_write.o:
	gcc $(FLAGS) it_write.c

it_disp.o:
	gcc $(FLAGS) it_disp.c

# ------------------------------------
# MIDI Objects
# ------------------------------------
midi_read.o:
	gcc $(FLAGS) midi_read.c

midi_write.o:
	gcc $(FLAGS) midi_write.c

midi_disp.o:
	gcc $(FLAGS) midi_disp.c

companies.o:
	gcc $(FLAGS) companies.c

controlnum.o:
	gcc $(FLAGS) controlnum.c

# ------------------------------------
# main functions
# ------------------------------------
it2midi.o:
	gcc $(FLAGS) it2midi.c

midi2it.o:
	gcc $(FLAGS) midi2it.c

examidi.o:
	gcc $(FLAGS) examidi.c

main.o:	
	gcc $(FLAGS) main.c 

main2.o:	
	gcc $(FLAGS) main2.c 

useful.o:
	gcc $(FLAGS) useful.c


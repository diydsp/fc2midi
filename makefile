ROOT=.

LIBDIR=$(ROOT)/lib
INCDIR=$(ROOT)/include

MIDI_DIR=midicode
MIDIOBJS=midi_read.o midi_write.o midi_disp.o companies.o controlnum.o
EXAMIDIOBJS = examidi.o $(MIDIOBJS) main3.o useful.o
FC2MIDIOBJS = fc2midi.o

FLAGS = -c -g

# ------------------------------------
# Targets
# ------------------------------------
all:	midilib examidi fc2midi

clean:
	rm -f *.o

install:
	cp libmidifile.a     $(LIBDIR)
	cp libimpulsetracker $(LIBDIR)

fc2midi: midilib fc2midi.o
	g++ $(FC2MIDIOBJS) libfc14audiodecoder-1.0.3/src/.libs/libfc14audiodecoder.a libmidifile.a -o fc2midi

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
# Future Composer Objects
# ------------------------------------
fc2midi.o: fc2midi.cpp
	gcc $(FLAGS) fc2midi.cpp


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
	gcc $(FLAGS) $(MIDI_DIR)/midi_read.cpp

midi_write.o:
	gcc $(FLAGS) $(MIDI_DIR)/midi_write.cpp

midi_disp.o:
	gcc $(FLAGS) $(MIDI_DIR)/midi_disp.cpp

companies.o:
	gcc $(FLAGS) $(MIDI_DIR)/companies.cpp

controlnum.o:
	gcc $(FLAGS) $(MIDI_DIR)/controlnum.cpp

# ------------------------------------
# main functions
# ------------------------------------
it2midi.o:
	gcc $(FLAGS) it2midi.c

midi2it.o:
	gcc $(FLAGS) midi2it.c

examidi.o:
	gcc $(FLAGS) $(MIDI_DIR)/examidi.cpp

main.o:	
	gcc $(FLAGS) main.c 

main2.o:	
	gcc $(FLAGS) main2.c 

main3.o:	
	gcc $(FLAGS) $(MIDI_DIR)/main3.cpp

useful.o:
	gcc $(FLAGS) $(MIDI_DIR)/useful.cpp


#
CC           = gcc
RANLIB       = ranlib

#set to the dir prefix for installation
DESTDIR      = /usr/local

#for unix, uncomment out next line and comment out the next next line
#define UNIX
DEFINES = -Dunix
#DEFINES  = -Dwin32

DEFINES += -DHAVE_MPG123

#uncomment the following to just compile the audio hash and read audio functions
#DEFINES += -DJUST_AUDIOHASH

CFLAGS	     = -fPIC -g -O3 -I. -I/usr/local/include
CPPFLAGS     = $(DEFINES)

LDFLAGS = -L. -L/usr/local/lib -pthread

HEADERS      = phash_audio.h audiodata.h

HASH_SOURCES = phash_audio.c fft.c complex.c
DATA_SOURCES = audiodata.c
SOURCES      = $(HASH_SOURCES) $(DATA_SOURCES)

OBJECTS      = $(SOURCES:.c=.o)
HASH_OBJECTS = $(HASH_SOURCES:.c=.o)
DATA_OBJECTS = $(DATA_SOURCES:.c=.o)

HASH_LIBRARY = libpHashAudio.a
DATA_LIBRARY = libAudioData.a

HASH_NAME = pHashAudio
DATA_NAME = AudioData

HASH_LIB = lib$(HASH_NAME).so
DATA_LIB = lib$(DATA_NAME).so

TABLEDIR     = ./table-4.3.0phmodified
TABLE        = libtable.a


DEPLIB       = $(TABLEDIR)/$(TABLE)
DEPS       = -L/usr/local/lib -lm -lsndfile -lsamplerate -lmpg123 -lzmq

UTILITY         = audioindex
UTILITY_SRC     = audio_index.c

TEST_INDEX     = audioindex_t
TEST_INDEX_SRC = test_index.c
TEST_DATA      = audiodata_t
TEST_DATA_SRC  = test_audiodata.c
SERVERS		= auscoutd tblservd metadatadb
DRIVERS		= mddriver

.PHONY : all

libs : $(HASH_LIB) $(DATA_LIB) $(HASH_LIBRARY) $(DATA_LIBRARY) 


%.d: %.c
	@set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's/\(.*\)\.o[ :]*/\1.o $@ : /g' < $@.$$$$ > $@; \
	rm -f $@.$$$$


include $(SOURCES:.c=.d)

install :  $(HASH_LIBRARY) $(DATA_LIBRARY) $(DEMO)
	$(RANLIB) $(HASH_LIBRARY)
	$(RANLIB) $(DATA_LIBRARY)
	install -c -m 0755 $(HEADERS) $(DESTDIR)/include
	install -c -m 0755 $(HASH_LIBRARY) $(DESTDIR)/lib
	install -c -m 0755 $(DATA_LIBRARY) $(DESTDIR)/lib
	install -c -m 0755 $(HASH_LIB) $(DESTDIR)/bin
	install -c -m 0755 $(DATA_LIB) $(DESTDIR)/bin
	install -c -m 0755 $(TABLEDIR)/$(TABLE) $(DESTDIR)/lib
	install -c -m 0755 $(DEMO)    $(DESTDIR)/bin

distr:
	install -c -m 0755 $(HEADERS) ./phashaudio
	install -c -m 0755 $(HASH_LIB) ./phashaudio
	install -c -m 0755 $(DATA_LIB) ./phashaudio
	install -c -m 0755 $(DEMO) ./phashaudio


uninstall :
	rm -f $(DESTDIR)/include/$(HEADERS)
	rm -f $(DESTDIR)/lib/$(HASH_LIBRARY)
	rm -f $(DESTDIR)/lib/$(DATA_LIBRARY)
	rm -f $(DESTDIR)/bin/$(HASH_DLL)
	rm -f $(DESTDIR)/bin/$(DATA_DLL)
	rm -f $(DESTDIR)/lib/$(TABLE)
	rm -f $(DESTDIR)/bin/$(DEMO)


$(HASH_LIBRARY): $(TABLEDIR)/table.o $(HASH_OBJECTS)
	$(AR) rc $@ $?
	$(RANLIB) $@

$(HASH_LIB) :$(TABLEDIR)/table.o  $(HASH_LIBRARY)
	$(CC) -o$(HASH_LIB) -shared -fPIC $(CFLAGS) $(CPPFLAGS) $(TABLEDIR)/table.o $(HASH_OBJECTS)


$(DATA_LIBRARY):$(DATA_OBJECTS)
	$(AR) rc $@ $?
	$(RANLIB) $@

$(DATA_LIB) : $(DATA_LIBRARY)
	$(CC) -o$(DATA_LIB) -shared -fPIC $(CFLAGS) $(CPPFLAGS) $(DATA_OBJECTS) $(DEPS)

$(TABLEDIR)/$(TABLE) :
	cd $(TABLEDIR) && make -f Makefile all


utility  : $(UTILITY)

$(UTILITY)  : $(HASH_LIB) $(DATA_LIB)
	$(CC) -o $@ $(UTILITY_SRC) $(CFLAGS) $(CPPFLAGS) -l$(HASH_NAME) -l$(DATA_NAME) $(LDFLAGS) -L/usr/local/lib $(DEP)

tests : $(TEST_INDEX) $(TEST_DATA)

$(TEST_INDEX) : $(HASH_LIB)  $(TEST_INDEX_SRC)
	$(CC) -o$@  $(TEST_INDEX_SRC) $(CFLAGS) $(CPPFLAGS)  -lm -l$(HASH_NAME) $(LDFLAGS) 

$(TEST_DATA): $(DATA_LIB) $(TEST_DATA_SRC)
	$(CC) -o$@ $(TEST_DATA_SRC) $(CFLAGS) $(CPPFLAGS) -l$(DATA_NAME) $(LDFLAGS)


$(SERVERS): $(HASH_LIB) $(DATA_LIB)
	$(CC) -ometadatadb metadatadb.c -g -O3 -lsqlite3 -lzmq -I. -I/usr/local/include -L. -L/usr/local/lib -pthread
	$(CC) -oauscoutd auscout.c -g -O3 -l$(DATA_NAME) -I. -I/usr/local/include -lzmq -lrt $(LDFLAGS) -pthread
	$(CC) -otblservd table_server.c -g -O3 -l$(HASH_NAME) -lzmq -I. -I/usr/local/include $(LDFLAGS) -pthread

servers	:$(SERVERS)

$(DRIVERS): $(SERVERS)
	$(CC) -omddriver metadatadb_driver.c -g -O3 -l$(DATA_NAME) -l$(HASH_NAME) -lzmq -lrt -I. -I/usr/local/include $(LDFLAGS) -pthread
	$(CC) -oauscdriver auscout-driver.c -g -O3 -l$(DATA_NAME) -l$(HASH_NAME) -lzmq -lrt -I. -I/usr/local/include $(LDFLAGS) -pthread -lm
	$(CC) -oausink auscout-sink.c -g -O3 -lzmq -lrt -I. -I/usr/local/include -L. $(LDFLAGS) -pthread
	$(CC) -otbldriver tblserver_driver.c -g -O3 -Wall -lAudioData -lpHashAudio -lzmq -lrt -I. -I/usr/local/include -L. $(LDFLAGS) -pthread -lm

drivers	:$(DRIVERS)


clients : $(SERVERS) $(UTILITY)
	gcc -oauscoutclient auscout-client.c -g -O3 -std=c99 -Wall -I. -L. -lAudioData -lpHashAudio -lzmq
	qmake -config release -o Makefile.clients auscout-client-main.pro
	make -f Makefile.clients

all	:drivers

.PHONY	: clean

clean:
	rm -f $(HASH_LIB) $(DATA_LIB) $(HASH_LIBRARY) $(DATA_LIBRARY) *.o $(DEMO) $(TEST_INDEX)\
               $(TEST_DATA) $(UTILITY) audioindextestfile
	rm -f auscoutd tblservd metadatadb
	rm -f mddriver auscdriver  tbldriver
	rm -f ausink
	rm -f Makefile.clients auscout
	rm -f auscoutclient
	cd $(TABLEDIR) && make clean




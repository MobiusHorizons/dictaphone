#dependencies for package 'dictaphone.c'
dictaphone.o: dictaphone.c rec.h filename.h cards.h opus.h

#dependencies for package 'rec.c'
rec.o: rec.c

#dependencies for package 'filename.c'
filename.o: filename.c

#dependencies for package 'cards.c'
LDLIBS += -lasound
cards.o: cards.c

#dependencies for package 'opus.c'
CFLAGS += -D_GNU_SOURCE
opus.o: opus.c

dictaphone: dictaphone.o rec.o filename.o cards.o opus.o
	$(CC) $(CFLAGS) $(LDFLAGS)  dictaphone.o rec.o filename.o cards.o opus.o -o dictaphone $(LDLIBS)

CLEAN_dictaphone:
	rm -rf dictaphone dictaphone.o rec.o filename.o cards.o opus.o

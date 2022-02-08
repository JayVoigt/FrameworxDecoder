CC=gcc
CFLAGS=-g -mmacosx-version-min=10.6
LDFLAGS=-L.
OBJ=main.o bmpWrite.o readFrame.o conversion.o

FrameworxDecoder: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) $(LDLIBS)

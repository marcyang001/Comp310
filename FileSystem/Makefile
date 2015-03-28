CFLAGS = -c -Wall `pkg-config fuse --cflags --libs` 

LDFLAGS = `pkg-config fuse --cflags --libs`

#SOURCES= disk_emu.c sfs_api.c fuse_wrappers.c
SOURCES= sfs_test.c disk_emu.c sfs_api.c sfs_api.h open.c remove.c read.c write.c 
#SOURCES= newsfs_test.c disk_emu.c sfs_api.c sfs_api.h open.c remove.c read.c write.c
#SOURCES=newTest.c disk_emu.c sfs_api.c sfs_api.h open.c remove.c write.c read.c write.c


OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=sfs

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	gcc $(OBJECTS) $(LDFLAGS) -o $@ -lm

.c.o:
	gcc $(CFLAGS) $< -o $@

clean:
	rm -rf *.o *~ $(EXECUTABLE) 

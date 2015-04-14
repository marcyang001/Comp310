CFLAGS = -c 
#-Wall `pkg-config fuse --cflags --libs` 



SOURCES= malloc_api.c malloc_api.h testing.c

OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=mymalloc

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	gcc $(OBJECTS) -o $@ -lm

.c.o:
	gcc $(CFLAGS) $< -o $@

clean:
	rm -rf *.o *~ $(EXECUTABLE) 

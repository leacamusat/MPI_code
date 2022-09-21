CC=mpicc
OUTPUT=gol
LIBS=../libs
CFLAGS=-O3 -Wall -Wextra -pedantic -g
LDFLAGS=-fsanitize=address -fsanitize=leak -fsanitize=undefined 
N_PROC=4
GOL=1
ENREGISTREMENT=0
N_PIXELS=4096
N_ITE=100

all: $(OUTPUT)

$(OUTPUT): automate.o lea_png.o libattopng.o
	$(CC) automate.o lea_png.o libattopng.o -o $(OUTPUT) $(LDFLAGS)

automate.o: automate.c automate.h ../common.h
	$(CC) $(CFLAGS) -c automate.c -o automate.o

lea_png.o: $(LIBS)/lea_png.c $(LIBS)/lea_png.h 
	$(CC) $(CFLAGS) -c $(LIBS)/lea_png.c -o lea_png.o

libattopng.o: $(LIBS)/libattopng.c $(LIBS)/libattopng.h
	$(CC) $(CFLAGS) -c $(LIBS)/libattopng.c -o libattopng.o


clean:
	rm -f *.o $(OUTPUT) ./video/*.png ./video/*.gif


rebuild: clean all

run: $(OUTPUT)
	mpirun --host master:4,worker1:4,worker2:4,worker3:4 --use-hwthread-cpus -np $(N_PROC) $(OUTPUT) $(GOL) $(ENREGISTREMENT) $(N_PIXELS) $(N_ITE)

generate_parity:
	cd video && mogrify -compress JPEG -quality 5 -path ./ parity*.png && convert -delay 50 -loop 0 parity*.png output_parity.gif

generate_GOL:
	cd video && mogrify -compress JPEG -quality 5 -path ./ GOL*.png && convert -delay 50 -loop 0 GOL*.png output_GOL.gif

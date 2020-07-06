CC=arm-arago-linux-gnueabi-gcc
#CC=arm-linux-gcc
CFLAG= -I. 
LFLAG= -lpthread
OBJS = esam.o

all:test_esam

test_esam:$(OBJS)
	$(CC) -o test_esam $(CFLAG) $(OBJS) $(LFLAG)

clean:
	rm -f *~
	rm -f *.o
	rm -f test_esam
	rm -f *.dat

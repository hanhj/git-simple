
CC=arm-arago-linux-gnueabi-gcc
#CC=
all: uart_test m72_test  m72
.PHONY : all
uart_test:uart_test.o
	$(CC) -o uart_test uart_test.o -lpthread
m72_test:m72_test.o
	$(CC) -o m72_test m72_test.o -lpthread
m72:m72.o
	$(CC) -o m72 m72.o -lpthread
clean:
	@rm -vf uart_test m72_test  m72 *.o

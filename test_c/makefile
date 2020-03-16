target=hello pipe test
all:$(target)
%.o:%.c
	gcc -c -g $< -o $@
objs=hello.o pipe.o test.o
hello:hello.o
pipe:pipe.o
test:test.o
clean:
	rm -f $(target) $(objs)

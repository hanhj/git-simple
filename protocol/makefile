target=test
all:$(target)
objs=pseudo.o
%.o:%.cpp
	g++ -g -c -Wall $< -o $@
$(target):$(objs)
	g++ $(objs) -o $@
help:
	doxygen
clean:
	rm -fr $(objs) $(target) html latex logfile


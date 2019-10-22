target=test
all:$(target)
objs=test.o protocol.o data.o Configurable.o com.o system.o utilty.o
%.o:%.cpp
	g++ -g -c -Wall $< -o $@
$(target):$(objs)
	g++ $(objs) -o $@
help:
	doxygen
clean:
	rm -fr $(objs) $(target) html latex logfile


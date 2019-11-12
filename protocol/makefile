target=test it 
all:$(target)
objs1=main.o Scada.o para.o protocol.o data.o configurable.o com.o system.o utilty.o
objs2=it.o
objs=$(objs1) $(objs2)
depend_file=$(objs:%.o=%.d)
#-include $(depend_file)
%.o:%.cpp
	g++ -g -c -Wall -std=c++0x  -fpermissive $< -o $@
%.d:%.cpp
	g++ -M $< -o $@

test:$(objs1)
	g++ $(objs1) -o $@
it:$(objs2)
	g++ $(objs2) -o $@
help:
	doxygen
clean:
	rm -fr $(objs) $(target) $(depend_file) html latex logfile


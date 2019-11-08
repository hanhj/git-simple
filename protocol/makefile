target=test
all:$(target)
objs=main.o Scada.o para.o protocol.o data.o configurable.o com.o system.o utilty.o
depend_file=$(objs:%.o=%.d)
-include $(depend_file)
%.o:%.cpp
	g++ -g -c -Wall  -fpermissive $< -o $@
%.d:%.cpp
	g++ -M $< -o $@

$(target):$(objs)
	g++ $(objs) -o $@
help:
	doxygen
clean:
	rm -fr $(objs) $(target) $(depend_file) html latex logfile


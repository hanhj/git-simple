target =test it 
all:$(target)
cover:coverage.info
html:coverage.info
	genhtml coverage.info -o html
objs1 =main.o Scada.o para.o gdw101_104.o data.o configurable.o com.o system.o utility.o profun.o
objs2 =it.o
objs  =$(objs1) 
objs  += $(objs2)
depend_file=$(objs:%.o=%.d)
-include $(depend_file)
cppflag=-g -c -Wall -std=c++0x -fpermissive -fprofile-arcs -ftest-coverage
ldflag=-fprofile-arcs -ftest-coverage
#为了生成coverage.info,需要先运行test程序,然后指向下面的语句.自动不行,需要手动敲入命令?
coverage.info:$(target)
	@echo "Please type this command:lcov  -d -c -o coverage.info -t test"
%.o:%.cpp
	g++ $(cppflag) $< -o $@
%.d:%.cpp
	g++ -M $< -o $@

test:$(objs1)
	g++ $(ldflag) $(objs1) -o $@
it:$(objs2)
	g++ $(ldflag) $(objs2) -o $@
help:
	doxygen
clean:
	rm -fr $(objs) $(target) $(depend_file) html latex logfile *.gcda *.gcno html coverage.info


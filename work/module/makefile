target=test
all:$(target)
objs=module.o	
cppflag=-g -c -Wall -std=c++0x -fpermissive -fprofile-arcs -ftest-coverage
cflag=-g -c -Wall  
ldflag=-fprofile-arcs -ftest-coverage
#为了生成coverage.info,需要先运行test程序,然后指向下面的语句.自动不行,需要手动敲入命令?
coverage.info:$(target)
	@echo "Please type this command:lcov  -d -c -o coverage.info -t test"
%.o:%.cpp
	g++ $(cppflag) $< -o $@
%.o:%.c
	gcc $(cflag) $< -o $@
%.d:%.cpp
	g++ -M $< -o $@

test:$(objs)
	gcc $^ -o $@ 

clean:
	rm -f $(target) $(objs)

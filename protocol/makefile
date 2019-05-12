target=test
all:$(target)
objs=pseudo.o
%.o:%.cpp
	g++ -g -c -Wall $< -o $@
$(target):$(objs)
	g++ $(objs) -o $@

clean:
	rm -f $(objs) $(target)


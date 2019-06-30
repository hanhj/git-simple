target=test
all:$(target)
objs=main.o test_list.o test_queue.o test_stack.o test_search.o test_sort.o  case.o data_type.o utils.o test_recurse.o
#depend=$(patsubst,%o,%d,$(objs))
depend=$(objs:%.o=%.d)
-include $(depend)
%.d:%.cpp
	g++ -MM $< -o $@
%.o:%.cpp
	g++ -c -g -Wall $< -o $@
$(target):$(objs)
	g++ -o $@ $^
	
.PHONY:clean
clean:
	rm -f $(target) $(objs) $(depend)
.PHONY:list 
list:
	@echo $(depend)

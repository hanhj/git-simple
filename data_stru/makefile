target=test test_ptr data_stu.pdf 
all:$(target)
objs=main.o test_list.o test_queue.o test_stack.o test_search.o test_sort.o  case.o data_type.o utils.o test_recurse.o test_tree.o test_graph.o
#depend=$(patsubst,%o,%d,$(objs))
depend=$(objs:%.o=%.d)
-include $(depend)
objs2=test_ptr.o
depend2=$(objs2:%.o=%.d)
-include $(depend2)
objs3=data_stu.tex
clear3=$(objs3:%.tex=%.log) $(objs3:%.tex=%.aux) $(objs3:%.tex=%.idx) $(objs3:%.tex=%.out) $(objs3:%.tex=%.toc) 

%.d:%.cpp
	g++ -MM $< -o $@
%.o:%.cpp
	g++ -c -g -Wall $< -o $@
test:$(objs)
	g++ -o $@ $^
test_ptr:$(objs2)
	g++ -o $@ $^
data_stu.pdf:$(objs3)
	xelatex $< && xelatex $<
.PHONY:clean
clean:
	rm -f $(target) $(objs) $(depend) $(objs) $(depend) $(clear3)
.PHONY:list 
list:
	@echo $(depend)
	@echo $(depend2)
	@echo $(clear3)


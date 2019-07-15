target=test data_stu.pdf
all:$(target)
objs=main.o test_list.o test_queue.o test_stack.o test_search.o test_sort.o  case.o data_type.o utils.o test_recurse.o test_tree.o test_graph.o
#depend=$(patsubst,%o,%d,$(objs))
depend=$(objs:%.o=%.d)
-include $(depend)
%.d:%.cpp
	g++ -MM $< -o $@
%.o:%.cpp
	g++ -c -g -Wall $< -o $@
test:$(objs)
	g++ -o $@ $^

data_stu.pdf:data_stu.tex
	xelatex $< && xelatex $<
.PHONY:clean
clean:
	rm -f $(target) $(objs) $(depend) data_stu.aux data_stu.idx data_stu.log data_stu.out data_stu.toc 
.PHONY:list 
list:
	@echo $(depend)

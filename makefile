subdir=latex_test fault_indicator
all:
	@for i in $(subdir);do	make -C $$i;done;
clean:
	@for i in $(subdir);do	make -C $$i clean;done;

subdir=latex_test fault_indicator data_stru
all:
	@for i in $(subdir);do	make -C $$i;done;
clean:
	@for i in $(subdir);do	make -C $$i clean;done;

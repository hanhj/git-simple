src=yaoqiu
target=$(src).pdf
all:$(target)
$(src).pdf:$(src).tex
	xelatex $<
clean:
	rm -f $(src).aux $(src).log $(src).out $(target) 

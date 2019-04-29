target=test.pdf 
all:$(target)
%.dvi:%.tex
	latex $<
%.ps:%.dvi
	dvips $<
%.pdf:%.ps
	ps2pdf $<
clean:
	rm -f *.fls *.idx *.ind *.dvi *.ps *.log *.toc *.aux $(target)


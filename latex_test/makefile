target=a.pdf 
all:$(target)
%.dvi:%.tex
	latex $<
%.ps:%.dvi
	dvips $<
a.pdf:a.ps
	ps2pdf $<
clean:
	rm -f *.dvi *.ps *.log *.toc *.aux $(target)


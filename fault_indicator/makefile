target=function.pdf
ifeq ($(xe),1)
	LATEX=xelatex
else
	LATEX=pdflatex 
endif
all:$(target)
function.pdf:function.tex
	$(LATEX) $< && $(LATEX) $<
clean:
	rm -f *.idx *.ind *.fls *.dvi *.ps *.log *.toc *.aux 


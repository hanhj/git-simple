target=function.pdf
all:$(target)
function.pdf:function.tex
	pdflatex $< && pdflatex $<
clean:
	rm -f *.idx *.ind *.fls *.dvi *.ps *.log *.toc *.aux $(target)


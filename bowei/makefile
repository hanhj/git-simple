target=note.docx note.html
all:$(target)
note.docx:note.md
note.html:note.md

%.docx:%.md	
	pandoc -f markdown  $< -o $@ -c dox.css
%.html:%.md	
	pandoc -f markdown  $< -o $@ -c dox.css
latex_study.pdf:latex_study.tex
	xelatex $<
%.pdf:%.dvi
	dvipdf $< 
%.dvi:%.tex
	latex $<  
	-makeindex $(basename $<).idx
	latex $<
.PHONY:clean
clean:
	rm -f $(target) *.out *.fls *.idx *.ind *.dvi *.ps *.log *.toc *.aux *.ilg *.lof *.lot *.html 

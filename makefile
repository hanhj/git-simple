#如果文中包括的图片为jpg格式，采用pdflatex来编译
#如果文中包括的图片为eps格式，采用latex -> dvips -> ps2pdf 来编译 
#latex 输入tex生成dvi文件，
#dvips 输入dvi生成ps文件，
#ps2pdf输入ps生成pdf文件。
all:a.pdf function.pdf
%.dvi:%.tex
	latex $<
%.ps:%.dvi
	dvips $<
a.pdf:a.ps
	ps2pdf $<
function.pdf:function.tex
	pdflatex $<
clean:
	rm -f *.dvi *.ps *.log *.toc *.aux


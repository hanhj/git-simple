target=test.pdf mathematics_study.html matlab_study.html
all:$(target)
%.html:%.md
	pandoc -f markdown $< -o $@ -c dox.css
test.pdf:test.tex
	xelatex $<
clean:
	rm -f $(target) test.log test.aux



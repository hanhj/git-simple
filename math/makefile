target=test.pdf mathematics_study.html
all:$(target)
mathematics_study.html:mathematics_study.md
	pandoc -f markdown $< -o $@ -c dox.css
test.pdf:test.tex
	xelatex $<
clean:
	rm -f $(target) test.log test.aux



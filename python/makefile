target=python_study.html
all:$(target)
%.html:%.md
	pandoc -f markdown -o $@ $< -c dox.css
clean:
	rm -f $(target)

target=kernel_code.html
all:$(target)
%.html:%.md
	pandoc $< -o $@ -c dox.css
clean:
	rm $(target)

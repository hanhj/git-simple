target=sysbios.html
all:$(target) 
$(target):sysbios.md
	pandoc $< -o $@ -c dox.css 
clean:
	rm $(target)

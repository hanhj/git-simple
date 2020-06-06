target=edma3.html
all:$(target) 
$(target):edma3.md
	pandoc $< -o $@ -c dox.css 
clean:
	rm $(target)

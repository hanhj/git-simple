target=需求规格书.html require.html
all:$(target)
%.html:%.md
	pandoc -f markdown $< -o $@ -c dox.css
clean:
	rm -f $(target)


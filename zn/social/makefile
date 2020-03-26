target=require.html feasibility.html cankao.html
all:$(target)
%.html:%.md
	pandoc -f markdown $< -o $@ -c dox.css
clean:
	rm -f $(target)


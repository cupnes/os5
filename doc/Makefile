SOURCE=$(shell ls *.org)
TARGET=$(SOURCE:%.org=%.txt)

all: $(TARGET)
	$(info $^)

%.txt: %.org
	emacs -Q $< --batch -f org-ascii-export-to-ascii

clean:
	rm -f *~ *.txt

.PHONY: clean

all: view

compile:
	pdflatex analysis.tex 
	mv analysis.pdf ../analysis.pdf

view: compile
	xdg-open ../analysis.pdf &

clean:
	rm *.aux *.log *.out

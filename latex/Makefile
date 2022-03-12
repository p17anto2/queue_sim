all: view

compile:
	pdflatex analysis.tex 
	mv analysis.pdf ../analysis.pdf

view: compile
	evince ../analysis.pdf &

clean:
	rm *.aux *.log *.out

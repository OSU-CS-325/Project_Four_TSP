#!/bin/bash

# I'm currently using TeX Live to convert the .tex files to PDF
# http://www.tug.org/texlive/

# Because we're using bibtex, it requires a somewhat silly compile
# order to ensure all references are built correctly
# http://tex.stackexchange.com/questions/204291/bibtex-latex-compiling
latex main
biber main
latex main
pdflatex main
mv main.pdf Project4_Group12_report.pdf

# Clean up after ourselves
rm main.aux main.bbl main.bcf main.blg main.dvi main.log main.run.xml
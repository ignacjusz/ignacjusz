#!/bin/bash


cd $( dirname "$0" )
cd img

for x in *.gv; do
	o=$( echo $x | sed -e 's/\.gv/.pdf/g' )
	dot -Tpdf $x -o $o
	echo $x '->' $o
done




#!/bin/bash

fdir="in_files"
x=`ls $fdir`

for f in $x
do
    if [[ "$f" == *.txt ]]; then
        file_name_base=`echo $f | cut -d '.' -f1`
        mv ${fdir}/${f} ${fdir}/${file_name_base}.foo
        echo `pwd ${fdir}/${f}.foo`/${file_name_base}.foo
    else
        echo `pwd $f`/${f}
    fi
    
done

#!/usr/bin/env bash

# for i in 15 ; do
for i in 35 55 75 95 115 135 150; do
    echo $i hMeV merging/digitising/clustering...
    for j in {5..9}; do
        j2=$( expr ${j} + 5 )
        echo -e "\tRuns" ${j}+${j2}
        cat merge-digitisation-nnc.xml  | sed "s/ENERGY_00000/${i}00/" \
                                        | sed "s/RUNID1_00/${j}/" \
                                        | sed "s/RUNID2_00/${j2}/" > "steering/mdcr_${i}_${j}+${j2}.xml"
        Marlin steering/mdcr_${i}_${j}+${j2}.xml 1>logs/mdcr_${i}_${j}+${j2}.log 2>logs/mdcr_${i}_${j}+${j2}.err &
    done
done

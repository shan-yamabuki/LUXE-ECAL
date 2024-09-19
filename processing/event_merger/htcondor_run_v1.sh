#!/usr/bin/env bash

for i in 35 ; do
# for i in 15 35 55 75 95 115 135 150 ; do
    echo ${i} "hMeV merging/digitising/clustering..."
    for j in {5..9}; do
        j2=$( expr ${j} + 5 )
        echo -e "\tRuns" ${j}+${j2}
        cat merge-digitisation-nnc_v1.xml  | sed "s/ENERGY_00000/${i}00/" \
                                        | sed "s/RUNID1_00/${j}/" \
                                        | sed "s/RUNID2_00/${j2}/" > "steering/mdcr_${i}00_${j}+${j2}.xml"
        cat merge-digitise-nnc.sh   | sed "s/ENERGY_00000/${i}00/" \
                                    | sed "s/RUNID1_00/${j}/" \
                                    | sed "s/RUNID2_00/${j2}/" > "steering/run_mdcr_${i}00_${j}+${j2}.sh"
        chmod +x steering/run_mdcr_${i}00_${j}+${j2}.sh
    done
    # To submit one job with five runs
    cat merge-digitisation-nnc.sub  | sed "s/ENERGY_00000/${i}00/" > "steering/mdcr_${i}00.sub"
    condor_submit steering/mdcr_${i}00.sub
done

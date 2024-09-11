#!/usr/bin/env bash

for i in 15 35 55 75 95 115 135 150; do
    echo $i hMeV pixelising...
    for j in {5..14}; do
        cat pixelisation.xml    | sed "s/ENERGY_00000/${i}00/" \
                                | sed "s/RUNID_00/${j}/" > "steering/pixelisation_${i}_${j}.xml"
        Marlin steering/pixelisation_${i}_${j}.xml 1>logs/pixelisation_${i}_${j}.log 2>logs/pixelisation_${i}_${j}.err &
    done
done

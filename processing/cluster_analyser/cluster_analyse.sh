#!/usr/bin/env bash

for i in 15 35 55 75 95 115 135 150; do
    cat cluster_analysis.xml | sed "s/00000/${i}00/" > "steering/nncluster_analysis_$i.xml"
    Marlin steering/nncluster_analysis_$i.xml 1>logs/nncluster_analysis_$i.log 2>logs/nncluster_analysis_$i.err &
done

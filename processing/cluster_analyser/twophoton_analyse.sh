#!/usr/bin/env bash

# for i in 15 35 55 75 95 115 135 150; do
for i in 35; do
    cat cluster_analysis.xml    | sed "s/ENERGY_00000/${i}00/" \
                                | sed "s/POSTFIX_00/10/" > "steering/twophoton_analysis_${i}_10.xml"
    Marlin steering/twophoton_analysis_${i}_10.xml --ClusterAnalysing.ClusterCollectionName=TwoPhotonClusters_strategy1 \
        1>logs/twophoton_analysis_${i}_10.log 2>logs/twophoton_analysis_${i}_10.err &
    cat cluster_analysis.xml    | sed "s/ENERGY_00000/${i}00/" \
                                | sed "s/POSTFIX_00/11/" > "steering/twophoton_analysis_${i}_11.xml"
    Marlin steering/twophoton_analysis_${i}_11.xml --ClusterAnalysing.ClusterCollectionName=TwoPhotonClusters_recluster_strategy1 \
        1>logs/twophoton_analysis_${i}_11.log 2>logs/twophoton_analysis_${i}_11.err &

    cat cluster_analysis.xml    | sed "s/ENERGY_00000/${i}00/" \
                                | sed "s/POSTFIX_00/20/" > "steering/twophoton_analysis_${i}_20.xml"
    Marlin steering/twophoton_analysis_${i}_20.xml --ClusterAnalysing.ClusterCollectionName=TwoPhotonClusters_strategy2 \
        1>logs/twophoton_analysis_${i}_20.log 2>logs/twophoton_analysis_${i}_20.err &
    cat cluster_analysis.xml    | sed "s/ENERGY_00000/${i}00/" \
                                | sed "s/POSTFIX_00/21/" > "steering/twophoton_analysis_${i}_21.xml"
    Marlin steering/twophoton_analysis_${i}_21.xml --ClusterAnalysing.ClusterCollectionName=TwoPhotonClusters_recluster_strategy2 \
        1>logs/twophoton_analysis_${i}_21.log 2>logs/twophoton_analysis_${i}_21.err &

    cat cluster_analysis.xml    | sed "s/ENERGY_00000/${i}00/" \
                                | sed "s/POSTFIX_00/30/" > "steering/twophoton_analysis_${i}_30.xml"
    Marlin steering/twophoton_analysis_${i}_30.xml --ClusterAnalysing.ClusterCollectionName=TwoPhotonClusters_strategy3 \
        1>logs/twophoton_analysis_${i}_30.log 2>logs/twophoton_analysis_${i}_30.err &
    cat cluster_analysis.xml    | sed "s/ENERGY_00000/${i}00/" \
                                | sed "s/POSTFIX_00/31/" > "steering/twophoton_analysis_${i}_31.xml"
    Marlin steering/twophoton_analysis_${i}_31.xml --ClusterAnalysing.ClusterCollectionName=TwoPhotonClusters_recluster_strategy3 \
        1>logs/twophoton_analysis_${i}_31.log 2>logs/twophoton_analysis_${i}_31.err &

    cat cluster_analysis.xml    | sed "s/ENERGY_00000/${i}00/" \
                                | sed "s/POSTFIX_00/40/" > "steering/twophoton_analysis_${i}_40.xml"
    Marlin steering/twophoton_analysis_${i}_40.xml --ClusterAnalysing.ClusterCollectionName=TwoPhotonClusters_strategy4 \
        1>logs/twophoton_analysis_${i}_40.log 2>logs/twophoton_analysis_${i}_40.err &
    cat cluster_analysis.xml    | sed "s/ENERGY_00000/${i}00/" \
                                | sed "s/POSTFIX_00/41/" > "steering/twophoton_analysis_${i}_41.xml"
    Marlin steering/twophoton_analysis_${i}_41.xml --ClusterAnalysing.ClusterCollectionName=TwoPhotonClusters_recluster_strategy4 \
        1>logs/twophoton_analysis_${i}_41.log 2>logs/twophoton_analysis_${i}_41.err &
done

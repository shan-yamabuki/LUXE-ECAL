#!/usr/bin/env bash

for i in 15 35 55 75 95 115 135 150; do
    cat resolution-batch.xml | sed "s/000000/$i/" > "resolution-$i.xml"
    Marlin resolution-$i.xml 1> resolution-$i.log 2>resolution-$i.err
    cat resolution-batch.xml | sed "s/PixelMIPCluster/PixelDigiCluster/" \
                             | sed "s/000000/$i/" > "resolution-digit-$i.xml"
    Marlin resolution-digit-$i.xml 1> resolution-digit-$i.log 2>resolution-digit-$i.err
done
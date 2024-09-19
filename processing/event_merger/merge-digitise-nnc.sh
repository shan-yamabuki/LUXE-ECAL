#!/usr/bin/env bash

j=$1


echo "Sourcing the following script: "
echo -e "\t/lhome/ific/h/huangsha/.setup_key4hep"
source /lhome/ific/h/huangsha/.setup_key4hep


j2=$( expr ${j} + 5 )
Marlin ${PWD}/steering/mdcr_ENERGY_00000_${j}+${j2}.xml


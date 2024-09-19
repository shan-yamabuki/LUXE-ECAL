#!/bin/bash

particle="e-"

# for particle in "pi-" "mu-" "gamma"; do
	for energy in {1..13..2}; do
		./generic_condor.sh $particle $energy.5 -N 1000 -r 50 -R 99 -gz "-1000" -ax "-0.035" -sx 20 -sy 20 --enable_MeV
		sleep 300
		./generic_condor.sh $particle $energy.5 -N 1000 -r 100 -R 149 -gz "-1000" -ax 0.035 -sx 20 -sy 20 --enable_MeV
		sleep 300
	done
	./generic_condor.sh $particle 15 -N 1000 -r 50 -R 99 -gz "-1000" -ax "-0.035" -sx 20 -sy 20 --enable_MeV
	sleep 300
	./generic_condor.sh $particle 15 -N 1000 -r 100 -R 149 -gz "-1000" -ax 0.035 -sx 20 -sy 20 --enable_MeV
# done

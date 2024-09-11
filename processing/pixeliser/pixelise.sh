#!/usr/bin/env bash

#RUN_PATH="/lustre/ific.uv.es/prj/gl/abehep.flc/LUXE/ddsim/electron/run_20240821"
RUN_PATH=$1

for i in $( ls -1 ${RUN_PATH}/*.slcio | xargs -n 1 basename )
do
     echo $i
     Marlin pixelisation.xml --global.LCIOInputFiles=${RUN_PATH}/$i --DSTOutput.LCIOOutputFile=${RUN_PATH}/../pixelised/pixelised_$i
done

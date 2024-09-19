#source /cvmfs/sw.hsf.org/key4hep/setup.sh
source /cvmfs/sw.hsf.org/key4hep/setup.sh -r 2024-04-12

export CPATH="/lhome/ific/h/huangsha/repos/luxestyle:${CPATH}"

for dll in $( ls /lhome/ific/h/huangsha/Applications/lib/*.so ); do
   export MARLIN_DLL="${dll}:${MARLIN_DLL}"
done

#echo ${CPATH}
#echo ${MARLIN_DLL}

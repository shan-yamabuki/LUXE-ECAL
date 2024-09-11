#source /cvmfs/sw.hsf.org/key4hep/setup.sh
source /cvmfs/sw.hsf.org/key4hep/setup.sh -r 2024-04-12

export CPATH="${HOME}/repos/luxestyle:${CPATH}"

for dll in $( ls ${HOME}/Applications/lib/*.so ); do
   export MARLIN_DLL="${dll}:${MARLIN_DLL}"
done

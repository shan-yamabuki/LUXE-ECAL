#source /cvmfs/sw.hsf.org/key4hep/setup.sh
source /cvmfs/sw.hsf.org/key4hep/setup.sh -r 2024-04-12
export LUXEECAL_REPO="/lhome/ific/h/huangsha/repos/SiWECAL"
export MARLIN_DLL="${LUXEECAL_REPO}/PixelizationProcessor/lib/libPixelizationProcessor.so:${MARLIN_DLL}"


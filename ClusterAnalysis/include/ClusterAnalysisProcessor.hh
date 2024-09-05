#ifndef ClusterAnalysisProcessor_h
#define ClusterAnalysisProcessor_h 1
#include <iomanip>
#include <EVENT/LCRelation.h>
#include "marlin/Processor.h"
#include <EVENT/LCCollection.h>
#include <EVENT/MCParticle.h>
#include <EVENT/SimCalorimeterHit.h>
#include <EVENT/ReconstructedParticle.h>
#include <IMPL/LCCollectionVec.h>
#include <IMPL/TrackImpl.h>
#include <IMPL/TrackStateImpl.h>
#include <EVENT/Track.h>
#include <EVENT/Vertex.h>
#include <IMPL/ReconstructedParticleImpl.h>
#include <IMPL/LCRelationImpl.h>
#include <UTIL/LCRelationNavigator.h>
#include <UTIL/PIDHandler.h>
#include <UTIL/CellIDDecoder.h>
#include <cmath>
#include <string>
#include <vector>
// ----- include for verbosity dependent logging ---------
#include "marlin/VerbosityLevels.h"
#include "marlin/StringParameters.h"
#define SLM streamlog_out(MESSAGE)

#include <TF1.h>
#include <TRandom.h>
#include <TFile.h>
#include <TTree.h>
// #include <TLorentzVector.h>
using namespace lcio;
using namespace marlin;
using namespace std;

class ClusterAnalysisProcessor : public Processor {

    public:
        virtual Processor *newProcessor() { return new ClusterAnalysisProcessor; }
        
        ClusterAnalysisProcessor();
        virtual ~ClusterAnalysisProcessor();
        
        /** Called at the begin of the job before anything is read.
         */
        virtual void init();
        
        /** Called for every run.
         */
        virtual void processRunHeader(LCRunHeader *run);
        
        /** Called for every event - the working horse.
         */
        virtual void processEvent(LCEvent *evt);
        
        virtual void check(LCEvent *evt);
        
        virtual void end();
    
    private:
        virtual void ShowMCInfo(LCCollection *col);
        virtual void ShowECALInfo(LCCollection *col);
        virtual void ShowECALpixelInfo(LCCollection *col);
        virtual void ShowClusterInfo(LCCollection *col);
        
        std::string _MCColName;
        std::string _ECALColName;
        std::string _ECALpColName;
        std::string _NNClusterColName;
        std::string _ABClusterColName;
};

#endif

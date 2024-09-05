#ifndef ClusterAnalysisProcessor_h
#define ClusterAnalysisProcessor_h 1
#include "marlin/Processor.h"
#include <EVENT/LCCollection.h>
#include <EVENT/MCParticle.h>
#include <EVENT/Cluster.h>
#include <UTIL/CellIDDecoder.h>

#include <Math/Point3D.h>
#include <Math/Vector3D.h>
#include <string>
#include <vector>

// #include <EVENT/LCRelation.h>
// #include <EVENT/ReconstructedParticle.h>
// #include <IMPL/LCCollectionVec.h>
// #include <IMPL/TrackImpl.h>
// #include <IMPL/TrackStateImpl.h>
// #include <EVENT/Track.h>
// #include <EVENT/Vertex.h>
// #include <IMPL/ReconstructedParticleImpl.h>
// #include <IMPL/LCRelationImpl.h>
// #include <UTIL/LCRelationNavigator.h>
// #include <UTIL/PIDHandler.h>
// #include <iomanip>
// #include <cmath>
// ----- include for verbosity dependent logging ---------
#include "marlin/VerbosityLevels.h"
#include "marlin/StringParameters.h"
#define SLM streamlog_out(MESSAGE)

// #include <TF1.h>
// #include <TRandom.h>
// #include <TFile.h>
// #include <TTree.h>
// #include <TLorentzVector.h>
using namespace lcio;
using namespace marlin;
using namespace std;

class ClusterAnalysisProcessor : public Processor {
private:
    double LINEARITY_MIP_to_GeV = 0.0141;
    double LINEARITY_Edep_to_GeV = 96;
    
    virtual void GetMCInfo(LCCollection *col);
    virtual void GetClusterInfo(LCCollection *col);
    
    string _MCColName;
    string _ClusterColName;
    
    vector<ROOT::Math::XYZPoint> mc_vertices;
    vector<ROOT::Math::XYZVector> mc_momentums;
    vector<double> mc_energies;
    vector<ROOT::Math::XYZVector> cl_directions;
    vector<double> cl_energies;

public:
    virtual Processor *newProcessor() { return new ClusterAnalysisProcessor; }
    
    ClusterAnalysisProcessor();
    virtual ~ClusterAnalysisProcessor();
    
    /* Called at the begin of the job before anything is read. */
    virtual void init();
    
    /* Called for every run. */
    virtual void processRunHeader(LCRunHeader *run);
    
    /* Called for every event - the working horse. */
    virtual void processEvent(LCEvent *evt);
    
    virtual void check(LCEvent *evt);
    
    /* Called at the end of the job to write out things. */
    virtual void end();
};

#endif

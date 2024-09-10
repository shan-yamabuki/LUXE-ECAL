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

#ifdef MARLIN_AIDA //AIDA
#include <marlin/AIDAProcessor.h>
#endif
#include <TH1.h>
#include <TH2.h>
#include <TTree.h>
#include <TMath.h>
// #include <TRandom.h>
// #include <TFile.h>
// #include <TLorentzVector.h>
using namespace lcio;
using namespace marlin;
using namespace std;

class ClusterAnalysisProcessor : public Processor {
private:
    float LINEARITY_MIP_to_GeV = 0.0141;
    // float LINEARITY_Edep_to_GeV = 96;
    float ECAL_Z[2] = {0, 15*15};//mm
    float ECAL_X[2] = {-180, +180};//mm
    float ECAL_Y[2] = {-90, +90};//mm
    float radiusOverSigma = 2.0;//Fitting range == mean +- radius
    
    virtual void GetMCInfo(LCCollection *col);
    virtual void GetClusterInfo(LCCollection *col);
    
    string _MCColName;
    string _ClusterColName;
    
    vector<ROOT::Math::XYZPoint> mc_vertices, mc_positions;
    vector<ROOT::Math::XYZVector> mc_directions;
    vector<double> mc_energies;
    vector<ROOT::Math::XYZPoint> cl_vertices, cl_positions;
    vector<ROOT::Math::XYZVector> cl_directions;
    vector<double> cl_energies;

    float runEnergy = -1;
    TH1* _runEnergy;
    TH1 *_xCluster, *_yCluster, *_zCluster;
    TH2 *_xyCluster, *_zxCluster, *_zyCluster;
    TH1 *_xResidue, *_yResidue;
    vector<double> _xResidueVec, _yResidueVec;
    TH2 *_xyResidue;
    TH1 *_axCluster, *_ayCluster, *_azCluster;
    TH1 *_cosResidue;
    TH1 *_emCluster;
    vector<double> _emClusterVec;
    // TH1 *_eeCluster, *_eeResidue;

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

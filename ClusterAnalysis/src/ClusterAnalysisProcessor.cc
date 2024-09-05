#include "ClusterAnalysisProcessor.hh"

// ----- include for verbosity dependent logging ---------
// #include "marlin/VerbosityLevels.h"
// #include "marlin/StringParameters.h"
// #define SLM streamlog_out(MESSAGE)

// using namespace std;
// using namespace lcio ;
// using namespace marlin ;
using EVENT::LCCollection;
using EVENT::MCParticle;
using EVENT::ReconstructedParticle;
using EVENT::Track;
using EVENT::Vertex;
using IMPL::LCRelationImpl;
using IMPL::ReconstructedParticleImpl;
using IMPL::TrackImpl;
using IMPL::TrackStateImpl;
using std::string;
using std::vector;
using UTIL::LCRelationNavigator;

ClusterAnalysisProcessor aClusterAnalysisProcessor;

ClusterAnalysisProcessor::ClusterAnalysisProcessor() : Processor("ClusterAnalysisProcessor") {

    // modify processor description
    _description = "";

    // input collections
    registerInputCollection(LCIO::MCPARTICLE,"MCCollectionName",
                            "MCParticle Collection",
                            _MCColName,
                            std::string("MCParticle"));
    // registerInputCollection(LCIO::SIMCALORIMETERHIT,"ECALCollectionName",
    //                         "Sim ECAL Monolithic Collection",
    //                         _ECALColName,
    //                         std::string("SiEcalCollection"));
    // registerInputCollection(LCIO::SIMCALORIMETERHIT,"PixelisedECALCollectionName",
    //                         "Sim ECAL Pixelised Collection",
    //                         _ECALpColName,
    //                         std::string("PixelSiEcalCollection"));
    registerInputCollection(LCIO::CLUSTER,"NNClusters",
                            "Nearest-Neighbour Clustering Collection",
                            _NNClusterColName,
                            std::string("NNClusters"));
    registerInputCollection(LCIO::CLUSTER,"CluAB_1st",
                            "Arbor Clustering Collection",
                            _ABClusterColName,
                            std::string("CluAB_1st"));
    if (_ABClusterColName == "CluAB_1st") {streamlog_out(MESSAGE) << "Hello world" << "1" <<endl;}
    if (_ABClusterColName == "CluAB_2nd") {streamlog_out(MESSAGE) << "Hello world" << "0" <<endl;}
}

ClusterAnalysisProcessor::~ClusterAnalysisProcessor() {}

void ClusterAnalysisProcessor::init() {
    streamlog_out(MESSAGE) << "Playground initialised!";
    printParameters();
}


void ClusterAnalysisProcessor::ShowMCInfo(EVENT::LCCollection *myCollection) {
    int number = myCollection->getNumberOfElements();
  
    for (int i = 0; i < number; i++) {

        MCParticle *particle = dynamic_cast<MCParticle *>(myCollection->getElementAt(i));
        vector<MCParticle *> daughters = particle->getDaughters();
    
        streamlog_out(MESSAGE) << "\n MCCollection, particle:" << i;
        streamlog_out(MESSAGE) << " pdg=" << particle->getPDG();
        streamlog_out(MESSAGE) << " satus=" << particle->getGeneratorStatus();
        streamlog_out(MESSAGE) << " Ndaughters=" << daughters.size();
        streamlog_out(MESSAGE) << " E=" << particle->getEnergy();
        streamlog_out(MESSAGE) << " px=" << particle->getMomentum()[0];
        streamlog_out(MESSAGE) << " py=" << particle->getMomentum()[1];
        streamlog_out(MESSAGE) << " pz=" << particle->getMomentum()[2];
        streamlog_out(MESSAGE) << " m=" << particle->getMass();
        streamlog_out(MESSAGE) << " charge=" << particle->getCharge();

    }

}

void ClusterAnalysisProcessor::ShowECALInfo(EVENT::LCCollection *myCollection) {
    int number = myCollection->getNumberOfElements();
    CellIDDecoder<EVENT::SimCalorimeterHit> cd(myCollection);
    float total_energy = 0;

    for (int i = 0; i < number; i++) {

        SimCalorimeterHit *ecalhit = dynamic_cast<SimCalorimeterHit *>(myCollection->getElementAt(i));

        int xyz_x = cd(ecalhit)["x"];
        int xyz_y = cd(ecalhit)["y"];
        int xyz_z = cd(ecalhit)["layer"];

        streamlog_out(MESSAGE) << "\n SimCalorimeterHit, :" << i;
        streamlog_out(MESSAGE) << " cellID-encoded=" << ecalhit->getCellID0();
        streamlog_out(MESSAGE) << " x=" << xyz_x;
        streamlog_out(MESSAGE) << " y=" << xyz_y;
        streamlog_out(MESSAGE) << " z=" << xyz_z;
        streamlog_out(MESSAGE) << " energy=" << ecalhit->getEnergy();
        total_energy+=ecalhit->getEnergy();

    }
    streamlog_out(MESSAGE) << "\nTotal energy deposited " << total_energy << "GeV";

}

void ClusterAnalysisProcessor::ShowECALpixelInfo(EVENT::LCCollection *myCollection) {
    int number = myCollection->getNumberOfElements();
    CellIDDecoder<EVENT::SimCalorimeterHit> cd(myCollection);
    float total_energy = 0;

    for (int i = 0; i < number; i++) {

        SimCalorimeterHit *ecalhit = dynamic_cast<SimCalorimeterHit *>(myCollection->getElementAt(i));

        int IJK_I = cd(ecalhit)["I"];
        int IJK_J = cd(ecalhit)["J"];
        int IJK_K = cd(ecalhit)["K"];

        streamlog_out(MESSAGE) << "\n SimCalorimeterHit, :" << i;
        streamlog_out(MESSAGE) << " cellID-encoded=" << ecalhit->getCellID0();
        streamlog_out(MESSAGE) << " I=" << IJK_I;
        streamlog_out(MESSAGE) << " J=" << IJK_J;
        streamlog_out(MESSAGE) << " K=" << IJK_K;
        streamlog_out(MESSAGE) << " energy=" << ecalhit->getEnergy();
        total_energy+=ecalhit->getEnergy();

    }
    streamlog_out(MESSAGE) << "\nTotal energy deposited " << total_energy << "GeV";

}

void ClusterAnalysisProcessor::ShowClusterInfo(EVENT::LCCollection *myCollection) {
    int number = myCollection->getNumberOfElements();

    for (int i = 0; i < number; i++) {

        // MCParticle *particle = dynamic_cast<MCParticle *>(myCollection->getElementAt(i));
        // vector<MCParticle *> daughters = particle->getDaughters();
    
        // streamlog_out(MESSAGE) << "\n MCCollection, particle:" << i;
        // streamlog_out(MESSAGE) << " pdg=" << particle->getPDG();
        // streamlog_out(MESSAGE) << " satus=" << particle->getGeneratorStatus();
        // streamlog_out(MESSAGE) << " Ndaughters=" << daughters.size();
        // streamlog_out(MESSAGE) << " E=" << particle->getEnergy();
        // streamlog_out(MESSAGE) << " px=" << particle->getMomentum()[0];
        // streamlog_out(MESSAGE) << " py=" << particle->getMomentum()[1];
        // streamlog_out(MESSAGE) << " pz=" << particle->getMomentum()[2];
        // streamlog_out(MESSAGE) << " m=" << particle->getMass();
        // streamlog_out(MESSAGE) << " charge=" << particle->getCharge() << "\n";

    }
    streamlog_out(MESSAGE) << "Found " << number+2 << " Clusters!\n";

}

void ClusterAnalysisProcessor::processRunHeader(LCRunHeader *run) {
}

void ClusterAnalysisProcessor::processEvent(LCEvent *evt) {

    try {
        streamlog_out(MESSAGE) << "Showing MC Info \n ----------------------------------------- ";
        LCCollection *mccol = evt->getCollection(_MCColName);
        ShowMCInfo(mccol);
        streamlog_out(MESSAGE) << "\n";
        
        // streamlog_out(MESSAGE) << "Showing ECAL Info \n ----------------------------------------- ";
        // LCCollection *ecal = evt->getCollection(_ECALColName);
        // ShowECALInfo(ecal);
        // streamlog_out(MESSAGE) << "\n";
        // LCCollection *ecalp = evt->getCollection(_ECALpColName);
        // ShowECALpixelInfo(ecalp);
        // streamlog_out(MESSAGE) << "\n";
        
        streamlog_out(MESSAGE) << "Showing NN Cluster Info \n ----------------------------------------- ";
        LCCollection *cluster = evt->getCollection(_NNClusterColName);
        ShowClusterInfo(cluster);
        streamlog_out(MESSAGE) << "\n";
        streamlog_out(MESSAGE) << "Showing Arbor Cluster Info \n ----------------------------------------- ";
        LCCollection *abcluster = evt->getCollection(_ABClusterColName);
        ShowClusterInfo(abcluster);
        streamlog_out(MESSAGE) << "\n";

    
    } catch (DataNotAvailableException &e) {
        streamlog_out(MESSAGE) << "Whoops!....\n";
        streamlog_out(MESSAGE) << e.what();
    }
}

void ClusterAnalysisProcessor::check(LCEvent * evt) {
    // nothing to check here - could be used to fill checkplots in reconstruction processor
}

void ClusterAnalysisProcessor::end() {
}

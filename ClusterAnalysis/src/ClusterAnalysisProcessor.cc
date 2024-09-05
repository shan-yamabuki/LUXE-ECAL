#include "ClusterAnalysisProcessor.hh"

using EVENT::LCCollection;
using EVENT::MCParticle;
// using EVENT::ReconstructedParticle;
// using EVENT::Track;
// using EVENT::Vertex;
// using IMPL::LCRelationImpl;
// using IMPL::ReconstructedParticleImpl;
// using IMPL::TrackImpl;
// using IMPL::TrackStateImpl;
// using std::string;
// using std::vector;
// using UTIL::LCRelationNavigator;

ClusterAnalysisProcessor aClusterAnalysisProcessor;

ClusterAnalysisProcessor::ClusterAnalysisProcessor() : Processor("ClusterAnalysisProcessor") {
    // modify processor description
    _description = "Cluster Analysis Processor";

    // input collections
    registerInputCollection(LCIO::MCPARTICLE,"MCCollectionName",
                            "Primary Particle Collection",
                            _MCColName,
                            std::string("MCParticle"));
    registerInputCollection(LCIO::CLUSTER,"ClusterCollectionName",
                            "Cluster Collection",
                            _ClusterColName,
                            std::string("CluAB_1st"));
    // registerInputCollection(LCIO::CLUSTER,"CluAB_1st",
    //                         "Arbor Clustering Collection",
    //                         _ABClusterColName,
    //                         std::string("CluAB_1st"));
}

ClusterAnalysisProcessor::~ClusterAnalysisProcessor() {}

void ClusterAnalysisProcessor::init() {
    streamlog_out(MESSAGE) << "Playground initialised!";
    printParameters();
    // if (_ABClusterColName == "CluAB_1st") {streamlog_out(MESSAGE) << "Hello world" << "1" <<endl;}
    // if (_ABClusterColName == "CluAB_2nd") {streamlog_out(MESSAGE) << "Hello world" << "0" <<endl;}
}


void ClusterAnalysisProcessor::GetMCInfo(EVENT::LCCollection *myCollection) {
    int number = myCollection->getNumberOfElements();
    streamlog_out(MESSAGE) << "Expecting " << number <<" particles in this event." <<endl;
    mc_vertices.clear(); mc_momentums.clear();
    
    for (int i = 0; i < number; i++) {
        MCParticle *particle = dynamic_cast<MCParticle *>(myCollection->getElementAt(i));
        // vector<MCParticle *> daughters = particle->getDaughters();
        const double* mc_vertex = particle->getVertex();
        const double* mc_momentum = particle->getMomentum();
        ROOT::Math::XYZVector mc_momentum_p = ROOT::Math::XYZVector(mc_momentum[0], mc_momentum[1], mc_momentum[2]);
        double mc_energy = particle->getEnergy();
        mc_vertices.push_back(ROOT::Math::XYZPoint(mc_vertex[0], mc_vertex[1], mc_vertex[2]));
        mc_momentums.push_back(mc_momentum_p);
        mc_energies.push_back(mc_energy);
        
        streamlog_out(MESSAGE) << "MC Particle " << i+1 <<"/"<< number <<":";
        streamlog_out(MESSAGE) << " pdg = " << particle->getPDG() <<",";
        streamlog_out(MESSAGE) << " m = " << particle->getMass() <<" GeV,";
        streamlog_out(MESSAGE) << " charge = " << particle->getCharge() <<",";
        streamlog_out(MESSAGE) << " status = " << particle->getGeneratorStatus() <<",";
        streamlog_out(MESSAGE) << " N_daughters = " << (particle->getDaughters()).size() <<",";
        streamlog_out(MESSAGE) <<endl;
        streamlog_out(MESSAGE) << "Shooting from :";
        streamlog_out(MESSAGE) << " vx = " << mc_vertex[0] <<",";
        streamlog_out(MESSAGE) << " vy = " << mc_vertex[1] <<",";
        streamlog_out(MESSAGE) << " vz = " << mc_vertex[2] <<" mm,";
        streamlog_out(MESSAGE) <<endl;
        streamlog_out(MESSAGE) << "With direction:";
        streamlog_out(MESSAGE) << " ax = " << mc_momentum[0]/mc_momentum_p.R() <<",";
        streamlog_out(MESSAGE) << " ay = " << mc_momentum[1]/mc_momentum_p.R() <<",";
        streamlog_out(MESSAGE) << " az = " << mc_momentum[2]/mc_momentum_p.R() <<";";
        streamlog_out(MESSAGE) << " E = " << mc_energy <<" GeV.";
        streamlog_out(MESSAGE) <<endl;
    }
}

void ClusterAnalysisProcessor::GetClusterInfo(EVENT::LCCollection *myCollection) {
    int number = myCollection->getNumberOfElements();
    streamlog_out(MESSAGE) << "Reconstructed " << number <<" clusters in this event." <<endl;

    for (int i = 0; i < number; i++) {
        Cluster *cluster = dynamic_cast<Cluster *>(myCollection->getElementAt(i));
        double cl_energy = cluster->getEnergy() * LINEARITY_MIP_to_GeV;
        const float* cl_position = cluster->getPosition();
        double cl_theta = cluster->getITheta();
        double cl_phi = cluster->getIPhi();
        auto cl_direction = new ROOT::Math::Polar3DVector(1, cl_theta, cl_phi);
        cl_energies.push_back(cl_energy);
        cl_directions.push_back(ROOT::Math::XYZVector(cl_direction->X(), cl_direction->Y(), cl_direction->Z()));
        
        streamlog_out(MESSAGE) << "Cluster " << i+1 <<"/"<< number <<":";
        streamlog_out(MESSAGE) << " cx = " << cl_position[0] <<",";
        streamlog_out(MESSAGE) << " cy = " << cl_position[1] <<",";
        streamlog_out(MESSAGE) << " cz = " << cl_position[2] <<" mm,";
        streamlog_out(MESSAGE) << " E = " << cl_energy << " GeV,";
        streamlog_out(MESSAGE) << " ax = " << cl_direction->X();
        streamlog_out(MESSAGE) << " ay = " << cl_direction->Y();
        streamlog_out(MESSAGE) << " az = " << cl_direction->Z();
        streamlog_out(MESSAGE) <<endl;
    }
}

void ClusterAnalysisProcessor::processRunHeader(LCRunHeader *run) {
}

void ClusterAnalysisProcessor::processEvent(LCEvent *evt) {

    try {
        streamlog_out(MESSAGE) << "Showing MC Info \n ----------------------------------------- ";
        LCCollection *mccol = evt->getCollection(_MCColName);
        GetMCInfo(mccol);
        streamlog_out(MESSAGE) << "\n";
        
        // streamlog_out(MESSAGE) << "Showing ECAL Info \n ----------------------------------------- ";
        // LCCollection *ecal = evt->getCollection(_ECALColName);
        // GetECALInfo(ecal);
        // streamlog_out(MESSAGE) << "\n";
        // LCCollection *ecalp = evt->getCollection(_ECALpColName);
        // GetECALpixelInfo(ecalp);
        // streamlog_out(MESSAGE) << "\n";
        
        // streamlog_out(MESSAGE) << "Showing NN Cluster Info \n ----------------------------------------- ";
        // LCCollection *cluster = evt->getCollection(_NNClusterColName);
        // GetClusterInfo(cluster);
        // streamlog_out(MESSAGE) << "\n";
        streamlog_out(MESSAGE) << "Showing Arbor Cluster Info \n ----------------------------------------- ";
        LCCollection *cluster = evt->getCollection(_ClusterColName);
        GetClusterInfo(cluster);
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

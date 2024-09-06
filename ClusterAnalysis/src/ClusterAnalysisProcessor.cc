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
    AIDAProcessor::tree(this);
    printParameters();
    // if (_ABClusterColName == "CluAB_1st") {streamlog_out(DEBUG) << "Hello world" << "1" <<endl;}
    // if (_ABClusterColName == "CluAB_2nd") {streamlog_out(DEBUG) << "Hello world" << "0" <<endl;}

    _xCluster = new TH1F("cluster_x", "Reconstructed cluster centre; x [mm];", 180, ECAL_X[0], ECAL_X[1]);
    _yCluster = new TH1F("cluster_y", "Reconstructed cluster centre; y [mm];",  90, ECAL_Y[0], ECAL_Y[1]);
    _zCluster = new TH1F("cluster_z", "Reconstructed cluster centre; z [mm];",  60, ECAL_Z[0], ECAL_Z[1]);
    _xyCluster = new TH2F("cluster_xy", "Reconstructed cluster center; x [mm]; y [mm]", 90, ECAL_X[0], ECAL_X[1], 45, ECAL_Y[0], ECAL_Y[1]);
    _zxCluster = new TH2F("cluster_zx", "Reconstructed cluster center; z [mm]; x [mm]", 30, ECAL_Z[0], ECAL_Z[1], 90, ECAL_X[0], ECAL_X[1]);
    _zyCluster = new TH2F("cluster_zy", "Reconstructed cluster center; z [mm]; y [mm]", 30, ECAL_Z[0], ECAL_Z[1], 45, ECAL_Y[0], ECAL_Y[1]);
    _xResidue = new TH1D("residue_x", "Position residue at front; #Deltax [mm];", 40, -10, 10); // yes, no space. Weird!!
    _yResidue = new TH1D("residue_y", "Position residue at front; #Deltay [mm];", 40, -10, 10);
    _xyResidue = new TH2D("residue_xy", "Position residue at front; #Deltax [mm]; #Deltay [mm]", 40, -10, 10, 40, -10, 10);

    _axCluster = new TH1F("cluster_ax", "Reconstructed direction vector; px/p;", 200, -1, 1);
    _ayCluster = new TH1F("cluster_ay", "Reconstructed direction vector; py/p;", 200, -1, 1);
    _azCluster = new TH1F("cluster_az", "Reconstructed direction vector; pz/p;", 200, -1, 1);
    _cosResidue = new TH1F("residue_cos", "Angle cosine; cos(#alpha);", 100, 0, 1);

    _eeCluster = new TH1F("cluster_energy", "Reconstructed energy; E [GeV];", 200, 0, 20);
    _eeResidue = new TH1F("residue_energy", "Energy residue; #DeltaE [GeV];", 100, -5, 5);
}


void ClusterAnalysisProcessor::GetMCInfo(EVENT::LCCollection *myCollection) {
    int number = myCollection->getNumberOfElements();
    streamlog_out(DEBUG) << "Expecting " << number <<" particles in this event." <<endl;
    mc_vertices.clear(); mc_positions.clear(); mc_directions.clear(); mc_energies.clear();
    
    for (int i = 0; i < number; i++) {
        MCParticle *particle = dynamic_cast<MCParticle *>(myCollection->getElementAt(i));
        // vector<MCParticle *> daughters = particle->getDaughters();
        const double* mc_vertex = particle->getVertex();
        auto mc_vertex_x0 = ROOT::Math::XYZPoint(mc_vertex[0], mc_vertex[1], mc_vertex[2]);
        const double* mc_momentum = particle->getMomentum();
        auto mc_direction = ROOT::Math::XYZVector(mc_momentum[0], mc_momentum[1], mc_momentum[2]);
        mc_direction /= mc_direction.R();
        ROOT::Math::XYZPoint mc_position_x = mc_vertex_x0 + mc_direction*(ECAL_Z[0]-mc_vertex[0]);
        double mc_energy = particle->getEnergy() - particle->getMass(); // get kinetic energy
        mc_vertices.push_back(mc_vertex_x0);
        mc_positions.push_back(mc_position_x);
        mc_directions.push_back(mc_direction);
        mc_energies.push_back(mc_energy);
        
        streamlog_out(DEBUG) << "MC Particle " << i+1 <<"/"<< number <<":";
        streamlog_out(DEBUG) << " pdg = " << particle->getPDG() <<",";
        streamlog_out(DEBUG) << " m = " << particle->getMass() <<" GeV,";
        streamlog_out(DEBUG) << " charge = " << particle->getCharge() <<",";
        streamlog_out(DEBUG) << " status = " << particle->getGeneratorStatus() <<",";
        streamlog_out(DEBUG) << " N_daughters = " << (particle->getDaughters()).size() <<",";
        streamlog_out(DEBUG) <<endl;
        streamlog_out(DEBUG) << "Shooting from :";
        streamlog_out(DEBUG) << " vx = " << mc_vertex[0] <<",";
        streamlog_out(DEBUG) << " vy = " << mc_vertex[1] <<",";
        streamlog_out(DEBUG) << " vz = " << mc_vertex[2] <<" mm,";
        streamlog_out(DEBUG) <<endl;
        streamlog_out(DEBUG) << "With direction:";
        streamlog_out(DEBUG) << " ax = " << mc_direction.X() <<",";
        streamlog_out(DEBUG) << " ay = " << mc_direction.Y() <<",";
        streamlog_out(DEBUG) << " az = " << mc_direction.Z() <<";";
        streamlog_out(DEBUG) << " E = " << mc_energy <<" GeV.";
        streamlog_out(DEBUG) <<endl;
    }
}

void ClusterAnalysisProcessor::GetClusterInfo(EVENT::LCCollection *myCollection) {
    int number = myCollection->getNumberOfElements();
    streamlog_out(DEBUG) << "Reconstructed " << number <<" clusters in this event." <<endl;
    cl_vertices.clear(); cl_positions.clear(); cl_directions.clear(); cl_energies.clear();

    for (int i = 0; i < number; i++) {
        Cluster *cluster = dynamic_cast<Cluster *>(myCollection->getElementAt(i));
        const float* cl_centre = cluster->getPosition();
        auto cl_centre_x1 = ROOT::Math::XYZPoint(cl_centre[0], cl_centre[1], cl_centre[2]);
        double cl_theta = cluster->getITheta();
        double cl_phi = cluster->getIPhi();
        auto cl_direction = ROOT::Math::Polar3DVector(1, cl_theta, cl_phi);
        ROOT::Math::XYZPoint cl_position_x = cl_centre_x1 - cl_direction*(cl_centre[0]-ECAL_Z[0]);
        double cl_energy = cluster->getEnergy() * LINEARITY_MIP_to_GeV;
        cl_positions.push_back(cl_position_x);
        cl_directions.push_back(ROOT::Math::XYZVector(cl_direction.X(), cl_direction.Y(), cl_direction.Z()));
        cl_energies.push_back(cl_energy);
        
        streamlog_out(DEBUG) << "Cluster " << i+1 <<"/"<< number <<":";
        streamlog_out(DEBUG) << " E = " << cl_energy << " GeV,";
        streamlog_out(DEBUG) <<endl;
        streamlog_out(DEBUG) << "Centred at :";
        streamlog_out(DEBUG) << " cx = " << cl_centre[0] <<",";
        streamlog_out(DEBUG) << " cy = " << cl_centre[1] <<",";
        streamlog_out(DEBUG) << " cz = " << cl_centre[2] <<" mm,";
        streamlog_out(DEBUG) <<endl;
        streamlog_out(DEBUG) << "With direction :";
        streamlog_out(DEBUG) << " ax = " << cl_direction.X() <<",";
        streamlog_out(DEBUG) << " ay = " << cl_direction.Y() <<",";
        streamlog_out(DEBUG) << " az = " << cl_direction.Z() <<";";
        streamlog_out(DEBUG) << " theta = " << cl_theta/TMath::Pi() <<" pi,";
        streamlog_out(DEBUG) << " phi = " << cl_phi/TMath::Pi() <<" pi.";
        streamlog_out(DEBUG) <<endl;

        _xCluster->Fill(cl_centre[0]);
        _yCluster->Fill(cl_centre[1]);
        _zCluster->Fill(cl_centre[2]);
        _xyCluster->Fill(cl_centre[0], cl_centre[1]);
        _zxCluster->Fill(cl_centre[2], cl_centre[0]);
        _zyCluster->Fill(cl_centre[2], cl_centre[1]);
        _axCluster->Fill(cl_direction.X());
        _ayCluster->Fill(cl_direction.Y());
        _azCluster->Fill(cl_direction.Z());
        _eeCluster->Fill(cl_energy);
    }
}

void ClusterAnalysisProcessor::processRunHeader(LCRunHeader *run) {
}

void ClusterAnalysisProcessor::processEvent(LCEvent *evt) {
    try {
        streamlog_out(DEBUG) << "Showing MC Info \n ----------------------------------------- ";
        LCCollection *mccol = evt->getCollection(_MCColName);
        GetMCInfo(mccol);
        streamlog_out(DEBUG) << "Showing Arbor Cluster Info \n ----------------------------------------- ";
        LCCollection *cluster = evt->getCollection(_ClusterColName);
        GetClusterInfo(cluster);

        int N_particles = mc_energies.size();
        int N_clusters = cl_energies.size();
        if (N_particles==N_clusters) {
            for (int i=0; i<N_clusters; i++) {
                auto mc_position = mc_positions[i];
                auto cl_position = cl_positions[i];
                ROOT::Math::XYZVector d_position = cl_position - mc_position;
                _xResidue->Fill(d_position.X());
                _yResidue->Fill(d_position.Y());
                _xyResidue->Fill(d_position.X(), d_position.Y());
                auto mc_direction = mc_directions[i];
                auto cl_direction = cl_directions[i];
                _cosResidue->Fill(mc_direction.Dot(cl_direction));
                auto mc_energy = mc_energies[i];
                auto cl_energy = cl_energies[i];
                _eeResidue->Fill(cl_energy-mc_energy);
            }
        } else {
            streamlog_out(ERROR) << "Different numbers of particle(s) and cluster(s): ";
            streamlog_out(ERROR) << N_particles <<" != "<< N_clusters <<endl;
        }



    
    } catch (DataNotAvailableException &e) {
        streamlog_out(ERROR) << "Whoops!....\n";
        streamlog_out(ERROR) << e.what();
    }
}

void ClusterAnalysisProcessor::check(LCEvent * evt) {
    // nothing to check here - could be used to fill checkplots in reconstruction processor
}

void ClusterAnalysisProcessor::end() {
}

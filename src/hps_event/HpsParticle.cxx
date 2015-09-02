/**
 *
 * @file HpsParticle.cxx
 * @brief Class used to describe an HPS particle. 
 * @author Omar Moreno <omoreno1@ucsc.edu>
 *         Santa Cruz Institute for Particle Physics
 *         University of California, Santa Cruz
 * @date: March 29, 2013
 *
 */

#include <HpsParticle.h>

ClassImp(HpsParticle)

HpsParticle::HpsParticle()
    : TObject(),
      svt_tracks(new TRefArray()),
      ecal_clusters(new TRefArray()),
      particles(new TRefArray()),
      n_daughters(0),
      type(0), 
      charge(0), 
      px(0),
      py(0),
      pz(0),
      vtx_x(0),
      vtx_y(0),
      vtx_z(0),
      vtx_fit_chi2(0),
      energy(0),
      mass(0) {
}

HpsParticle::HpsParticle(const HpsParticle &particle_obj)
    : TObject(),
      svt_tracks(new TRefArray()),
      ecal_clusters(new TRefArray()),
      particles(new TRefArray()),
      n_daughters(particle_obj.n_daughters),
      type(particle_obj.type),
      charge(particle_obj.charge),
      px(particle_obj.px),
      py(particle_obj.py),
      pz(particle_obj.pz), 
      vtx_x(particle_obj.vtx_x),
      vtx_y(particle_obj.vtx_y),
      vtx_z(particle_obj.vtx_z),
      vtx_fit_chi2(particle_obj.vtx_fit_chi2),
      energy(particle_obj.energy),
      mass(particle_obj.mass) {

    *svt_tracks = *particle_obj.svt_tracks;
    *ecal_clusters = *particle_obj.ecal_clusters;   
    *particles = *particle_obj.particles; 
}

HpsParticle::~HpsParticle() {
    Clear();
    delete svt_tracks; 
    delete ecal_clusters; 
    delete particles;
}

HpsParticle &HpsParticle::operator=(const HpsParticle &particle_obj) {
    // Check for self-assignment
    if(this == &particle_obj) return *this;

    TObject::operator=(particle_obj);
    Clear(); 

    this->n_daughters = particle_obj.n_daughters;
    this->type = particle_obj.type;
    this->charge = particle_obj.charge;

    this->px = particle_obj.px; 
    this->py = particle_obj.py; 
    this->pz = particle_obj.pz; 
    this->vtx_x = particle_obj.vtx_x;
    this->vtx_y = particle_obj.vtx_y;
    this->vtx_z = particle_obj.vtx_z;
    this->vtx_fit_chi2 = particle_obj.vtx_fit_chi2;
    this->energy = particle_obj.energy;
    this->mass = particle_obj.mass; 

    svt_tracks = new TRefArray();
    *svt_tracks = *particle_obj.svt_tracks;
    ecal_clusters = new TRefArray(); 
    *ecal_clusters = *particle_obj.ecal_clusters;
    particles = new TRefArray(); 
    *particles = *particle_obj.particles;   

    return *this; 
}

void HpsParticle::Clear(Option_t* /* option */) {
    TObject::Clear();
    svt_tracks->Delete();
    ecal_clusters->Delete(); 
    particles->Delete();     
    n_daughters = 0;    
}

void HpsParticle::addTrack(SvtTrack* svt_track) {
    svt_tracks->Add((TObject*) svt_track);
}

void HpsParticle::addCluster(EcalCluster* ecal_cluster) {
    ecal_clusters->Add((TObject*) ecal_cluster);
}

void HpsParticle::addParticle(HpsParticle* particle) {
    ++n_daughters;
    particles->Add(particle); 
}

void HpsParticle::setMomentum(const double* momentum) {
    px = momentum[0];
    py = momentum[1];
    pz = momentum[2];
}

void HpsParticle::setVertexPosition(const float* vtx_pos) {
    vtx_x = (double) vtx_pos[0];
    vtx_y = (double) vtx_pos[1];
    vtx_z = (double) vtx_pos[2];
}

TRefArray* HpsParticle::getTracks() const {
    return svt_tracks;
}

TRefArray* HpsParticle::getClusters() const {
    return ecal_clusters;
}

TRefArray* HpsParticle::getParticles() const {
    return particles; 
}

std::vector<double> HpsParticle::getMomentum() const {
    std::vector<double> momentum(3,0);
    momentum[0] = px;
    momentum[1] = py;
    momentum[2] = pz;
    return momentum;
}

std::vector<double> HpsParticle::getVertexPosition() const {
    std::vector<double> vertex(3,0); 
    vertex[0] = vtx_x; 
    vertex[1] = vtx_y; 
    vertex[2] = vtx_z; 
    return vertex; 
}


/**
 *  @file MCParticleDataWriter.cxx
 *  @brief Class used to write MC particles to the DST.
 *	@author Omar Moreno, SLAC National Accelerator Laboratory
 */

#include "MCParticleDataWriter.h"

MCParticleDataWriter::MCParticleDataWriter() {
}


MCParticleDataWriter::~MCParticleDataWriter() {
}

void MCParticleDataWriter::writeData(EVENT::LCEvent* event, HpsEvent* hps_event)
{

	// Get the collection of MC particles from the event. If the event doesn't have
	// MC particles, return.
	try {
		mc_particles = static_cast<IMPL::LCCollectionVec*>(event->getCollection(mc_particles_collection_name));
	} catch(EVENT::DataNotAvailableException e){
		return;
	}

    std::map<IMPL::MCParticleImpl*, MCParticle*> particle_map;
	// Loop over the MC particles and fill the HpsEvent
	for(int mc_particle_n = 0; mc_particle_n < mc_particles->getNumberOfElements(); ++mc_particle_n) {

		// Get an MC particle from the LCIO collection
		mc_particle = static_cast<IMPL::MCParticleImpl*>(mc_particles->getElementAt(mc_particle_n));

		// Check If an MCParticle object corresponding to the LCIO MC particle
        // has been created. If not, create it and add it to the particle map.
        auto particle_it = particle_map.find(mc_particle);
        if (particle_it == particle_map.end()) {
            particle_map[mc_particle] = hps_event->addMCParticle(); 
        }
		MCParticle* hps_mc_particle = particle_map[mc_particle];
	
        // Set the energy
		hps_mc_particle->setEnergy(mc_particle->getEnergy());

		// Set the PDG ID
		hps_mc_particle->setPdgID(mc_particle->getPDG());

		// Set the generator status
		hps_mc_particle->setGenStatus(mc_particle->getGeneratorStatus());

        // Set the time
        hps_mc_particle->setTime(mc_particle->getTime()); 

        // Set the vertex position
        hps_mc_particle->setVertex(mc_particle->getVertex()[0], 
                                   mc_particle->getVertex()[1],
                                   mc_particle->getVertex()[2]);

        // Set the charge of the MC particle.  If the charge is equal to NaN,
		// set the charge to 1000
		if(std::isnan(mc_particle->getCharge())) hps_mc_particle->setCharge(1000);
		else hps_mc_particle->setCharge(mc_particle->getCharge());

		// Set the mass
		hps_mc_particle->setMass(mc_particle->getMass());

		// Set the end point of the MC particle
		hps_mc_particle->setEndPoint(mc_particle->getEndpoint()[0],
                                     mc_particle->getEndpoint()[1],
                                     mc_particle->getEndpoint()[2]);
		
        // Set the momentum
		hps_mc_particle->setMomentum(mc_particle->getMomentum()[0],
                                     mc_particle->getMomentum()[1], 
                                     mc_particle->getMomentum()[2]);

    }

	// Loop over the MC particles and build the parent and daughter trees.
	for(int mc_particle_n = 0; mc_particle_n < mc_particles->getNumberOfElements(); ++mc_particle_n) {
		
        // Get an MC particle from the LCIO collection.
		mc_particle = static_cast<IMPL::MCParticleImpl*>(mc_particles->getElementAt(mc_particle_n));
   
        // Get the corresponding MC particle from the MCParticle collection. 
		MCParticle* hps_mc_particle = particle_map[mc_particle];
        
        // Loop through and add all of the parents
        EVENT::MCParticleVec parents = mc_particle->getParents();
        for (auto parent : parents) { 

		    MCParticle* hps_parent = particle_map[static_cast<IMPL::MCParticleImpl*>(parent)];
            hps_mc_particle->addParent(hps_parent); 
        } 
        
        // Loop through and add all of the daughters
        EVENT::MCParticleVec daughters = mc_particle->getDaughters();
        for (auto daughter : daughters) { 

		    MCParticle* hps_daughter = particle_map[static_cast<IMPL::MCParticleImpl*>(daughter)];
            hps_mc_particle->addDaughter(hps_daughter); 
        } 
    }
}

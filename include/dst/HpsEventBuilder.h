/**
 *  @file   HpsEventBuilder.h
 *  @author Omar Moreno <omoreno1@ucsc.edu>
 *          Santa Cruz Institute for Particle Physics
 *          University of California, Santa Cruz
 *  @date   December 31, 2013
 *
 */

#ifndef __HPS_EVENT_BUILDER_H__
#define __HPS_EVENT_BUILDER_H__

//-----------//
//--- DST ---//
//-----------//
#include "EventBuilder.h"
#include "SvtDataWriter.h"
#include "EcalDataWriter.h"
#include "MCParticleDataWriter.h"
#include "HpsParticleDataWriter.h"
#include "TriggerData.h"

//------------//
//--- LCIO ---//
//------------//
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated"  // LCIO has lots of depricated "throw" statements.
#include <EVENT/LCGenericObject.h>
#include <EVENT/LCCollection.h>
#pragma GCC diagnostic pop

class HpsEventBuilder : public EventBuilder {

    public: 

        /** Constructor */
        HpsEventBuilder(); 

        /** Destructor */
        ~HpsEventBuilder(); 

        /**
         *  Write collections encapsulated by an LCEvent object to an 
         *  {@link HpsEvent} object.
         *
         *  @param lc_event LCEvent to process.
         *  @param hps_event The {@link HpsEvent} to write the LCEvent data to.
         */
        void makeHpsEvent(EVENT::LCEvent* lc_event, HpsEvent* hps_event); 

        /**
         * Write event specific variables to an {@link HpsEvent}.
         * 
         * @param lc_event  LCSim event from which the event information is
         *                  retrieved.
         * @param hps_event {@link HpsEvent} that the event information will
         *                  written to. 
         */
        void writeEventData(EVENT::LCEvent* lc_event, HpsEvent* hps_event); 

        /**
         *  Enable/disable the creation of an HpsEvent using recon files 
         *  containing ECal data only.
         *
         *  @param Ecal_only - true to write ECal data only, false otherwise
         */    
        void writeEcalOnly(const bool Ecal_only) { ecal_only = Ecal_only; };

    private:

        /** SVT data writer. */
        SvtDataWriter* svt_writer; 
        
        /** Ecal data writer. */
        EcalDataWriter* ecal_writer; 
        
        /** Monte Carlo data writer. */
        MCParticleDataWriter* mc_particle_writer;
        
        /** Recon particle data writer. */
        HpsParticleDataWriter* particle_writer; 

        /** The name of the collection containing RF hits */
        static const std::string RF_HIT_COL_NAME; 

        bool ecal_only;

}; // HpsEventBuilder

#endif // __HPS_EVENT_BUILDER_H__

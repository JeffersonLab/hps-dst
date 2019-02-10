/**
 *
 *  @file HpsEventBuilder.cxx
 *  @brief Builds {@link HpsEvent} objects out of LCIO events.
 *  @author Omar Moreno <omoreno1@ucsc.edu>
 *          Santa Cruz Institute for Particle Physics
 *          University of California, Santa Cruz
 *  @date   January 6, 2014
 *
 */

#include "HpsEventBuilder.h"

const std::string HpsEventBuilder::RF_HIT_COL_NAME = "RFHits";

HpsEventBuilder::HpsEventBuilder() 
    : svt_writer(new SvtDataWriter()), 
      ecal_writer(new EcalDataWriter()),
      mc_particle_writer(new MCParticleDataWriter()),
      particle_writer(new HpsParticleDataWriter()),
      ecal_only(false) {
}

HpsEventBuilder::~HpsEventBuilder() {
    delete svt_writer;
    delete ecal_writer;     
    delete mc_particle_writer;
    delete particle_writer; 
}

void HpsEventBuilder::makeHpsEvent(EVENT::LCEvent* event, HpsEvent* hps_event) {
    
    // Clear the HpsEvent from all previous information
    hps_event->Clear();

    this->writeEventData(event, hps_event); 

    // Write Ecal data to the HpsEvent
    ecal_writer->writeData(event, hps_event); 

    // If only Ecal data is going to be written to the DST, skip the rest
    // of the writers
    if (ecal_only) return;

    // Write SVT data to the HpsEvent
    svt_writer->writeData(event, hps_event);

    // Write MC particle data to the HpsEvent
    mc_particle_writer->writeData(event, hps_event);

    // Write the HpsParticle data to the HpsEvent
    particle_writer->writeData(event, hps_event); 

}

void HpsEventBuilder::writeEventData(EVENT::LCEvent* lc_event, HpsEvent* hps_event) { 
    
    // Set the event number
    hps_event->setEventNumber(lc_event->getEventNumber());

    // Set the run number
    hps_event->setRunNumber(lc_event->getRunNumber());

    // Set the trigger timestamp 
    hps_event->setEventTime(lc_event->getTimeStamp()); 

    // Set the SVT bias state
    hps_event->setSvtBiasState(lc_event->getParameters().getIntVal("svt_bias_good")); 
    
    // Set the flag indicating whether the event was affected by SVT burst
    // mode noise 
    hps_event->setSvtBurstModeNoise(lc_event->getParameters().getIntVal("svt_burstmode_noise_good"));

    // Set the flag indicating whether the SVT latency was correct during an
    // event.
    hps_event->setSvtLatencyState(lc_event->getParameters().getIntVal("svt_latency_good")); 

    // Set the SVT position state
    hps_event->setSvtPositionState(lc_event->getParameters().getIntVal("svt_position_good"));

    // Set the SVT event header state
    hps_event->setSvtEventHeaderState(lc_event->getParameters().getIntVal("svt_event_header_good"));

    // Set the trigger data
    EVENT::LCCollection* trigger_data = nullptr;
    try { 
        trigger_data = (EVENT::LCCollection*) lc_event->getCollection("TriggerBank"); 
    } catch(EVENT::DataNotAvailableException e) {
        // It's fine if the event doesn't have a trigger bank.
    }

    for (int trigger_datum_n = 0; trigger_datum_n < trigger_data->getNumberOfElements(); ++trigger_datum_n) { 
       
        EVENT::LCGenericObject* trigger_datum = (EVENT::LCGenericObject*) trigger_data->getElementAt(trigger_datum_n); 
        if (trigger_datum->getIntVal(0) == 0xe10a) { 
           
            TriggerData* hps_trigger_data = new TriggerData(trigger_datum); 
            hps_event->setSingle0Trigger((int) hps_trigger_data->isSingle0Trigger());
            hps_event->setSingle1Trigger((int) hps_trigger_data->isSingle1Trigger());
            hps_event->setPair0Trigger((int) hps_trigger_data->isPair0Trigger());
            hps_event->setPair1Trigger((int) hps_trigger_data->isPair1Trigger());
            hps_event->setPulserTrigger((int) hps_trigger_data->isPulserTrigger());

            delete hps_trigger_data;
            hps_trigger_data = NULL;
            break;
        }
    }

    //
    // Write the RF time to the event
    //

    // Get the LCIO GenericObject collection containing the RF times
    EVENT::LCCollection* rf_hits = (EVENT::LCCollection*) lc_event->getCollection(RF_HIT_COL_NAME);

    // The collection should only have a single RFHit object per event
    if (rf_hits->getNumberOfElements() > 1) { 
        throw std::runtime_error("[ HpsEventBuilder ]: The collection " + RF_HIT_COL_NAME + 
                " doesn't have the expected number of elements."); 
    }

    // Loop over all the RF hits in the event and write them to the DST
    for (int rf_hit_n = 0; rf_hit_n < rf_hits->getNumberOfElements(); ++rf_hit_n) { 
        
        // Get the RF hit from the event
        EVENT::LCGenericObject* rf_hit = (EVENT::LCGenericObject*) rf_hits->getElementAt(rf_hit_n);
    
        // An RFHit GenericObject should only have two RF times
        if (rf_hit->getNDouble() != 2) { 
            throw std::runtime_error("[ HpsEventBuilder ]: The collection " + RF_HIT_COL_NAME + 
                    " has the wrong structure."); 
        }
    
        // Write the RF times to the event
        for (int rf_hit_channel = 0; rf_hit_channel < rf_hit->getNDouble(); ++rf_hit_channel) { 
            hps_event->setRfTime(rf_hit_channel, rf_hit->getDoubleVal(rf_hit_channel));  
        }
    }
}

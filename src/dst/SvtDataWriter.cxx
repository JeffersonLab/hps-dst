/**
 * 
 * @file SvtDataWriter.cxx
 * @brief Data writer used to convert LCIO Tracks and TrackerHits
 *        to SvtTracks and SvtHits and add them to the HPS event.
 * @author Omar Moreno <omoreno1@ucsc.edu>
 *         Santa Cruz Institute for Particle Physics
 *         University of California, Santa Cruz
 * @date January 2, 2013
 *
 */

#include <SvtDataWriter.h>

const std::string SvtDataWriter::GBL_KINK_DATA_COL_NAME = "GblKinkData";

const std::string SvtDataWriter::GBL_KINK_DATA_REL_COL_NAME = "GBLKinkDataRelations";

const std::string SvtDataWriter::TRACK_DATA_COL_NAME = "TrackData"; 

const std::string SvtDataWriter::TRACK_DATA_REL_COL_NAME = "TrackDataRelations";

const std::string SvtDataWriter::SEED_TO_GBL_REL_COL_NAME = "MatchedToGBLTrackRelations";

SvtDataWriter::SvtDataWriter() { 
}

SvtDataWriter::~SvtDataWriter() {
}

void SvtDataWriter::writeData(EVENT::LCEvent* event, HpsEvent* hps_event) {
   
    // Get the collection of 3D hits from the LCIO event. If no such collection 
    // exist, a DataNotAvailableException is thrown
    EVENT::LCCollection* tracker_hits = static_cast<EVENT::LCCollection*>(event->getCollection("RotatedHelicalTrackHits"));

    // Create a map from an LCIO TrackerHit to a SvtHit. This will be used when
    // assigning references to a track
    // TODO: Use an unordered map for faster access
    std::map<EVENT::TrackerHit*, SvtHit*> svt_hit_map; 

    // Loop over all of the 3D hits in the LCIO event and add them to the 
    // HPS event
    for (int tracker_hit_n = 0; tracker_hit_n < tracker_hits->getNumberOfElements(); ++tracker_hit_n) { 
        
        // Get a 3D hit from the list of hits
        EVENT::TrackerHit* tracker_hit = static_cast<EVENT::TrackerHit*>(tracker_hits->getElementAt(tracker_hit_n));

        // Add an SvtHit object to the HPS event
        SvtHit* svt_hit = hps_event->addSvtHit();

        // Set the SvtHit layer
        svt_hit->setLayer(TrackUtils::getLayer(tracker_hit));
                
        // Rotate the position of the LCIO TrackerHit and Set the position
        // of the SvtHit
        double hit_position[3] = { 
            tracker_hit->getPosition()[1], 
            tracker_hit->getPosition()[2], 
            tracker_hit->getPosition()[0]
        };
        svt_hit->setPosition(hit_position);

        // Set the covariance matrix of the SvtHit
        svt_hit->setCovarianceMatrix(tracker_hit->getCovMatrix());
                
        // Set the time of the SvtHit
        svt_hit->setTime(tracker_hit->getTime());

        // Map the TrackerHit object to the corresponding SvtHit object. This
        // will be used later when setting references for hits on tracks.
        svt_hit_map[tracker_hit] = svt_hit; 
    }

    // Get all track collections from the event
    std::vector<EVENT::LCCollection*> track_collections = DstUtils::getCollections(event, EVENT::LCIO::TRACK);

    // Get the collection of LCRelations between track data variables 
    // (TrackData) and the corresponding track.
    EVENT::LCCollection* track_data = static_cast<EVENT::LCCollection*>(event->getCollection(TRACK_DATA_REL_COL_NAME));

    // Instantiate an LCRelation navigator which will allow faster access
    // to TrackData objects  
    UTIL::LCRelationNavigator* track_data_nav = new UTIL::LCRelationNavigator(track_data);

    // Get the collection of LCRelations between GBL kink data variables 
    // (GBLKinkData) and the corresponding track.
    EVENT::LCCollection* gbl_kink_data = static_cast<EVENT::LCCollection*>(event->getCollection(GBL_KINK_DATA_REL_COL_NAME));

    // Instantiate an LCRelation navigator which will allow faster access 
    // to GBLKinkData object
    UTIL::LCRelationNavigator* gbl_kink_data_nav = new UTIL::LCRelationNavigator(gbl_kink_data);

    // Create a map between an LCIO GBL track and the corresponding HpsEvent
    // GBL track. This map will be used when relating an HpsEvent GBL track to 
    // the corresponding seed track.
    std::map<GblTrack*, EVENT::Track*> gbl_track_map; 

    // Create a map between an LCIO track and the corresponding HpsEvent 
    // track.
    std::map<EVENT::Track*, SvtTrack*> track_map; 

    // Loop over all the track collections and process them
    for (auto tracks : track_collections) { 
       
        // Loop over all the LCIO Tracks and add them to the HPS event.
        for (int track_n = 0; track_n < tracks->getNumberOfElements(); ++track_n) {
        
            // Get a LCIO Track from the LCIO event
            EVENT::Track* track = static_cast<EVENT::Track*>(tracks->getElementAt(track_n));

            // Check whether a track has been refit with GBL. If so, add a 
            // Gbltrack to the HPS event.  Otherwise, just add an SvtTrack
            SvtTrack* svt_track = nullptr; 
            if (DstUtils::isGbl(track)) { 
          
                // Add a GblTrack object to the HPS event
                svt_track = hps_event->addGblTrack(); 

                // Map the LCIO GBL track to the HpsEvent GBL track
                gbl_track_map[(GblTrack*) svt_track] = track;

                // Get the list of GBLKinkData associated with the LCIO Track
                EVENT::LCObjectVec gbl_kink_data_list = gbl_kink_data_nav->getRelatedFromObjects(track);

                // The container of GBLKinkData objects should only contain a 
                // single object. If not, throw an exception
                if (gbl_kink_data_list.size() != 1) { 
                    throw std::runtime_error("[ SvtDataWriter ]: The data structure has the wrong format."); 
                }

                // Get the list GBLKinkData GenericObject associated with the LCIO Track
                IMPL::LCGenericObjectImpl* gbl_kink_datum = static_cast<IMPL::LCGenericObjectImpl*>(gbl_kink_data_list.at(0));

                // Set the lambda and phi kink values
                for (int kink_index = 0; kink_index < gbl_kink_datum->getNDouble(); ++kink_index) { 
                    ((GblTrack*) svt_track)->setLambdaKink(kink_index, gbl_kink_datum->getFloatVal(kink_index));
                    ((GblTrack*) svt_track)->setPhiKink(kink_index, gbl_kink_datum->getDoubleVal(kink_index));
                }

            } else { 
                // Add an SvtTrack object to the HPS event
                svt_track = hps_event->addTrack();

                // Map the LCIO track to the HpsEvent track
                track_map[track] = svt_track; 
            }
            
            // Set the SvtTrack track parameters
            svt_track->setTrackParameters(track->getD0(), 
                                          track->getPhi(), 
                                          track->getOmega(), 
                                          track->getTanLambda(), 
                                          track->getZ0());
           
            // Set the SvtTrack type
            svt_track->setType(track->getType()); 

            // Set the SvtTrack fit chi^2
            svt_track->setChi2(track->getChi2());

            // Set the position of the extrapolated track at the Ecal face. The
            // extrapolation uses the full 3D field map.
            const EVENT::TrackState* track_state = track->getTrackState(EVENT::TrackState::AtCalorimeter);
            if (track_state != NULL) {
                double position_at_ecal[3] = { 
                    track_state->getReferencePoint()[1],  
                    track_state->getReferencePoint()[2],  
                    track_state->getReferencePoint()[0]
            };  
                svt_track->setPositionAtEcal(position_at_ecal); 
            } 
        
            // Get the list of TrackData associated with the LCIO Track
            EVENT::LCObjectVec track_data_list = track_data_nav->getRelatedFromObjects(track);
            
            // The container of TrackData objects should only contain a single
            //  object.  If not, throw an exception.
            if (track_data_list.size() == 1) { 
                // Get the TrackData GenericObject associated with the LCIO Track
                IMPL::LCGenericObjectImpl* track_datum = static_cast<IMPL::LCGenericObjectImpl*>(track_data_list.at(0));

                // Check that the TrackData data structure is correct.  If it's
                // not, throw a runtime exception.   
                if (track_datum->getNDouble() != 12 || track_datum->getNFloat() != 1 
                        || track_datum->getNInt() != 1) {
                    throw std::runtime_error("[ SvtDataWriter ]: The collection " + TRACK_DATA_COL_NAME 
                            + " has the wrong structure.");
                }

                // Set the SvtTrack isolation values
                for (int iso_index = 0; iso_index < track_datum->getNDouble(); ++iso_index) { 
                    svt_track->setIsolation(iso_index, track_datum->getDoubleVal(iso_index));
                }
        
                // Set the SvtTrack time
                svt_track->setTrackTime(track_datum->getFloatVal(0));

                // Set the volume (top/bottom) in which the SvtTrack resides
                svt_track->setTrackVolume(track_datum->getIntVal(0));
            }

            // Get the collection of 3D hits associated with a LCIO Track
            EVENT::TrackerHitVec tracker_hits_vec = track->getTrackerHits();

            //  Iterate through the collection of 3D hits (TrackerHit objects)
            //  associated with a track, find the corresponding hits in the HPS
            //  event and add references to the track
            for (auto tracker_hit : tracker_hits_vec) {
                
                // Add a reference to the hit
                svt_track->addHit(svt_hit_map[tracker_hit]);  
            }
        }
    }

    // Delete all LCRelations navigator objects
    delete track_data_nav;
    delete gbl_kink_data_nav;

    // Get the collection of LCRelations between seed tracks and a GBL tracks.
    EVENT::LCCollection* seed_to_gbl_relations = static_cast<EVENT::LCCollection*>(event->getCollection(SEED_TO_GBL_REL_COL_NAME));
   
    // Instantiate an LCRelation navigator which will allow faster access
    // to the seed to GBL LCRelations
    UTIL::LCRelationNavigator* seed_to_gbl_relations_nav = new UTIL::LCRelationNavigator(seed_to_gbl_relations); 

    for (int gbl_track_n = 0; gbl_track_n < hps_event->getNumberOfGblTracks(); ++gbl_track_n) { 
        GblTrack* gbl_track = hps_event->getGblTrack(gbl_track_n); 
        
        // Get the list of LCRelations associated with the LCIO GBL track used
        // to create this HpsEvent GBL track
        EVENT::LCObjectVec seed_to_gbl_list 
            = seed_to_gbl_relations_nav->getRelatedFromObjects(gbl_track_map[gbl_track]); 

        // There should only be a single LCRelation between a LCIO GBL track 
        // and a LCIO seed trak.
        if (seed_to_gbl_list.size() != 1) { 
            throw std::runtime_error("[ SvtDataWriter ]: The data structure has the wrong format.");
        }

        // Get the TrackData GenericObject associated with the LCIO Track
        EVENT::Track* seed_track = static_cast<EVENT::Track*>(seed_to_gbl_list.at(0));

        // Set a reference to the HpsEvent seed track
        gbl_track->setSeedTrack(track_map[seed_track]);

        // Set a reference from the seed track to the GBL track
        track_map[seed_track]->setGblTrack(gbl_track);
    }

    delete seed_to_gbl_relations_nav; 
}

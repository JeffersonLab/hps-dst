/**
 * @file EcalDataWriter.cxx
 * @date January 7, 2013
 */

#include "EcalDataWriter.h"

const std::string EcalDataWriter::ECAL_CLUSTER_COL_NAME{"EcalClustersCorr"};

EcalDataWriter::EcalDataWriter() { 
}

EcalDataWriter::~EcalDataWriter() {
}

void EcalDataWriter::writeData(EVENT::LCEvent* event, HpsEvent* hps_event) {

    // Get the collection of Ecal hits from the event.
    std::map<std::pair<int,int>,EcalHit *> hit_map;

    // Attempt to retrieve the collection "TimeCorrEcalHits" from the event. If
    // the collection doesn't exist, handle the DataNotAvailableCollection and
    // attempt to retrieve the collection "EcalCalHits". If that collection 
    // doesn't exist, the DST maker will fail.
    EVENT::LCCollection* hits{nullptr};
    try { 
        hits =  static_cast<EVENT::LCCollection*>(event->getCollection(hits_collection_name)); 
    } catch (EVENT::DataNotAvailableException e) { 
        this->hits_collection_name = "EcalCalHits";
        hits =  static_cast<EVENT::LCCollection*>(event->getCollection(hits_collection_name)); 
    }
    //std::cout << "[ EcalDataWriter ]: Using collection " << hits_collection_name << std::endl;

    for(int hit_n=0;hit_n<hits->getNumberOfElements();++hit_n){
        
        calorimeter_hit = static_cast<IMPL::CalorimeterHitImpl*>(hits->getElementAt(hit_n));

        // Get the unique cell id of this hit. Combine it with the integer time,
        // since a crystal can be hit more than once.
        int id0=calorimeter_hit->getCellID0();
        // 0.1 ns resolution is sufficient to distinguish any 2 hits on the same crystal.
        int id1=(int)(10.0*calorimeter_hit->getTime()); 

        // Add an Ecal hit to the HPS Event
        ecal_hit = hps_event->addEcalHit();

        // Store the hit in the map for easy access later.
        hit_map[std::make_pair(id0,id1)] = ecal_hit;

        // Set the energy of the Ecal hit
        ecal_hit->setEnergy(calorimeter_hit->getEnergy());

        // Set the hit time of the Ecal hit
        ecal_hit->setTime(calorimeter_hit->getTime());

        // Set the indices of the crystal
        int index_x = (int)EcalUtils::getIdentifierFieldValue("ix", calorimeter_hit);
        int index_y = (int)EcalUtils::getIdentifierFieldValue("iy", calorimeter_hit);

        ecal_hit->setCrystalIndices(index_x, index_y);

    }

    // Get the collection of Ecal clusters from the event
    EVENT::LCCollection* clusters 
        = static_cast<EVENT::LCCollection*>(event->getCollection(ECAL_CLUSTER_COL_NAME));

    // Loop over all clusters and fill the event
    for(int cluster_n = 0; cluster_n < clusters->getNumberOfElements(); ++cluster_n) {

        // Get an Ecal cluster from the LCIO collection
        cluster = static_cast<IMPL::ClusterImpl*>(clusters->getElementAt(cluster_n));

        // Add a cluster to the HPS Event
        ecal_cluster = hps_event->addEcalCluster();

        // Set the cluster position
        ecal_cluster->setPosition(cluster->getPosition());

        // Set the cluster energy
        ecal_cluster->setEnergy(cluster->getEnergy());

        // Get the ecal hits used to create the cluster
        EVENT::CalorimeterHitVec calorimeter_hits = cluster->getCalorimeterHits();

        // Loop over all of the Ecal hits and add them to the Ecal cluster.  The
        // seed hit is set to be the hit with the highest energy.  The cluster time
        // is set to be the hit time of the seed hit.
        for(int ecal_hit_n = 0; ecal_hit_n < (int) calorimeter_hits.size(); ++ecal_hit_n) {

            // Get an Ecal hit
            calorimeter_hit = static_cast<IMPL::CalorimeterHitImpl*>(calorimeter_hits[ecal_hit_n]);

            int id0=calorimeter_hit->getCellID0();
            int id1=(int)(10.0*calorimeter_hit->getTime());

            if( hit_map.find(std::make_pair(id0,id1)) == hit_map.end() ){
                std::cerr << "WOOPS -- Hit not found in map, but it is in the cluster. " << id0;
            }else{
                // Get the hit and add it to the cluster
                ecal_hit =hit_map[std::make_pair(id0,id1)];
                ecal_cluster->addHit(ecal_hit);
            }
        }
    }
    
    // Get the collection of Ecal clusters from the event
    EVENT::LCCollection* uncor_clusters = static_cast<EVENT::LCCollection*>(event->getCollection("EcalClusters"));
    
    // Loop over all clusters and fill the event
    for(int cluster_n = 0; cluster_n < uncor_clusters->getNumberOfElements(); ++cluster_n) {
        
        // Get an Ecal cluster from the LCIO collection
        cluster = static_cast<IMPL::ClusterImpl*>(uncor_clusters->getElementAt(cluster_n));
        
        // Add a cluster to the HPS Event
        ecal_cluster = hps_event->addUncorEcalCluster();
        
        // Set the cluster position
        ecal_cluster->setPosition(cluster->getPosition());
        
        // Set the cluster energy
        ecal_cluster->setEnergy(cluster->getEnergy());
        
        // Get the ecal hits used to create the cluster
        EVENT::CalorimeterHitVec calorimeter_hits = cluster->getCalorimeterHits();
        
        // Loop over all of the Ecal hits and add them to the Ecal cluster.  The
        // seed hit is set to be the hit with the highest energy.  The cluster time
        // is set to be the hit time of the seed hit.
        for(int ecal_hit_n = 0; ecal_hit_n < (int) calorimeter_hits.size(); ++ecal_hit_n) {
            
            // Get an Ecal hit
            calorimeter_hit = static_cast<IMPL::CalorimeterHitImpl*>(calorimeter_hits[ecal_hit_n]);
            
            int id0=calorimeter_hit->getCellID0();
            int id1=(int)(10.0*calorimeter_hit->getTime());
            
            if( hit_map.find(std::make_pair(id0,id1)) == hit_map.end() ){
                std::cerr << "WOOPS -- Hit not found in map, but it is in the cluster. " << id0;
            }else{
                // Get the hit and add it to the cluster
                ecal_hit =hit_map[std::make_pair(id0,id1)];
                ecal_cluster->addHit(ecal_hit);
            }
        }
    }
}

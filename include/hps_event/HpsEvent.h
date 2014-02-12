/**
 *
 * @author: 	Omar Moreno <omoreno1@ucsc.edu>
 * @section institution
 * 				Santa Cruz Institute for Particle Physics
 * 				University of California, Santa Cruz
 * @version:    v 0.1
 * @date:       February 19, 2013
 */

#ifndef _HPS_EVENT_H_
#define	_HPS_EVENT_H_

//--- C++ ---//
#include <iostream>
#include <assert.h>

//-- ROOT ---//
#include <TObject.h>
#include <TClonesArray.h>

//--- HPS Event ---//
//-----------------//
#include <SvtTrack.h>
#include <SvtHit.h>
#include <EcalCluster.h> 
#include <EcalHit.h>
#include <MuonCluster.h>
#include <HpsReconstructedParticle.h>
#include <HpsMCParticle.h>

class HpsEvent : public TObject { 

    public:

		HpsEvent();
        HpsEvent(const HpsEvent &hpsEventoObj);
        virtual ~HpsEvent();    
        HpsEvent &operator=(const HpsEvent &hpsEventObj);

        void Clear(Option_t *option="");
        SvtTrack* 		addTrack();
        SvtHit* 		addSvtHit();
        EcalCluster* 	addEcalCluster();
        EcalHit* 		addEcalHit();
        MuonCluster* 	addMuonCluster();
        HpsReconstructedParticle* addReconParticle(int);
        HpsMCParticle*  addHpsMCParticle();

        void setEventNumber(int event_number){ this->event_number = event_number; };
        void setRunNumber(int run_number){ this->run_number = run_number; };
        void setTriggerBitInfo(std::vector<int> trigger_bits){ this->trigger_bits = trigger_bits; };

        int getEventNumber()       const  { return event_number; };
        int getRunNumber()         const  { return run_number; };
        int getNumberOfTracks()    const { return n_tracks; };
        int getNumberOfEcalClusters()  const { return n_ecal_clusters; };
        int getNumberOfMuonClusters()  const { return n_muon_clusters; };
        std::vector<int> getTriggerBitInfo() const { return trigger_bits; };

        SvtTrack*      getTrack(int);
        SvtHit*        getSvtHit(int);
        EcalCluster*   getEcalCluster(int);
        EcalHit* 	   getEcalHit(int);
        MuonCluster*   getMuonCluster(int);
        HpsMCParticle* getMCParticle(int);

        ClassDef(HpsEvent, 1);	

    private:

        TClonesArray* tracks;              //->
        TClonesArray* svt_hits;            //->
        TClonesArray* ecal_clusters;       //->
        TClonesArray* ecal_hits; 		   //->
        TClonesArray* muon_clusters;       //->
        TClonesArray* fs_recon_particles;  //->
        TClonesArray* vtx_recon_particles; //->
        TClonesArray* mc_particles; 	   //->

        int event_number;
        int run_number;
        int n_tracks;
        int n_svt_hits;
        int n_ecal_clusters;
        int n_ecal_hits;
        int n_muon_clusters;
        int n_fs_recon_particles;
        int n_vtx_recon_particles;
        int n_mc_particles;

        static const int fs_type;
        static const int vtx_type;

        std::vector<int> trigger_bits;
};

#endif



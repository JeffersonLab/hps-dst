/**
 * 
 * @file GblTrack.h
 * @brief Class used to describe an HPS SVT GBL track.
 * @author Omar Moreno <omoreno1@ucsc.edu>
 *         Santa Cruz Institute for Particle Physics
 *         University of California, Santa Cruz
 * @date December 11, 2015
 * 
 */

#ifndef __GBL_TRACK_H__
#define __GBL_TRACK_H__

//-----------------//
//--- HPS Event ---//
//-----------------//
#include "SvtTrack.h"

class GblTrack : public SvtTrack { 

    public: 
        
        /** Constructor */
        GblTrack();

        /**
         * Set the lambda kink of the given layer.
         *
         * @param layer Layer number associated with the given lambda kink.
         * @param lambda_kink The lambda kink value.
         */
        void setLambdaKink(const int layer, const double lambda_kink) { this->lambda_kinks[layer] = lambda_kink; }

        /**
         * Set the phi kink of the given layer.
         *
         * @param layer Layer number associated with the given phi kink.
         * @param phi_kink The phi kink value.
         */
        void setPhiKink(const int layer, const double phi_kink) { this->phi_kinks[layer] = phi_kink; }

        /**
         * Set the seed track that was refit to create this GBL track.
         *
         * @param seed_track The seed track (SvtTrack object)
         */
        void setSeedTrack(SvtTrack* seed_track) { this->seed_track = (TObject*) seed_track; }

        /**
         * Get the lambda kink value of the given layer.
         *
         * @param layer The SVT layer of interest.
         * @return The lambda kink value of the given layer.
         */
        double getLambdaKink(const int layer) const { return lambda_kinks[layer]; }

        /**
         * Get the phi kink value of the given layer.
         *
         * @param layer The SVT layer of interest.
         * @return The phi kink value of the given layer.
         */
        double getPhiKink(const int layer) const { return phi_kinks[layer]; }

        /**
         * Get the seed track that was refit to create this GBL track.
         *
         * @return The seed track.
         */ 
        SvtTrack* getSeedTrack() const { return (SvtTrack*) this->seed_track.GetObject(); }

        ClassDef(GblTrack, 1)

    private: 

        /** Reference to corresponding seed track */
        TRef seed_track; 

        /** Array used to store the lambda kinks for each of the sensor layers. */
        double lambda_kinks[12];  
        
        /** Array used to store the phi kinks for each of the sensor layers. */
        double phi_kinks[12];  
};

#endif // __GBL_TRACK_H__

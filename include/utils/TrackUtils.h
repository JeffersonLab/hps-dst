/**
 *  @section purpose:
 *  @author: Omar Moreno <omoreno1@ucsc.edu>
 *			 Santa Cruz Institute for Particle Physics
 *			 University of California, Santa Cruz
 *  @date: December 16, 2013
 *  @version: 1.0
 *
 */

#ifndef __TRACK_UTILS_H__
#define __TRACK_UTILS_H__

//--- C++ ---//
//-----------//
#include <cmath>
#include <vector>

//--- LCIO ---//
//------------//
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated"  // LCIO has lots of depricated "throw" statements.
#include <IMPL/TrackImpl.h>
#include <EVENT/TrackerHit.h>
#pragma GCC diagnostic pop

namespace TrackUtils { 

    /**
     *
     */
    double getX0(IMPL::TrackImpl* track);

    /**
     *
     */ 
    double getY0(IMPL::TrackImpl* track); 

    /**
     *
     */ 
    double getR(IMPL::TrackImpl* track); 

    /**
     *
     */ 
    double getDoca(IMPL::TrackImpl* track);

    /**
     *
     */ 
    double getPhi0(IMPL::TrackImpl* track);

    /**
     *
     */
   double getPhi(IMPL::TrackImpl*, std::vector<double>); 

    /**
     *
     */ 
    double getZ0(IMPL::TrackImpl* track); 

    /**
     *
     */ 
    double getTanLambda(IMPL::TrackImpl* track); 

    /**
     *
     */ 
    double getSinTheta(IMPL::TrackImpl* track); 

    /**
     *
     */ 
    double getCosTheta(IMPL::TrackImpl* track);

    /**
     *
     */
    double getXc(IMPL::TrackImpl* track); 

    /**
     *
     */
    double getYc(IMPL::TrackImpl* track); 

	/**
	 *
	 */
    template <typename T> int signum(T val){
        return (T(0) < val) - (val < T(0));
    }

	/**
	 *
	 */
	std::vector<double> getMomentumVector(IMPL::TrackImpl*, double);

	/**
	 *
	 */
	double getMomentum(IMPL::TrackImpl*, double);

	/**
	 *
	 */
	int getCharge(IMPL::TrackImpl*);

	/**
	 *
	 */
	int getLayer(EVENT::TrackerHit*);

}

#endif // __TRACK_UTILS_H__

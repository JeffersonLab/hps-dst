/**
 *  @section purpose:
 *  @author: Omar Moreno <omoreno1@ucsc.edu>
 *			 Santa Cruz Institute for Particle Physics
 *			 University of California, Santa Cruz
 *  @date: December 12, 2013
 *  @version: 1.0
 *
 */

#ifndef __ECAL_UTILS_H__
#define __ECAL_UTILS_H__

//--- C++ ---//
//-----------//
#include <cmath>

//--- LCIO ---//
//------------//
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated"  // LCIO has lots of depricated "throw" statements.
#include <EVENT/CalorimeterHit.h>
#include <IMPL/ClusterImpl.h>
#include <UTIL/BitField64.h>
#include <IMPL/CalorimeterHitImpl.h>
#include <IMPL/LCRelationImpl.h>
#include <IMPL/LCCollectionVec.h>
#include <IMPL/LCGenericObjectImpl.h>
#pragma GCC diagnostic pop

namespace EcalUtils { 

	/**
	 *
	 */
    EVENT::CalorimeterHit* getClusterSeed(IMPL::ClusterImpl*);
    
	/**
	 *
	 */
	UTIL::BitFieldValue getIdentifierFieldValue(std::string, EVENT::CalorimeterHit*);
    
	/**
	 *
	 */
	int getQuadrant(int, int);

	/**
	 *
	 */
	std::vector<double> getShowerMoments(IMPL::ClusterImpl*, IMPL::LCCollectionVec*); 	
}

#endif // __ECAL_UTILS_H__

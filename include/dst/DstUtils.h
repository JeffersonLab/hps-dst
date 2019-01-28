/**
 *
 * @file DstUtils.h
 * @brief A set of utility methods used by the DST writers and maker.
 * @author Omar Moreno <omoreno1@ucsc.edu>
 *         Santa Cruz Institute for Particle Physics (SCIPP)
 *         University of California, Santa Cruz
 * @date September 1, 2015
 *
 */

#ifndef __DST_UTILS_H__
#define __DST_UTILS_H__

//------------------//
//--- C++ StdLib ---//
//------------------//
#include <string>
#include <vector>
#include <iostream>

//------------//
//--- LCIO ---//
//------------//
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated"  // LCIO has lots of depricated "throw" statements.
#include <EVENT/LCEvent.h>
#include <EVENT/LCCollection.h>
#include <EVENT/ReconstructedParticle.h>
#include <EVENT/Track.h>
#pragma GCC diagnostic pop

namespace DstUtils { 

    /**
     * Get all LCIO collections of the given type from the event.
     *
     * @param event The LCIO event
     * @param type The LCIO collection type
     * @return A vector containing all LCIO collections of the given type
     */
    std::vector<EVENT::LCCollection*> getCollections(EVENT::LCEvent* event, std::string type); 

    /**
     * Checks the track type of an LCIO Track to determine if it was 
     * refit using GBL.
     *
     * @param track LCIO Track whose track type will be checked.
     * @return Returns <code>true</code> if an LCIO Track was refit using
     *         GBL strategy, <code>false</code> otherwise.
     */
    bool isGbl(EVENT::Track* track); 

    /**
     * Checks the particle type of an LCIO ReconstructedParticle to determine
     * if it is composed of GBL tracks.
     *
     * @param particle LCIO ReconstructedParticle whose type will be checked.
     * @return Returns <code>true</code> if the LCIO RecontructedParticle is 
     *         composed of GBL tracks.
     */
    bool isGbl(EVENT::ReconstructedParticle* particle);

} // DstUtils

#endif // __DST_UTILS_H__

/**
 *  @file PairHash.h
 *  @brief  A hash function for a std::pair of integers.  The hash function
 *          mimics what is done by boost::hash.
 *  @author Omar Moreno <omoreno1@ucsc.edu>
 *          Santa Cruz Institute for Particle Physics
 *          University of California, Santa Cruz
 *  @date December 09, 2014
 *
 */

#ifndef __PAIR_HASH_H__
#define __PAIR_HASH_H__

//------------------//
//--- C++ StdLib ---//
//------------------//
#include <cstdlib>
#include <utility>
#include <functional>

class PairHash {

    public:

        /**
         * Hash function of a std::pair of integers
         *
         * @parameter p - std::pair of integers
         * @return Hash for a std::pair
         */
        // template <typename T, typename U>
        // std::size_t operator() (const std::pair <T, U> &p) const;
        std::size_t operator() (const std::pair <int, int> &p) const;


};  // PairHash

#endif // __PAIR_HASH_H__

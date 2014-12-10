/**
 *  @file PairHash.cxx
 *  @brief  A hash function for a pair of integers.  The hash function mimics
 *          what is done by boost::hash.
 *  @author Omar Moreno <omoreno1@ucsc.edu>
 *          Santa Cruz Institute for Particle Physics
 *          University of California, Santa Cruz
 *  @date December 09, 2014
 *
 */

#include <PairHash.h>

std::size_t PairHash::operator() (const std::pair <int, int> &p) const {

    // This is just a copy of what boost::hash does.
	size_t seed = std::hash<int>()(p.first);
	return std::hash<int>()(p.second) + 0x9e3779b9 + (seed<<6) + (seed>>2);
}

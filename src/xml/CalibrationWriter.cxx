/**
 *
 *  @file   CalibrationWriter.cxx 
 *  @brief  XML writer used to write SVT baseline and noise conditions.
 *  @author Omar Moreno <omoreno1@ucsc.edu>
 *          University of California, Santa Cruz
 *          Santa Cruz Institute for Particle Physics
 *  @date   September 15, 2014 
 */

#include <CalibrationWriter.h>

CalibrationWriter::CalibrationWriter() {}

CalibrationWriter::~CalibrationWriter() {}

void CalibrationWriter::writeBaseline(int feb_id, int hybrid_id, int channel,
                                      int sample, double baseline) {
    this->getFebNode(feb_id)->getHybridNode(hybrid_id)->writeChannelData(
            "baseline", channel, sample, baseline);
}

void CalibrationWriter::writeNoise(int feb_id, int hybrid_id, int channel,
                                   int sample, double noise) {
    this->getFebNode(feb_id)->getHybridNode(hybrid_id)->writeChannelData(
            "noise", channel, sample, noise);
}


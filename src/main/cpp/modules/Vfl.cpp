/*
 * Vfl.cpp
 *
 *  Created on: Nov 30, 2011
 *      Author: thomasstorm
 */

#include "Vfl.h"

using std::abs;
using std::fill;

Vfl::Vfl() : BasicModule("VFL") {
}

Vfl::~Vfl() {
}

void Vfl::start(Context& context) {
    collocatedSegment = &context.getSegment(Constants::SEGMENT_SYN_COLLOCATED);
}

void Vfl::process(Context& context) {

}

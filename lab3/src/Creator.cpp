#include "Creator.h"

Creator::~Creator() = default;

Creator::Creator():
        start(0),
        end(0)
{}

void Creator::setData(unsigned int start_, unsigned int end_) {
    start = start_;
    end = end_;
}

unsigned Creator::getStart() const {
    return start;
}

unsigned Creator::getEnd() const {
    return end;
}
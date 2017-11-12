#ifndef POLARCODE_POLARCODE_H
#define POLARCODE_POLARCODE_H


#include "GnCosetCode.h"
#include "Channel.h"

class PolarCode : public GnCosetCode {
public:
    PolarCode(unsigned int length, unsigned int codeDimension, const Channel &channel);

public:

};


#endif //POLARCODE_POLARCODE_H

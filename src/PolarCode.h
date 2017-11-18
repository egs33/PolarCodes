#ifndef POLARCODE_POLARCODE_H
#define POLARCODE_POLARCODE_H


#include "GnCosetCode.h"
#include "Channel.h"

class PolarCode : public GnCosetCode {
protected:
    Channel &channel;
public:
    PolarCode(unsigned int length, unsigned int codeDimension, Channel &channel);

    std::vector<int> SuccessiveCancellationDecode(std::vector<int> &y);
};


#endif //POLARCODE_POLARCODE_H

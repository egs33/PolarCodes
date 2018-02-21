#ifndef POLARCODE_POLARCODE_H
#define POLARCODE_POLARCODE_H


#include "GnCosetCode.h"
#include "channel/Channel.h"

class PolarCode : public GnCosetCode {
protected:
    const Channel &channel;
public:
    PolarCode(unsigned int length, unsigned int codeDimension, const Channel &channel);

    std::vector<int> SuccessiveCancellationDecode(const std::vector<int> &y) const;

    std::vector<int> LlrSCDecode(const std::vector<int> &y) const;
};


#endif //POLARCODE_POLARCODE_H

#ifndef POLARCODE_POLARCODE_H
#define POLARCODE_POLARCODE_H


#include "GnCosetCode.h"
#include "Channel.h"

class PolarCode : public GnCosetCode {
protected:
    const Channel &channel;
public:
    PolarCode(unsigned int length, unsigned int codeDimension, const Channel &channel,
              const std::vector<std::pair<int, int>> &sendBits = std::vector<std::pair<int, int>>());

    std::vector<int> SuccessiveCancellationDecode(const std::vector<int> &y) const;

    std::vector<int> LlrSCDecode(const std::vector<int> &y) const;

    std::vector<int> SCLDecode(const std::vector<int> &y, int L) const;

    std::vector<std::vector<int>> SCLDecode2(const std::vector<int> &y, int L) const;
};


#endif //POLARCODE_POLARCODE_H

#ifndef POLARCODE_GNCOSETCODE_H
#define POLARCODE_GNCOSETCODE_H

#include <vector>
#include "channel/Channel.h"

class GnCosetCode {
protected:
    unsigned int length;
    std::vector<int> informationSet;
    std::vector<int> frozenBits;

    GnCosetCode();

public:
    GnCosetCode(unsigned int length, std::vector<int> informationSet, std::vector<int> frozenBits);

    GnCosetCode(unsigned int length, std::vector<int> informationSet);

    std::vector<int> encode(const std::vector<int> &information) const;

    unsigned int getLength() const;

    const std::vector<int> &getInformationSet() const;

    const std::vector<int> &getFrozenBits() const;

    virtual std::vector<int> SuccessiveCancellationDecode(const std::vector<int> &y, const Channel &channel) const;

    virtual std::vector<int> LlrSCDecode(const std::vector<int> &y, const Channel &channel) const;

};


#endif //POLARCODE_GNCOSETCODE_H

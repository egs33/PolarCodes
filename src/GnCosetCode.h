#ifndef POLARCODE_GNCOSETCODE_H
#define POLARCODE_GNCOSETCODE_H

#include <vector>
#include "Channel.h"

class GnCosetCode {
protected:
    unsigned int length;
    std::vector<int> informationSet;
    std::vector<int> frozenBits;

    GnCosetCode();

public:
    GnCosetCode(unsigned int length, std::vector<int> informationSet, std::vector<int> frozenBits);

    GnCosetCode(unsigned int length, std::vector<int> informationSet);

    std::vector<int> encode(std::vector<int> information);

    unsigned int getLength() const;

    const std::vector<int> &getInformationSet() const;

    const std::vector<int> &getFrozenBits() const;

    virtual std::vector<int> SuccessiveCancellationDecode(std::vector<int> &y, Channel &channel);

};


#endif //POLARCODE_GNCOSETCODE_H

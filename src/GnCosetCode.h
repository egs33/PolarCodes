#ifndef POLARCODE_GNCOSETCODE_H
#define POLARCODE_GNCOSETCODE_H

#include <vector>

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

};


#endif //POLARCODE_GNCOSETCODE_H

#ifndef POLARCODE_GNCOSETCODE_H
#define POLARCODE_GNCOSETCODE_H

#include <vector>

class GnCosetCode {
private:
    unsigned int length;
    std::vector<int> informationSet;
    std::vector<int> frozenBits;

    std::vector<int> encodeInner(std::vector<int> u);

public:
    GnCosetCode(unsigned int length, std::vector<int> informationSet, std::vector<int> frozenBits);

    GnCosetCode(unsigned int length, std::vector<int> informationSet);

    std::vector<int> encode(std::vector<int> information);
};


#endif //POLARCODE_GNCOSETCODE_H

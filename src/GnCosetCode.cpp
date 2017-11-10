#include <stdexcept>
#include <algorithm>
#include "GnCosetCode.h"

GnCosetCode::GnCosetCode(unsigned int length, std::vector<int> informationSet, std::vector<int> frozenBits) :
        length(length), informationSet(informationSet), frozenBits(frozenBits) {
    if (length != informationSet.size() + frozenBits.size()) {
        throw std::invalid_argument("invalid length");
    }
    std::sort(this->informationSet.begin(), this->informationSet.end());
}

GnCosetCode::GnCosetCode(unsigned int length, std::vector<int> informationSet) :
        length(length), informationSet(informationSet) {
    if (length < informationSet.size()) {
        throw std::invalid_argument("invalid length");
    }
    std::sort(this->informationSet.begin(), this->informationSet.end());
    this->frozenBits = std::vector<int>(length - informationSet.size(), 0);
}

std::vector<int> GnCosetCode::encode(std::vector<int> information) {
    if (this->informationSet.size() != information.size()) {
        throw std::invalid_argument("invalid length");
    }
    std::vector<int> u(this->length);
    for (int i = 0, informationIndex = 0; i < this->length; ++i) {
        if (this->informationSet[informationIndex] == i) {
            u[i] = information[informationIndex];
            ++informationIndex;
        } else {
            u[i] = this->frozenBits[i - informationIndex];
        }
    }
    return encodeInner(u);
}

std::vector<int> GnCosetCode::encodeInner(std::vector<int> u) {
    std::vector<int> ret(u.size());
    if (u.size() == 1) {
        ret[0] = u[0];
        return ret;
    } else if (u.size() == 2) {
        ret[0] = u[0] ^ u[1];
        ret[1] = u[1];
        return ret;
    }
    std::vector<int> temp1(u.size() / 2);
    std::vector<int> temp2(u.size() / 2);
    for (int i = 0; i < u.size() / 2; i++) {
        temp1[i] = u[2 * i] ^ u[2 * i + 1];
        temp2[i] = u[2 * i + 1];
    }
    std::vector<int> temp_dest1 = encodeInner(temp1);
    std::vector<int> temp_dest2 = encodeInner(temp2);
    for (int i = 0; i < u.size() / 2; i++) {
        ret[i] = temp_dest1[i];
        ret[u.size() / 2 + i] = temp_dest2[i];
    }
    return ret;
}

#include <stdexcept>
#include <algorithm>
#include "GnCosetCode.h"
#include "Channel.h"


GnCosetCode::GnCosetCode() = default;

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
    return Channel::combine(u);
}

unsigned int GnCosetCode::getLength() const {
    return length;
}

const std::vector<int> &GnCosetCode::getInformationSet() const {
    return informationSet;
}

const std::vector<int> &GnCosetCode::getFrozenBits() const {
    return frozenBits;
}


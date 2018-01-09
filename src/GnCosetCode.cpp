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

std::vector<int> GnCosetCode::encode(const std::vector<int> &information) const {
    if (this->informationSet.size() != information.size()) {
        throw std::invalid_argument("invalid length");
    }
    std::vector<int> u(this->length);
    for (int i = 0, informationIndex = 0; i < this->length; ++i) {
        if (informationSet.size() > informationIndex && this->informationSet[informationIndex] == i) {
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

std::vector<int> GnCosetCode::SuccessiveCancellationDecode(const std::vector<int> &y, const Channel &channel) const {
    std::vector<int> decoded, ret;
    std::vector<int> information(getInformationSet().size());
    for (int i = 0, informationIndex = 0; i < getLength(); ++i) {
        if (getInformationSet().size() > informationIndex && i == getInformationSet()[informationIndex]) {
            int decodedBit = channel.w(getLength(), y, decoded, 0) >= channel.w(getLength(), y, decoded, 1) ? 0 : 1;
            decoded.push_back(decodedBit);
            ret.push_back(decodedBit);
            ++informationIndex;
        } else {
            decoded.push_back(getFrozenBits()[i - informationIndex]);
        }
    }
    return ret;
}

std::vector<int> GnCosetCode::LlrSCDecode(const std::vector<int> &y, const Channel &channel) const {
    std::vector<int> decoded, ret;
    std::vector<int> information(getInformationSet().size());
    for (int i = 0, informationIndex = 0; i < getLength(); ++i) {
        if (getInformationSet().size() > informationIndex && i == getInformationSet()[informationIndex]) {
            int decodedBit = channel.logLikelihoodRatio(getLength(), y, decoded) > 0 ? 0 : 1;
            decoded.push_back(decodedBit);
            ret.push_back(decodedBit);
            ++informationIndex;
        } else {
            decoded.push_back(getFrozenBits()[i - informationIndex]);
        }
    }
    return ret;
}

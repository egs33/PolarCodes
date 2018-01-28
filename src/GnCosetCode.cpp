#include <stdexcept>
#include <algorithm>
#include <map>
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

std::vector<int> GnCosetCode::SCLDecode(const std::vector<int> &y, const int L, const Channel &channel) const {
    std::vector<std::vector<int>*> list;
    list.push_back(new std::vector<int>);
    for (int i = 0, informationIndex = 0; i < getLength() - 1; ++i) {
        if (getInformationSet().size() <= informationIndex || i != getInformationSet()[informationIndex]) {
            // frozen bit
            for (auto &&path : list) {
                path->push_back(getFrozenBits()[i - informationIndex]);
            }
        } else {
            ++informationIndex;
            if (list.size() < L) {
                duplicatePath(list);
                continue;
            }
            std::vector<std::pair<std::vector<int>*, double>> candidateList;
            for (auto &&path : list) {
                const double llr0 = channel.w(getLength(), y, *path, 0);
                const double llr1 = channel.w(getLength(), y, *path, 1);
                auto v0 = new std::vector<int>(*path);
                auto v1 = new std::vector<int>(*path);
                v0->push_back(0);
                v1->push_back(1);
                candidateList.emplace_back(std::make_pair(v0, llr0));
                candidateList.emplace_back(std::make_pair(v1, llr1));
                delete path;
            }
            list.clear();
            std::sort(candidateList.begin(), candidateList.end(),
                      [](const std::pair<std::vector<int>*, double>& a, const std::pair<std::vector<int>*, double>& b) {
                          return a.second > b.second;
            });
            for (int j = 0; j < L; ++j) {
                list.push_back(candidateList[j].first);
            }
            for (int j = L; j < candidateList.size(); ++j) {
                delete candidateList[j].first;
            }
        }
    }
    duplicatePath(list);
    double maxLikelihood = -1;
    std::vector<int>* max = nullptr;
    for (auto &&path : list) {
        auto x = Channel::combine(*path);
        const double likelihood = channel.w(y, x);
        if (likelihood > maxLikelihood) {
            maxLikelihood = likelihood;
            max = path;
        }
    }
    std::vector<int> ret;
    for (auto &&i : getInformationSet()) {
        ret.push_back((*max)[i]);
    }
    for (auto &&path : list) {
        delete path;
    }
    return ret;
}

void GnCosetCode::duplicatePath(std::vector<std::vector<int> *> &list) const {
    const int originalSize = list.size();
    for (int j = 0; j < originalSize; ++j) {
        auto newCandidate = new std::vector<int>(*list[j]);
        list[j]->push_back(0);
        newCandidate->push_back(1);
        list.push_back(newCandidate);
    }
}

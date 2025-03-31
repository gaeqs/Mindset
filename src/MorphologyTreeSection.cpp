//
// Created by gaeqs on 26/03/2025.
//

#include <mindset/MorphologyTreeSection.h>

namespace mindset
{

    MorphologyTreeSection::MorphologyTreeSection(UID uid) :
        Identifiable(uid)
    {
    }

    MorphologyTreeSection::MorphologyTreeSection(UID uid, std::initializer_list<UID> neurites) :
        Identifiable(uid),
        _neurites(neurites)
    {
    }

    MorphologyTreeSection::MorphologyTreeSection(UID uid, std::span<const UID> neurites) :
        Identifiable(uid),
        _neurites(neurites.begin(), neurites.end())
    {
    }

    std::span<const UID> MorphologyTreeSection::getNeurites() const
    {
        return _neurites;
    }

    bool MorphologyTreeSection::containsNeurite(UID neuriteId) const
    {
        return std::ranges::find(_neurites, neuriteId) != _neurites.end();
    }

    void MorphologyTreeSection::redefineNeurites(std::span<const UID> uids)
    {
        _neurites.clear();
        _neurites.reserve(uids.size());
        for (auto id : uids) {
            _neurites.push_back(id);
        }
        incrementVersion();
    }

    void MorphologyTreeSection::addNeurite(UID uid)
    {
        _neurites.push_back(uid);
    }

    size_t MorphologyTreeSection::getNeuritesCount() const
    {
        return _neurites.size();
    }

    std::optional<UID> MorphologyTreeSection::getFistNeurite()
    {
        if (_neurites.empty()) {
            return {};
        }
        return _neurites.front();
    }

    std::optional<UID> MorphologyTreeSection::getLastNeurite()
    {
        if (_neurites.empty()) {
            return {};
        }
        return _neurites.back();
    }

    std::optional<UID> MorphologyTreeSection::getParentSection() const
    {
        return _parentSection;
    }

    const std::unordered_set<UID>& MorphologyTreeSection::getChildSections() const
    {
        return _childSections;
    }

    void MorphologyTreeSection::setParentSection(UID parent)
    {
        _parentSection = parent;
    }

    bool MorphologyTreeSection::addChildSection(UID child)
    {
        auto [it, result] = _childSections.insert(child);
        if (result) {
            incrementVersion();
        }
        return result;
    }

    void MorphologyTreeSection::removeParentSection()
    {
        _parentSection = {};
    }

    bool MorphologyTreeSection::removeChildSection(UID child)
    {
        bool result = _childSections.erase(child) > 0;
        if (result) {
            incrementVersion();
        }
        return result;
    }

} // namespace mindset
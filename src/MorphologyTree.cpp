//
// Created by gaeqs on 26/03/2025.
//

#include <mindset/MorphologyTree.h>

#include <queue>
#include <mindset/Dataset.h>
#include <mindset/DefaultProperties.h>
#include <mindset/Morphology.h>

namespace mindset
{

    MorphologyTree::MorphologyTree() = default;

    MorphologyTree::MorphologyTree(const Morphology* morphology, const Dataset& dataset)
    {
        if (morphology == nullptr || !morphology->getSoma().has_value()) {
            return;
        }
        _morphologyVersion = morphology->getVersion();

        auto parentPropOptional = dataset.getProperties().getPropertyUID(PROPERTY_PARENT);
        if (!parentPropOptional.has_value()) {
            return;
        }
        UID parentProp = parentPropOptional.value();
        auto* soma = morphology->getSoma().value();

        // First, let's compute every child of all neurites.
        std::multimap<UID, UID> _children;

        for (auto* neurite : morphology->getNeurites()) {
            if (auto parent = neurite->getProperty<UID>(parentProp); parent.has_value()) {
                _children.insert({parentProp, parentProp});
            }
        }

        UID uidGenerator = 0;

        // The pair links the parent section to the section to process.
        std::queue<std::pair<std::optional<UID>, UID>> queue;

        // Let's start with the soma!
        queue.push({{}, soma->getUID()});

        while (!queue.empty()) {
            auto [parent, firstNeurite] = queue.front();
            queue.pop();

            std::vector<UID> neuritesInSection;

            UID currentNeurite = firstNeurite;
            size_t amount = _children.count(currentNeurite);
            while (amount == 1) {
                neuritesInSection.push_back(currentNeurite);
                currentNeurite = _children.equal_range(currentNeurite).first->second;
                amount = _children.count(currentNeurite);
            }
            neuritesInSection.push_back(currentNeurite);

            MorphologyTreeSection section(uidGenerator++, neuritesInSection);

            if (parent.has_value()) {
                getSection(parent.value()).value()->addChildSection(section.getUID());
                section.setParentSection(parent.value());
            }

            // Now that we processed a section, let's add its children to the queue.
            auto [begin, end] = _children.equal_range(currentNeurite);
            for (auto it = begin; it != end; ++it) {
                queue.emplace(section.getUID(), it->second);
            }

            if (!_root.has_value()) {
                _root = section.getUID();
            }

            // Finally, add the section
            _sections.insert({section.getUID(), std::move(section)});
        }
    }

    std::optional<int64_t> MorphologyTree::getMorphologyVersion() const
    {
        return _morphologyVersion;
    }

    std::optional<MorphologyTreeSection*> MorphologyTree::getRoot()
    {
        if (!_root.has_value()) {
            return {};
        }

        return getSection(_root.value());
    }

    std::optional<const MorphologyTreeSection*> MorphologyTree::getRoot() const
    {
        if (!_root.has_value()) {
            return {};
        }

        return getSection(_root.value());
    }

    std::optional<MorphologyTreeSection*> MorphologyTree::getSection(UID uid)
    {
        auto it = _sections.find(uid);
        if (it != _sections.end()) {
            return &it->second;
        }
        return {};
    }

    std::optional<const MorphologyTreeSection*> MorphologyTree::getSection(UID uid) const
    {
        auto it = _sections.find(uid);
        if (it != _sections.end()) {
            return &it->second;
        }
        return {};
    }

    void MorphologyTree::reserveSpaceForSections(size_t amount)
    {
        _sections.reserve(amount);
    }

    std::pair<MorphologyTreeSection*, bool> MorphologyTree::addSection(MorphologyTreeSection section, bool root)
    {
        auto [it, result] = _sections.insert({section.getUID(), std::move(section)});
        if (result) {
            incrementVersion();
            if (root) {
                _root = it->second.getUID();
            }
        }
        return {&it->second, result};
    }

    bool MorphologyTree::removeNeurite(UID uid)
    {
        auto result = _sections.erase(uid) > 0;
        if (result) {
            incrementVersion();
        }
        return result;
    }

    size_t MorphologyTree::getSectionsAmount() const
    {
        return _sections.size();
    }

    std::optional<MorphologyTreeSection*> MorphologyTree::getSectionWithNeurite(UID neuriteId)
    {
        for (auto& section : _sections | std::views::values) {
            if (section.containsNeurite(neuriteId)) {
                return &section;
            }
        }

        return {};
    }

    std::optional<const MorphologyTreeSection*> MorphologyTree::getSectionWithNeurite(UID neuriteId) const
    {
        for (auto& section : _sections | std::views::values) {
            if (section.containsNeurite(neuriteId)) {
                return &section;
            }
        }

        return {};
    }

    std::vector<MorphologyTreeSection*> MorphologyTree::walkToRoot(UID sectionId)
    {
        std::vector<MorphologyTreeSection*> result;
        auto section = getSectionWithNeurite(sectionId);

        while (section.has_value()) {
            result.push_back(section.value());
            auto uid = section.value()->getParentSection();
            if (!uid.has_value()) {
                break;
            }
            section = getSectionWithNeurite(uid.value());
        }

        return result;
    }

    std::vector<const MorphologyTreeSection*> MorphologyTree::walkToRoot(UID sectionId) const
    {
        std::vector<const MorphologyTreeSection*> result;
        auto section = getSectionWithNeurite(sectionId);

        while (section.has_value()) {
            result.push_back(section.value());
            auto uid = section.value()->getParentSection();
            if (!uid.has_value()) {
                break;
            }
            section = getSectionWithNeurite(uid.value());
        }

        return result;
    }

    std::vector<UID> MorphologyTree::flatWalkToRoot(UID sectionId) const
    {
        std::vector<UID> result;
        auto section = getSectionWithNeurite(sectionId);

        while (section.has_value()) {
            auto neurites = section.value()->getNeurites();
            for (UID neurite : neurites | std::views::reverse) {
                result.push_back(neurite);
            }
            auto uid = section.value()->getParentSection();
            if (!uid.has_value()) {
                break;
            }
            section = getSectionWithNeurite(uid.value());
        }

        return result;
    }
} // namespace mindset
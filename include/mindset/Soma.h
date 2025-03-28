//
// Created by gaeqs on 15/03/25.
//

#ifndef SOMA_H
#define SOMA_H

#include <vector>
#include <set>

#include <rush/rush.h>
#include <mindset/Identifiable.h>
#include <mindset/PropertyHolder.h>

namespace mindset
{

    struct SomaNode
    {
        rush::Vec3f position;
        float radius;
    };

    class Soma : public Identifiable, public PropertyHolder
    {
        std::set<UID> _extraIds;
        std::vector<SomaNode> _nodes;

        mutable rush::Vec3f _center;
        mutable float _minRadius;
        mutable float _maxRadius;
        mutable float _meanRadius;
        mutable float _positionsRadius;
        mutable bool _dirty;

        void recalculate() const;

      public:
        /**
         * Creates a new soma using the provided UID.
         * @param uid The UID.
         */
        explicit Soma(UID uid);

        /**
         * Retrieves the extra id defining this soma.
         * In some datasets, Somas can be defined using multiple sections,
         * each one with a unique UID. This vector provides these extra UIDs.
         */
        const std::set<UID>& getExtraId() const;

        /**
         * Adds an extra id to the soma.
         */
        void addExtraId(UID uid);

        /**
         * Clears all extra ids from the soma.
         */
        void clearExtraIds();

        /**
         * Returns whether this soma is represented by the given UID.
         */
        bool isRepresentedById(UID uid) const;

        /**
         * Retrieves the soma nodes defining its geometry.
         */
        const std::vector<SomaNode>& getNodes() const;

        /**
         * Adds a node to the soma.
         */
        void addNode(SomaNode node);

        /**
         * Clears all nodes from the soma.
         */
        void clearNodes();

        /**
         * Returns the center of the soma geometry.
         */
        [[nodiscard]] rush::Vec3f getCenter() const;

        /**
         * Returns the minimum radius among soma nodes.
         */
        [[nodiscard]] float getMinRadius() const;

        /**
         * Returns the maximum radius among soma nodes.
         */
        [[nodiscard]] float getMaxRadius() const;

        /**
         * Returns the mean radius among soma nodes.
         */
        [[nodiscard]] float getMeanRadius() const;

        /**
         * Calculates the radius based on the positions of soma nodes relative to the center.
         */
        [[nodiscard]] float getPositionsRadius() const;

        /**
         * Provides a robust radius estimation, falling back to positions-based radius if necessary.
         */
        [[nodiscard]] float getBestMeanRadius() const;
    };
} // namespace mindset

#endif // SOMA_H

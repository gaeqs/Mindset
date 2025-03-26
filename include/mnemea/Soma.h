//
// Created by gaeqs on 15/03/25.
//

#ifndef SOMA_H
#define SOMA_H

#include <vector>
#include <rush/rush.h>
#include <mnemea/Identifiable.h>
#include <mnemea/PropertyHolder.h>

namespace mnemea
{

    struct SomaNode
    {
        rush::Vec3f position;
        float radius;
    };

    class Soma : public Identifiable, public PropertyHolder
    {
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
        * @param the UID.
        */
        explicit Soma(UID uid);

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
} // namespace mnemea

#endif //SOMA_H

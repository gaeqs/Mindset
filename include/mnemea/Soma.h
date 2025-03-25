//
// Created by gaeqs on 15/03/25.
//

#ifndef SOMA_H
#define SOMA_H

#include <vector>
#include <rush/rush.h>
#include <mnemea/Neurite.h>
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
        explicit Soma(UID uid);

        const std::vector<SomaNode>& getNodes() const;

        void addNode(SomaNode node);

        void clearNodes();

        [[nodiscard]] rush::Vec3f getCenter() const;

        [[nodiscard]] float getMinRadius() const;

        [[nodiscard]] float getMaxRadius() const;

        [[nodiscard]] float getMeanRadius() const;

        /**
        * A special radius calculated using the distance of the positions to the center.
        */
        [[nodiscard]] float getPositionsRadius() const;
    };
} // namespace mnemea

#endif //SOMA_H

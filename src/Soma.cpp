// Copyright (c) 2025. VG-Lab/URJC.
//
// Authors: Gael Rial Costas <gael.rial.costas@urjc.es>
//
// This file is part of Mindset <https://gitlab.gmrv.es/g.rial/mindset>
//
// This library is free software; you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License version 3.0 as published
// by the Free Software Foundation.
//
// This library is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License for more
// details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library; if not, write to the Free Software Foundation, Inc.,
// 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

#include <mindset/Soma.h>

namespace mindset
{
    void Soma::recalculate() const
    {
        if (_nodes.empty()) {
            _center = rush::Vec3f(0.0f);
            _meanRadius = 0.0f;
            _minRadius = 0.0f;
            _maxRadius = 0.0f;
            return;
        }

        _center = rush::Vec3f(0.0f);
        _meanRadius = 0.0f;
        _minRadius = _nodes[0].radius;
        _maxRadius = _nodes[0].radius;
        for (const auto& [position, radius] : _nodes) {
            _center += position;
            _meanRadius += radius;
            if (_maxRadius < radius) {
                _maxRadius = radius;
            }
            if (_minRadius > radius) {
                _minRadius = radius;
            }
        }
        _center /= _nodes.size();
        _meanRadius /= _nodes.size();

        _positionsRadius = 0.0f;

        for (const auto& [position, radius] : _nodes) {
            _positionsRadius += (_center - position).squaredLength();
        }
        _positionsRadius = std::sqrt(_positionsRadius / _nodes.size());

        // Handle SWC Special case
        if (_nodes.size() == 3) {
            float diff = (_nodes[0].position - (_nodes[1].position + _nodes[2].position) / 2.0f).squaredLength();
            if (std::abs(diff) < 0.0001f) {
                _maxRadius = _nodes[0].radius;
                _minRadius = _maxRadius;
                _meanRadius = _maxRadius;
            }
        }
    }

    Soma::Soma(UID uid) :
        Identifiable(uid),
        _center(0.0f),
        _minRadius(0.0f),
        _maxRadius(0.0f),
        _meanRadius(0.0f),
        _positionsRadius(0.0f),
        _dirty(false)
    {
    }

    const std::set<UID>& Soma::getExtraId() const
    {
        return _extraIds;
    }

    void Soma::addExtraId(UID uid)
    {
        _extraIds.insert(uid);
    }

    void Soma::clearExtraIds()
    {
        _extraIds.clear();
    }

    bool Soma::isRepresentedById(UID uid) const
    {
        return getUID() == uid || _extraIds.contains(uid);
    }

    const std::vector<SomaNode>& Soma::getNodes() const
    {
        return _nodes;
    }

    void Soma::addNode(SomaNode node)
    {
        _nodes.push_back(std::move(node));
        _dirty = true;
    }

    void Soma::clearNodes()
    {
        _nodes.clear();
        _dirty = true;
    }

    rush::Vec3f Soma::getCenter() const
    {
        if (_dirty) {
            recalculate();
        }
        return _center;
    }

    float Soma::getMinRadius() const
    {
        if (_dirty) {
            recalculate();
        }
        return _minRadius;
    }

    float Soma::getMaxRadius() const
    {
        if (_dirty) {
            recalculate();
        }
        return _maxRadius;
    }

    float Soma::getMeanRadius() const
    {
        if (_dirty) {
            recalculate();
        }
        return _meanRadius;
    }

    float Soma::getPositionsRadius() const
    {
        if (_dirty) {
            recalculate();
        }
        return _positionsRadius;
    }

    float Soma::getBestMeanRadius() const
    {
        if (_dirty) {
            recalculate();
        }
        if (std::abs(_meanRadius) < 0.0001f) {
            return _positionsRadius;
        }
        return _meanRadius;
    }
} // namespace mindset

//
// Created by gaeqs on 18/12/2024.
//

#include <mindset/util/NeuronTransform.h>

namespace mindset
{
    void NeuronTransform::recalculateIfRequired() const
    {
        if (!_dirty) {
            return;
        }
        _quat = rush::Quatf::euler(_rotation);
        _model = rush::Mat4f::model(_scale, _quat, _position);
        _normal = rush::Mat4f::normal(_scale, _quat);

        _dirty = false;
    }

    NeuronTransform::NeuronTransform() :
        _model(1),
        _normal(1),
        _position(0),
        _scale(1),
        _dirty(false)
    {
    }

    NeuronTransform::NeuronTransform(const rush::Mat4f& model) :
        _dirty(true)
    {
        _position = model[3](0, 1, 2);
        _scale.x() = model[0](0, 1, 2).toVec().length();
        _scale.y() = model[1](0, 1, 2).toVec().length();
        _scale.z() = model[2](0, 1, 2).toVec().length();

        auto rot = rush::Mat3f(model);
        rot[0] /= _scale.x();
        rot[1] /= _scale.y();
        rot[2] /= _scale.z();

        _rotation = rush::Quatf::fromRotationMatrix(rot).euler();
    }

    const rush::Mat4f& NeuronTransform::getModel() const
    {
        recalculateIfRequired();
        return _model;
    }

    const rush::Mat4f& NeuronTransform::getNormal() const
    {
        recalculateIfRequired();
        return _normal;
    }

    const rush::Vec3f& NeuronTransform::getPosition() const
    {
        return _position;
    }

    void NeuronTransform::setPosition(const rush::Vec3f& position)
    {
        if (_position == position) {
            return;
        }
        _position = position;
        _dirty = true;
    }

    const rush::Vec3f& NeuronTransform::getRotation() const
    {
        return _rotation;
    }

    void NeuronTransform::setRotation(const rush::Vec3f& rotation)
    {
        if (_rotation == rotation) {
            return;
        }
        _rotation = rotation;
        _dirty = true;
    }

    const rush::Vec3f& NeuronTransform::getScale() const
    {
        return _scale;
    }

    void NeuronTransform::setScale(const rush::Vec3f& scale)
    {
        if (_scale == scale) {
            return;
        }
        _scale = scale;
        _dirty = true;
    }

    rush::Vec3f NeuronTransform::positionToGlobalCoordinates(rush::Vec3f local) const
    {
        recalculateIfRequired();
        return _quat * (_scale * local) + _position;
    }

    rush::Vec3f NeuronTransform::vectorToGlobalCoordinates(rush::Vec3f local) const
    {
        recalculateIfRequired();
        return _quat * (_scale * local);
    }

    rush::Vec3f NeuronTransform::positionToLocalCoordinates(rush::Vec3f global) const
    {
        return _quat.conjugate() * (global - _position) / _scale;
    }

    rush::Vec3f NeuronTransform::vectorToLocalCoordinates(rush::Vec3f global) const
    {
        return _quat.conjugate() * global / _scale;
    }

} // namespace mindset

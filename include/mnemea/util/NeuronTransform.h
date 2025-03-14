//
// Created by gaeqs on 18/12/2024.
//

#ifndef NEURONTRANSFORM_H
#define NEURONTRANSFORM_H

#include <rush/rush.h>

namespace mnemea {
    class NeuronTransform {
        mutable rush::Mat4f _model;
        mutable rush::Mat4f _normal;

        rush::Vec3f _position;
        rush::Vec3f _rotation;
        rush::Vec3f _scale;

        mutable bool _dirty;

        void recalculateIfRequired() const;

    public:
        NeuronTransform();

        explicit NeuronTransform(const rush::Mat4f& model);

        [[nodiscard]] const rush::Mat4f& getModel() const;

        [[nodiscard]] const rush::Mat4f& getNormal() const;

        [[nodiscard]] const rush::Vec3f& getPosition() const;

        void setPosition(const rush::Vec3f& position);

        [[nodiscard]] const rush::Vec3f& getRotation() const;

        void setRotation(const rush::Vec3f& rotation);

        [[nodiscard]] const rush::Vec3f& getScale() const;

        void setScale(const rush::Vec3f& scale);
    };
}


#endif //NEURONTRANSFORM_H
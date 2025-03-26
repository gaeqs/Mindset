//
// Created by gaeqs on 18/12/2024.
//

#ifndef NEURONTRANSFORM_H
#define NEURONTRANSFORM_H

#include <rush/rush.h>

namespace mnemea
{
    /**
     * Manages spatial transformations (position, rotation, scale) of a neuron and computes corresponding matrices.
     */
    class NeuronTransform
    {
        mutable rush::Mat4f _model;
        mutable rush::Mat4f _normal;

        rush::Vec3f _position;
        rush::Vec3f _rotation;
        rush::Vec3f _scale;

        mutable bool _dirty;

        void recalculateIfRequired() const;

      public:
        /**
         * Constructs a NeuronTransform with default transformations (identity).
         */
        NeuronTransform();

        /**
         * Constructs a NeuronTransform based on a provided model matrix.
         * @param model Initial model matrix to define position, rotation, and scale.
         */
        explicit NeuronTransform(const rush::Mat4f& model);

        /**
         * Retrieves the computed model matrix representing the transformation.
         */
        [[nodiscard]] const rush::Mat4f& getModel() const;

        /**
         * Retrieves the normal matrix derived from the transformation, suitable for lighting calculations.
         */
        [[nodiscard]] const rush::Mat4f& getNormal() const;

        /**
         * Retrieves the current position vector.
         */
        [[nodiscard]] const rush::Vec3f& getPosition() const;

        /**
         * Sets a new position vector.
         * @param position The new position.
         */
        void setPosition(const rush::Vec3f& position);

        /**
         * Retrieves the current rotation (Euler angles).
         */
        [[nodiscard]] const rush::Vec3f& getRotation() const;

        /**
         * Sets new rotation values (Euler angles).
         * @param rotation The new rotation angles.
         */
        void setRotation(const rush::Vec3f& rotation);

        /**
         * Retrieves the current scale factors.
         */
        [[nodiscard]] const rush::Vec3f& getScale() const;

        /**
         * Sets new scale factors.
         * @param scale The new scale values.
         */
        void setScale(const rush::Vec3f& scale);
    };
} // namespace mnemea

#endif //NEURONTRANSFORM_H
//
// Created by gaeqs on 18/12/2024.
//

#ifndef NEURONTRANSFORM_H
#define NEURONTRANSFORM_H

#include <rush/rush.h>

namespace mindset
{
    /**
     * Manages spatial transformations (position, rotation, scale) of a neuron and computes corresponding matrices.
     */
    class NeuronTransform
    {
        mutable rush::Quatf _quat;
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

        /**
         * Transforms a position vector from local to global coordinate space.
         *
         * @param local The position vector defined in local coordinates.
         * @return The transformed position vector in global coordinates.
         *
         * This transformation applies scaling, rotation (by quaternion), and translation,
         * converting a position from the local frame of the NeuronTransform to the global frame.
         */
        rush::Vec3f positionToGlobalCoordinates(rush::Vec3f local) const;

        /**
         * Transforms a direction vector from local to global coordinate space.
         *
         * @param local The direction vector defined in local coordinates.
         * @return The transformed direction vector in global coordinates.
         *
         * This method applies scaling and rotation (by quaternion) without translation.
         * It's intended for vectors representing directions, velocities, or similar.
         */
        rush::Vec3f vectorToGlobalCoordinates(rush::Vec3f local) const;

        /**
         * Transforms a position vector from global to local coordinate space.
         *
         * @param global The position vector defined in global coordinates.
         * @return The transformed position vector in local coordinates.
         *
         * The method reverses the global-to-local transformation by first removing the translation,
         * followed by applying the inverse rotation (quaternion conjugate) and inverse scaling.
         */
        rush::Vec3f positionToLocalCoordinates(rush::Vec3f global) const;

        /**
         * Transforms a direction vector from global to local coordinate space.
         *
         * @param global The direction vector defined in global coordinates.
         * @return The transformed direction vector in local coordinates.
         *
         * Similar to positionToLocalCoordinates(), but excludes translation.
         * It applies inverse rotation (quaternion conjugate) and inverse scaling,
         * suitable for transforming directions or velocities.
         */
        rush::Vec3f vectorToLocalCoordinates(rush::Vec3f global) const;
    };
} // namespace mindset

#endif // NEURONTRANSFORM_H
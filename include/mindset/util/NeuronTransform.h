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
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

#ifndef LOADERSTATUS_H
#define LOADERSTATUS_H

namespace mindset
{

    /**
     * Enumeration of possible loader status types.
     */
    enum class LoaderStatusType
    {
        /// Loader is ready to start.
        READY,
        /// Loader is actively loading data.
        LOADING,
        /// Loading has completed successfully.
        DONE,
        /// Loading encountered an error.
        LOADING_ERROR
    };

    /**
     * Represents the loading status of a Loader, including progress details.
     */
    struct LoaderStatus
    {
        /// Current status of the loader.
        LoaderStatusType status;
        /// Description of the current task.
        std::string currentTask;
        /// Total number of loading stages.
        size_t stages;
        /// Number of completed stages.
        size_t stagesCompleted;
    };

} // namespace mindset

#endif //LOADERSTATUS_H

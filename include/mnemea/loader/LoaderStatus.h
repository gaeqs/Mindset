//
// Created by gaeqs on 13/03/25.
//

#ifndef LOADERSTATUS_H
#define LOADERSTATUS_H

namespace mnemea
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
        ERROR
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

} // namespace mnemea

#endif //LOADERSTATUS_H

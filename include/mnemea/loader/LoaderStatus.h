//
// Created by gaeqs on 13/03/25.
//

#ifndef LOADERSTATUS_H
#define LOADERSTATUS_H

namespace mnemea {
    enum class LoaderStatusType {
        READY,
        LOADING,
        DONE,
        ERROR
    };

    struct LoaderStatus {
        LoaderStatusType status;
        std::string currentTask;
        size_t stages;
        size_t stagesCompleted;
    };
}

#endif //LOADERSTATUS_H

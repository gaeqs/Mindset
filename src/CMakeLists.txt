if (MNEMEA_BUILD_SHARED)
    add_library(mnemea STATIC)
else ()
    add_library(mnemea SHARED)
endif ()

target_sources(mnemea PRIVATE
        Identifiable.cpp
        Neuron.cpp
        Dataset.cpp
        Node.cpp
        Properties.cpp
        PropertyHolder.cpp
        Neurite.cpp
        Synapse.cpp
        Circuit.cpp
        Morphology.cpp

        util/NeuronTransform.cpp

        loader/BlueConfigLoader.cpp
        loader/MorphoIOLoader.cpp
        loader/SWCLoader.cpp
)

target_include_directories(mnemea
        PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(mnemea PUBLIC ${LIBRARIES})

set_target_properties(mnemea PROPERTIES
        VERSION ${PROJECT_VERSION}
        SUBVERSION ${PROJECT_VERSION_MAJOR}
)

install(TARGETS mnemea
        EXPORT MnemeaTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
)
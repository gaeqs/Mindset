if (MNEMEA_BUILD_SHARED)
    add_library(Mnemea STATIC)
else ()
    add_library(Mnemea SHARED)
endif ()

target_sources(Mnemea PRIVATE
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
)

target_include_directories(Mnemea
        PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(Mnemea PUBLIC ${LIBRARIES})

set_target_properties(Mnemea PROPERTIES
        VERSION ${PROJECT_VERSION}
        SUBVERSION ${PROJECT_VERSION_MAJOR}
)

install(TARGETS Mnemea
        EXPORT MnemeaTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
)
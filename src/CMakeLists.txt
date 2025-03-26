if (MINDSET_BUILD_SHARED)
    add_library(mindset STATIC)
else ()
    add_library(mindset SHARED)
endif ()

target_sources(mindset PRIVATE
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
        Soma.cpp
        Versioned.cpp

        util/NeuronTransform.cpp

        loader/Loader.cpp
        loader/BlueConfigLoader.cpp
        loader/MorphoIOLoader.cpp
        loader/SWCLoader.cpp
        loader/XMLLoader.cpp
        loader/SnuddaLoader.cpp
        loader/LoaderRegistry.cpp
)

target_include_directories(mindset
        PUBLIC
        $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(mindset PUBLIC ${LIBRARIES})

set_target_properties(mindset PROPERTIES
        VERSION ${PROJECT_VERSION}
        SUBVERSION ${PROJECT_VERSION_MAJOR}
)

if (MINDSET_USE_BRION)
    target_compile_definitions(mindset PUBLIC MINDSET_BRION)
endif ()

install(TARGETS mindset
        EXPORT MindsetTargets
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
        RUNTIME DESTINATION bin
)
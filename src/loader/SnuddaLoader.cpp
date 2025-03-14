//
// Created by gaeqs on 14/03/25.
//

#include <mnemea/loader/SnuddaLoader.h>

struct Syn {
    int32_t sourceCell;
    int32_t destCell;
    int32_t voxelX;
    int32_t voxelY;
    int32_t voxelZ;
    int32_t hyperVoxelId;
    int32_t channelModelId;
    int32_t sourceAxonSomaDist;
    int32_t destSegId;
    int32_t destSegX;
    int32_t conductance;
    int32_t parameterId;
};

namespace mnemea {
    SnuddaLoader::SnuddaLoader(const std::filesystem::path& path)
        : _file(path, HighFive::File::ReadOnly) {}

    void SnuddaLoader::load(Dataset& dataset) const {
        auto numSynapsesDS = _file.getDataSet("network/num_synapses");
        int numSynapses = numSynapsesDS.read<int>();

        auto synapsesDS = _file.getDataSet("network/synapses");
        std::cout << synapsesDS.getDimensions()[0] << std::endl;
        std::cout << synapsesDS.getDimensions()[1] << std::endl;
        auto data = synapsesDS.read<std::vector<std::vector<int32_t>>>();
        for (auto& synapse: data) {
            for (auto& prop : synapse) {
                std::cout << prop << ", ";
            }
            std::cout  << std::endl;
        }
    }

    LoaderFactory SnuddaLoader::createFactory() {
        return LoaderFactory(
            SNUDDA_LOADER_ID,
            SNUDDA_LOADER_NAME,
            false,
            [](const std::string& name) {
                std::string extension = std::filesystem::path(name).extension().string();
                return extension == ".h5" || extension == ".hdf5";
            },
            [](LoaderFactory::FileProvider, const std::filesystem::path& path) {
                return LoaderFactory::Result(std::make_unique<SnuddaLoader>(path));
            }
        );
    }
}

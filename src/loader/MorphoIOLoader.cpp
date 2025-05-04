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

#ifdef MINDSET_BRION

    #include <mindset/loader/MorphoIOLoader.h>
    #include <mindset/DefaultProperties.h>

    #include <brain/neuron/morphology.h>
    #include <brain/circuit.h>
    #include <rush/rush.h>

namespace mindset
{
    MorphoIOLoader::MorphoIOLoader(const LoaderCreateInfo& info, std::filesystem::path path) :
        Loader(info),
        _path(std::move(path))
    {
    }

    void MorphoIOLoader::addUIDProvider(std::function<UID()> provider)
    {
        _provider = provider;
    }

    void MorphoIOLoader::load(Dataset& dataset) const
    {
        constexpr size_t STAGES = 5;
        invoke({LoaderStatusType::LOADING, "Loading morphology", STAGES, 0});
        brain::neuron::Morphology morphology(brion::URI(absolute(_path).string()));
        auto& points = morphology.getPoints();
        auto& sections = morphology.getSections();
        auto& types = morphology.getSectionTypes();

        invoke({LoaderStatusType::LOADING, "Defining properties", STAGES, 1});

        auto& properties = dataset.getProperties();
        auto propLock = dataset.writeLock();
        auto propPosition = properties.defineProperty(PROPERTY_POSITION);
        auto propRadius = properties.defineProperty(PROPERTY_RADIUS);
        auto propParent = properties.defineProperty(PROPERTY_PARENT);
        auto propType = properties.defineProperty(PROPERTY_NEURITE_TYPE);
        propLock.unlock();

        invoke({LoaderStatusType::LOADING, "Parsing neurites", STAGES, 2});

        auto result = std::make_shared<Morphology>();

        std::vector<std::optional<UID>> lastNeurites;
        lastNeurites.resize(sections.size(), 0);

        size_t idGenerator = 0;
        for (size_t i = 0; i < sections.size(); ++i) {
            int fatherSectionId = sections[i].y;
            auto sectionType = static_cast<NeuriteType>(types[i]);

            size_t startNode = sections[i].x;
            size_t endNode = i + 1 >= sections.size() ? points.size() : sections[i + 1].x;

            if (sectionType == NeuriteType::SOMA) {
                // Handle Soma creation
                Soma soma(idGenerator++);
                lastNeurites[i] = soma.getUID();
                for (size_t p = startNode; p < endNode; ++p) {
                    auto point = points[p];
                    soma.addNode({rush::Vec3f(point.x, point.y, point.z), point.w / 2.0f});
                }
                result->setSoma(std::move(soma));
            } else {
                auto previous = fatherSectionId >= 0 ? lastNeurites[fatherSectionId] : std::optional<UID>();
                for (size_t p = startNode; p < endNode; ++p) {
                    auto point = points[p];
                    Neurite neurite(idGenerator++);
                    neurite.setProperty(propPosition, rush::Vec3f(point.x, point.y, point.z));
                    neurite.setProperty(propRadius, point.w / 2.0f); // Brion returns the diameter!
                    if (previous.has_value()) {
                        neurite.setProperty(propParent, previous.value());
                    }
                    neurite.setProperty(propType, sectionType);
                    previous = neurite.getUID();
                    result->addNeurite(std::move(neurite));
                    if (p == startNode) {
                        lastNeurites[i] = previous;
                    }
                }
            }
        }

        invoke({LoaderStatusType::LOADING, "Creating neuron", STAGES, 4});

        {
            UID uid = _provider == nullptr ? 0 : _provider();
            auto lock = dataset.writeLock();
            if (auto neuron = dataset.getNeuron(uid)) {
                auto neuronLock = neuron.value()->writeLock();
                neuron.value()->setMorphology(std::move(result));
            } else {
                dataset.addNeuron(Neuron(uid, std::move(result)));
            }
        }
        invoke({LoaderStatusType::DONE, "Done", STAGES, 5});
    }

    LoaderFactory MorphoIOLoader::createFactory()
    {
        return LoaderFactory(
            MORPHO_IO_LOADER_ID, MORPHO_IO_LOADER_NAME, false, {},
            [](const std::string& name) {
                std::string extension = std::filesystem::path(name).extension().string();
                return extension == ".h5" || extension == ".swc" || extension == ".arc" || extension == ".hdf5";
            },
            [](const LoaderCreateInfo& info, const std::filesystem::path& path) {
                return FactoryResult(std::make_unique<MorphoIOLoader>(info, path));
            });
    }
} // namespace mindset

#endif

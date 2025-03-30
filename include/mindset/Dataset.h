//
// Created by gaeqs on 09/03/2025.
//

#ifndef DATASET_H
#define DATASET_H

#include <optional>
#include <unordered_map>
#include <hey/Observable.h>

#include <mindset/Contextualized.h>
#include <mindset/Versioned.h>
#include <mindset/Neuron.h>
#include <mindset/Node.h>
#include <mindset/UID.h>
#include <mindset/Properties.h>
#include <mindset/Circuit.h>
#include <mindset/Context.h>

namespace mindset
{
    /**
     * The Dataset class serves as the primary container for scene data within the Mindset library.
     *
     * This class encapsulates all essential elements of a neural scene representation, including neurons,
     * their associated properties, circuit definitions, and an optional hierarchical structure.
     */
    class Dataset final : public Versioned, public Context
    {
        std::unordered_map<UID, Neuron> _neurons;
        Properties _properties;
        Circuit _circuit;
        std::optional<Node> _hierarchy;

        hey::Observable<Neuron*> _neuronAddedEvent;
        hey::Observable<UID> _neuronRemovedEvent;
        hey::Observable<void> _clearEvent;

      public:
        /**
         * Constructs an empty Dataset object.
         */
        Dataset();

        /**
         * Reserves space for a specified number of neurons to optimize insertion performance.
         * @param amount The number of neurons to reserve space for.
         */
        void reserveSpaceForNeurons(size_t amount);

        /**
         * Returns the amount of neurons inside this dataset.
         */
        [[nodiscard]] size_t getNeuronsAmount() const;

        /**
         * Adds a new neuron to the dataset.
         * @param neuron The neuron object to insert.
         * @return A pair containing a pointer to the inserted neuron and a boolean indicating success.
         */
        std::pair<Neuron*, bool> addNeuron(Neuron neuron);

        /**
         * Removes a neuron identified by its UID from the dataset.
         * @param uid The unique identifier of the neuron.
         * @return True if the neuron was successfully removed; false otherwise.
         */
        bool removeNeuron(UID uid);

        /**
         * Retrieves a mutable pointer to a neuron by its UID, if it exists.
         * @param uid The unique identifier of the neuron.
         * @return Optional pointer to the neuron, or std::nullopt if not found.
         */
        [[nodiscard]] std::optional<Neuron*> getNeuron(UID uid);

        /**
         * Retrieves a const pointer to a neuron by its UID, if it exists.
         * @param uid The unique identifier of the neuron.
         * @return Optional const pointer to the neuron, or std::nullopt if not found.
         */
        [[nodiscard]] std::optional<const Neuron*> getNeuron(UID uid) const;

        /**
         * Retrieves a mutable reference to the dataset's properties.
         * @return Reference to the properties object.
         */
        [[nodiscard]] Properties& getProperties() override;

        /**
         * Retrieves a const reference to the dataset's properties.
         * @return Const reference to the properties object.
         */
        [[nodiscard]] const Properties& getProperties() const override;

        /**
         * Retrieves a mutable reference to the circuit representing neuron connectivity.
         * @return Reference to the circuit object.
         */
        [[nodiscard]] Circuit& getCircuit();

        /**
         * Retrieves a const reference to the circuit representing neuron connectivity.
         * @return Const reference to the circuit object.
         */
        [[nodiscard]] const Circuit& getCircuit() const;

        /**
         * Retrieves a mutable pointer to the optional hierarchy node, if it exists.
         * @return Optional pointer to the hierarchy node, or std::nullopt if hierarchy is not set.
         */
        [[nodiscard]] std::optional<Node*> getHierarchy();

        /**
         * Retrieves a const pointer to the optional hierarchy node, if it exists.
         * @return Optional const pointer to the hierarchy node, or std::nullopt if hierarchy is not set.
         */
        [[nodiscard]] std::optional<const Node*> getHierarchy() const;

        /**
         * Creates and sets a new hierarchy node with the provided UID and type.
         * If another hierarchy was already present, this method overwrites it.
         * @param uid The unique identifier for the new hierarchy node.
         * @param type A string representing the type of the hierarchy node.
         * @return Pointer to the newly created hierarchy node.
         */
        Node* createHierarchy(UID uid, std::string type);

        /**
         * Clears this dataset. This includes all properties, neurons, synapses and hierarchy.
         */
        void clear();

        /**
         * The observable that manages the event triggered when a neuron is added.
         */
        hey::Observable<Neuron*>& getNeuronAddedEvent();

        /**
         * The observable that manages the event triggered when a neuron is removed.
         */
        hey::Observable<UID>& getNeuronRemovedEvent();

        /**
         * The observable that manages the event triggered when the dataset is cleared.
         */
        hey::Observable<UID>& getClearEvent();

        /**
         * Returns a view to iterate over all stored neurons' UIDs.
         * @returns A range view of UIDs.
         */
        [[nodiscard]] auto getNeuronsUIDs() const
        {
            return _neurons | std::views::keys;
        }

        /**
         * Returns a view to iterate over all stored neurons in a mutable context.
         * @return A range view of mutable neuron references.
         */
        [[nodiscard]] auto getNeurons()
        {
            return _neurons | std::views::transform([this](auto& pair) { return Contextualized(&pair.second, this); });
        }

        /**
         * Returns a view to iterate over all stored neurons in a read-only context.
         * @return A range view of const neuron references.
         */
        [[nodiscard]] auto getNeurons() const
        {
            return _neurons | std::views::transform([this](auto& pair) { return Contextualized(&pair.second, this); });
        }
    };
} // namespace mindset

#endif // DATASET_H

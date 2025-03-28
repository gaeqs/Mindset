//
// Created by gaeqs on 27/03/25.
//

#ifndef CONTEXT_H
#define CONTEXT_H

#include <mindset/Properties.h>

namespace mindset
{

    class Context
    {
      public:
        virtual ~Context() = default;

        /**
         * Retrieves a mutable reference to the dataset's properties.
         * @return Reference to the properties object.
         */
        [[nodiscard]] virtual Properties& getProperties() = 0;

        /**
         * Retrieves a const reference to the dataset's properties.
         * @return Const reference to the properties object.
         */
        [[nodiscard]] virtual const Properties& getProperties() const = 0;
    };

} // namespace mindset

#endif // CONTEXT_H

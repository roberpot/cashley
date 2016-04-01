/*
 * Copyright 2016 Roberto García Carvajal
 *
 * This file is part of CAshley.
 * CAshley is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * CAshley is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License
 * along with CAshley. If not, see <http://www.gnu.org/licenses/>.
 */

/** \file */

#ifndef __CASHLEY_COMPONENT_H
#define __CASHLEY_COMPONENT_H

#include "common.h"

/**
 * \brief Needed by all components.
 *
 * When create new components, you have to add this on public section of the new components.
 */
#define CASHLEY_COMPONENT \
__CASHLEY_COMMON_METHOD

namespace CAshley {

    class Entity;

    /**
     * \brief Individual component of an entity.
     *
     * Each component is a simple and small amount of data.
     */
    class Component{
    public:
        /**
         * \brief Default constructor.
         * Important! do not add any initialization code to the constructor!
         * \see init().
         */
        Component();
        /**
         * \brief Default destructor.
         * Important! do not add any code to the destructor!
         * \see shutdown().
         */
        ~Component();
        /**
         * \brief Needed by the Engine.
         */
        virtual std::string get_name();
        /**
         * \brief Set the Entity _owner of the component.
         * \param o Pointer to the entity _owner of the component.
         */
        void set_owner(Entity * o);
        /**
         * \brief Gets the Entity _owner of the component.
         * \return Pointer to the entity _owner of the component.
         */
        Entity * get_owner();
        /**
         * \brief The component initialization method.
         * This method will be called when a Component is intialized.
         */
        virtual void init();
        /**
         * \brief The component shutdown method.
         * This method will be called when an component is deallocated.
         */
        virtual void shutdown();
    private:
        /** Owner of the component */
        Entity *_owner;
    };
}

#endif //__CASHLEY_COMPONENT_H

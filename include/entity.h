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

#ifndef __CASHLEY_ENTITY_H
#define __CASHLEY_ENTITY_H

#include <map>
#include <type_traits>

#include "common.h"
#include "component.h"
#include "engine.h"
#include "exceptions.h"

#define CASHLEY_ENTITY friend class CAshley::Engine;

namespace CAshley {

    class Entity {
    public:
        /**
         * \brief Default constructor.
         * Warning! Avoid to add components on construction time! We need to link the Entity to an engine first.
         * \see init().
         */
        Entity();
        /**
         * \brief Default destructor.
         * When destroying an Entity, automatically unlink from engine.
         */
        virtual ~Entity();

        /**
         * \brief This method initalizes the entity.
         * When an entity is linked to the engine, this method is automatically called.
         */
        virtual void init();

        /**
         * \brief Add a component to the Entity.
         * An Entity can not own 2 components of the same type. If the Entity is
         * initialized, the component will be initialized.
         */
        template <class T>
        void add_component() {
            if (! std::is_base_of<Component, T>::value) {
                ComponentError e("Invalid component class");
                throw e;
            }
            if (!_engine) {
                EntityError e("Entity not installed on an engine.");
                throw e;
            }
            if (has_component<T>()) {
                EntityError e("Component duplicate.");
                throw e;
            }
            std::pair<std::string, unsigned int> component_index = _engine->get_component<T>();
            _components[component_index.first] = component_index.second;
            _engine->get_component<T>(component_index.second)->set_owner(this);
            _engine->get_component<T>(component_index.second)->init();
            if (_active) {
                _engine->activate_component(component_index.first, component_index.second);
            }
        }

        /**
         * \brief Check if the Entity has a component.
         * \return true if has this component, false otherwise.
         */
        template <class T>
        bool has_component() {
            if (! std::is_base_of<Component, T>::value) {
                ComponentError e("Invalid component class");
                throw e;
            }
            T x;
            return has_component(x.get_name());
        }

        /**
         * \brief Check if the Entity has a component.
         * \param c component class string.
         * \return true if has this component, false otherwise.
         */
        bool has_component(std::string c);

        /**
         * \brief Get a pointer to a component.
         * \return A pointer to a component.
         */
        template <class T>
        T * get_component() {
            if (! std::is_base_of<Component, T>::value) {
                ComponentError e("Invalid component class");
                throw e;
            }
            T x;
            if (_components.find(x.get_name()) == _components.end()) {
                ComponentError e("Component not found");
                throw e;
            }
            return _engine->get_component<T>(_components[x.get_name()]);
        }

        /**
         * \brief Remove a component from the Entity.
         */
        template <class T>
        void remove_component() {
            if (! std::is_base_of<Component, T>::value) {
                ComponentError e("Invalid component class");
                throw e;
            }
            T x;
            if (_components.find(x.get_name()) == _components.end()) {
                ComponentError e("Component not found");
                throw e;
            }
            _remove_component(x.get_name());
        }

        /**
         * \brief Remove all components from the Entity.
         */
        void remove_components();

        /**
         * \brief Activate an Entity.
         * This activates all components and mark this as activated.
         * This makes an entity visible to processors.
         */
        void activate();

        /**
         * \brief Deactivate an Entity.
         * This deactivates all components andmark this as deactivated.
         * This makes an entity invisible to processors.
         */
        void deactivate();

        /**
         * \brief Get the activation status of the Entity.
         * \return true if active, false otherwise.
         */
        inline bool is_active() { return _active; }

        friend class Engine;

    private:
        /**
         * \brief Removes a component.
         * \param s class name of the component.
         */
        void _remove_component(std::string s);
        /**
         * \brief Is the Entity active?
         */
        bool _active;
        /**
         * \brief Linked engine.
         */
        Engine * _engine;
        /**
         * \brief Set of components of the Entity (class string, UID).
         */
        std::map<std::string, unsigned int> _components;
    };

}

#endif //__CASHLEY_ENTITY_H

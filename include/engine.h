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

#ifndef __CASHLEY_ENGINE_H
#define __CASHLEY_ENGINE_H

#include <vector>
#include <map>
#include <set>
#include <string>
#include <type_traits>

#include "cache.h"
#include "component.h"
#include "exceptions.h"
#include "processor.h"
#include "inmutablearray.h"
#include "family.h"
#include "entitylistener.h"

namespace CAshley {

    class Entity;

    /**
     * \brief Core of the system.
     *
     * Maintain all processors, listeners, entities and components.
     */
    class Engine {
    public:
        Engine();
        ~Engine();

        /**
         * \brief Allocate a component in cache.
         *
         * First, if there is not a cache for that type of component, creates a cache.
         * Second, if, in the cache has space for a component, allocate him.
         * \return std::pair with first element the componen class string and second element the UID of the component.
         */
        template <class T>
        std::pair<std::string, unsigned int> get_component() {
            T x;
            std::pair<std::string, unsigned int> r;
            r.first = x.get_name();
            _Cache * c;
            if (_components.find(r.first) == _components.end()) {
                c = new Cache<T>(100);
                _components[r.first] = c;
            } else {
                c = _components.find(x.get_name())->second;
            }
            r.second = static_cast<Cache<T> *>(c)->block_alloc();
            return r;
        }

        /**
         * \brief Get a pointer to a component.
         *
         * Get a pointer to the instance in internal cache. Important! this pointer can change,
         * do not maintain it between 2 ticks.
         * \param uid UID of the component.
         * \return Pointer to a component with UID i and type T.
         */
        template <class T>
        T * get_component(unsigned int uid) {
            if (! std::is_base_of<Component, T>::value) {
                ComponentError e("Invalid component class");
                throw e;
            }
            T x;
            _Cache * c;
            if (_components.find(x.get_name()) == _components.end()) {
                ComponentError e("Component not found");
                throw e;
            } else {
                c = _components.find(x.get_name())->second;
            }
            return static_cast<Cache<T> *>(c)->get_block(uid);
        }

        /**
         * \brief Get a pointer to a component.
         *
         * Get a pointer to the instance in internal cache. Important! this pointer can change,
         * do not maintain it between 2 ticks.
         * \param c class string of the component.
         * \param uid UID of the component.
         * \return Pointer to a component with UID i and type T.
         */
        Component * get_component(std::string c, unsigned int uid);

        /**
         * \brief Activates a component.
         *
         * When an entity is activated, his components need to be actived. This ensures data locality.
         * \param c class string of the component.
         * \param uid UID of the component.
         */
        void activate_component(std::string c, unsigned int uid);

        /**
         * \brief Activates a component.
         *
         * When an entity is deactivated, his components need to be deactived. This ensures data locality.
         * \param c class string of the component.
         * \param uid UID of the component.
         */
        void deactivate_component(std::string c, unsigned int uid);

        /**
         * \brief Free a component.
         *
         * Removes the component from cache and also defrags it. This ensures data locality.
         * \param c class string of the component.
         * \param uid UID of the component.
         */
        void remove_component(std::string c, unsigned int uid);

        /**
         * \brief Link a entity to the engine.
         *
         * The entity need to be linked to an engine to alloc and use components.
         * \param e Entity to link.
         */
        void add_entity(Entity * e);

        /**
         * \brief Unlink a entity to the engine.
         *
         * Automatically called from entity destructor. When unlinking an entity, all components are removed.
         * \param e Entity to unlink.
         */
        void remove_entity(Entity * e);

        /**
         * \brief Get an EntityArray from a Family.
         *
         * Only active entities will be returned.
         * \param f Family of entities.
         * \return An inmutable array of entities.
         */
        EntityArray get_entities_for(Family f);

        /**
         * \brief Add a processor with a priority to the engine.
         * Only one processor per type is allowed.
         * \param priority Priority of the processor.
         */
        template <class P>
        void add_processor(unsigned int priority=INT32_MAX) {
            if (! std::is_base_of<Processor, P>::value) {
                ProcessorError e("Invalid processor class");
                throw e;
            }
            Processor * p = new P;
            std::multimap<unsigned int, Processor *>::iterator it = _processors.begin(), end = _processors.end();
            for (; it != end; it++) {
                if (p->get_name() == it->second->get_name()) {
                    delete p;
                    ProcessorError e("Engine can not own two processors of the same type.");
                    throw e;
                }
            }
            p->_engine = this;
            _processors.insert(std::pair<unsigned int, Processor *>(priority, p));
        }

        /**
         * \brief Get a pointer to a processor.
         * \return Pointer to a processor.
         */
        template <class P>
        P * get_processor() {
            if (! std::is_base_of<Processor, P>::value) {
                ProcessorError e("Invalid processor class");
                throw e;
            }
            P p;
            std::multimap<unsigned int, Processor *>::iterator it = _processors.begin(), end = _processors.end();
            for (; it != end; it++) {
                if (p.get_name() == it->second->get_name()) {
                    return static_cast<P*>(it->second);
                }
            }
            ProcessorError e("Processor not found.");
            throw e;
        }

        /**
         * \brief Remove a processor from the engine.
         */
        template <class P>
        void remove_processor() {
            if (! std::is_base_of<Processor, P>::value) {
                ProcessorError e("Invalid processor class");
                throw e;
            }
            P p;
            std::multimap<unsigned int, Processor *>::iterator it = _processors.begin(), end = _processors.end();
            for (; it != end; it++) {
                if (p.get_name() == it->second->get_name()) {
                    delete it->second;
                    _processors.erase(it);
                    return;
                }
            }
            ProcessorError e("Engine do not own that processor type");
            throw e;
        }

        /**
         * \brief Add a listener to the engine.
         * The listener only will be notified for the Entities from a family and on priority asc order.
         * Important! The listener will be notified for activated and unactivated entities!
         * \param e Entity listener to link.
         * \param f Family of entities that has to notify.
         * \param priority priority of the EntityListener.
         */
        void add_listener(EntityListener * e, Family f=Family(), unsigned int priority=INT32_MAX);
        /**
         * \brief Remove an EntityListener from the engine.
         */
        void remove_listener(EntityListener * e);

        /**
         * \brief Run a tick over all the active processors.
         * Processors run_tick method is called on priority asc order.
         * Important! If any entity is removed, it will not be succesfully removed until all processors
         * are called.
         * \param delay Delay to pass to active processors run_tick method.
         */
        void run_tick(unsigned int delay);
        friend class Family;
    private:
        /**
         * \brief Remove all entities that are waiting to be removed.
         *
         * This method is called at the end of a run_tick call. This succesfully remove entites,
         * calling the EntityListeners.
         */
        void _remove_entities();
        /**
         * \brief Remove an Entity.
         *
         * Call the EntityListeners, remove components and remove the Entity.
         */
        void _remove_entity(Entity * e);
        /**
         * \brief Call the listeners for an Entity.
         * \param e Entity added / removed.
         * \param add Determine if the Entity was Added (true) or removed (false).
         */
        void _call_listeners(Entity * e, bool add=true);
        /**
         * \brief Determines if  the engine is ticking processors.
         * If the engine is ticking processors, the deletion of entities will be
         * delayed until we tick all processors.
         */
        bool _ticking;
        /**
         * \brief Entities we want to remove during a tick.
         * When engine is ticking processors, the deletion of entities will be
         * delayed until we tick all processors. When an Entity deletion is delayed,
         * the Entity is added to this vector.
         */
        std::vector<Entity *> _entities_to_remove;
        /**
         * \brief The set of all entities of the engine.
         */
        std::set<Entity *> _entities;
        /**
         * \brief The set of processors of the engine.
         * Key is the priority of the processor.
         */
        std::multimap<unsigned int, Processor *> _processors;
        /**
         * \brief Component caches of the engine.
         */
        std::map<std::string, _Cache *> _components;
        /**
         * \brief Set of EntityListeners of the engine.
         * Key is the priority of the EntityListener.
         */
        std::multimap<unsigned int, std::pair<Family, EntityListener *> > _listeners;
    };
}

#endif //__CASHLEY_ENGINE_H

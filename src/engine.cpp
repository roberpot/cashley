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

#include "../include/engine.h"
#include "../include/entity.h"
#include "../include/family.h"

namespace CAshley {

    Engine::Engine() {
        _ticking = false;
    }

    Engine::~Engine() {

    }

    Component * Engine::get_component(std::string c, unsigned int uid) {
        _Cache * _c;
        if (_components.find(c) == _components.end()) {
            ComponentError e("Component not found");
            throw e;
        } else {
            _c = _components.find(c)->second;
        }
        return static_cast<Cache<Component> *>(_c)->get_block(uid);
    }

    void Engine::activate_component(std::string c, unsigned int uid) {
        _Cache * cache;
        if (_components.find(c) == _components.end()) {
            ComponentError e("Unknown component type.");
            throw e;
        } else {
            cache = _components.find(c)->second;
        }
        cache->block_activate(uid);
    }

    void Engine::deactivate_component(std::string c, unsigned int uid) {
        _Cache * cache;
        if (_components.find(c) == _components.end()) {
            ComponentError e("Unknown component type.");
            throw e;
        } else {
            cache = _components.find(c)->second;
        }
        cache->block_deactivate(uid);
    }


    void Engine::remove_component(std::string c, unsigned int uid) {
        _Cache * cache;
        if (_components.find(c) == _components.end()) {
            ComponentError e("Unknown component type.");
            throw e;
        } else {
            cache = _components.find(c)->second;
        }
        cache->block_free(uid);
    }

    void Engine::add_entity(Entity *e) {
        if (_entities.find(e) != _entities.end()) {
            EntityError e("Entity already added.");
            throw e;
        }
        _entities.insert(e);
        e->_engine = const_cast<CAshley::Engine *>(this);
        e->init();
        _call_listeners(e);
    }

    void Engine::remove_entity(Entity *e) {
        if (_entities.find(e) == _entities.end()) {
            EntityError e("Entity not found.");
            throw e;
        }
        if (_ticking) {
            _entities_to_remove.push_back(e);
        } else {
            _remove_entity(e);
        }
    }

    EntityArray Engine::get_entities_for(Family f) {
        return f._filter_entities(_entities);
    }

    void Engine::add_listener(EntityListener * e, Family f, unsigned int priority) {
        std::multimap<unsigned int, std::pair<Family, EntityListener *> >::iterator it = _listeners.begin(), end = _listeners.end();
        for (; it != end; it++) {
            if (it->second.second == e) {
                EntityListenerError e("Entity listener already added.");
                throw e;
            }
        }
        _listeners.insert(std::pair<unsigned int, std::pair<Family, EntityListener *> >(priority, std::pair<Family, EntityListener *>(f, e)));
    }

    void Engine::remove_listener(EntityListener * e) {
        std::multimap<unsigned int, std::pair<Family, EntityListener *> >::iterator it = _listeners.begin(), end = _listeners.end();
        for (; it != end && it->second.second != e; it++);
        if (it != end) {
            _listeners.erase(it);
        } else {
            EntityListenerError e("Entity listener has not been added.");
            throw e;
        }
    }

    void Engine::run_tick(unsigned int delay) {
        _ticking = true;
        std::multimap<unsigned int, Processor *>::iterator it, end = _processors.end();
        for(it = _processors.begin(); it != end; it++) {
            if (it->second->is_running()) {
                it->second->run_tick(delay);
            }
        }
        _ticking = false;
        _remove_entities();
    }

    void Engine::_remove_entities() {
        Entity * e;
        for (unsigned int i = 0; i < _entities_to_remove.size(); i++) {
            e = _entities_to_remove[i];
            _remove_entity(e);
        }
        _entities_to_remove.clear();
    }

    void Engine::_remove_entity(Entity * e) {
        _call_listeners(e, false);
        e->remove_components();
        _entities.erase(e);
    }

    void Engine::_call_listeners(Entity * e, bool add) {
        std::multimap<unsigned int, std::pair<Family, EntityListener *> >::iterator it = _listeners.begin(), end=_listeners.end();
        for (; it != end; it++) {
            if (it->second.first._filter_entity(e, false)) {
                if (add) {
                    it->second.second->entity_added(e);
                } else {
                    it->second.second->entity_removed(e);
                }
            }
        }
    }
}


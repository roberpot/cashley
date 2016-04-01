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

#include "../include/entity.h"

namespace CAshley {

    Entity::Entity() {
        _engine = NULL;
        _active = false;
    }

    Entity::~Entity() {
        if (_engine) {
            _engine->remove_entity(this);
        }
    }

    void Entity::init() {
    }

    bool Entity::has_component(std::string c) {
        return !(_components.find(c) == _components.end());
    }

    void Entity::activate() {
        if (_active) {
            return;
        }
        _active = true;
        std::map<std::string, unsigned int>::iterator init = _components.begin(), end = _components.end();
        for (; init != end; init++) {
            _engine->activate_component(init->first, init->second);
        }
    }

    void Entity::deactivate() {
        if (!_active) {
            return;
        }
        _active = false;
        std::map<std::string, unsigned int>::iterator init = _components.begin(), end = _components.end();
        for (; init != end; init++) {
            _engine->deactivate_component(init->first, init->second);
        }
    }

    void Entity::remove_components() {
        std::map<std::string, unsigned int>::iterator init = _components.begin(), end = _components.end();
        for (; init != end; init++) {
            _remove_component(init->first);
        }
    }

    void Entity::_remove_component(std::string s) {
        _engine->get_component(s, _components[s])->shutdown();
        _engine->remove_component(s, _components[s]);
        _components.erase(s);
    }


}
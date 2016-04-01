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

#include "../include/component.h"

namespace CAshley {

    Component::Component() {

    }

    Component::~Component() {

    }

    void Component::set_owner(Entity * o) {
        _owner = o;
    }

    Entity * Component::get_owner() {
        return _owner;
    }

    void Component::init() {

    }

    void Component::shutdown() {

    }

    std::string Component::get_name() {
        std::string name = typeid(this).name();
        return name;
    }

}

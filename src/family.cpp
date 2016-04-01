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

#include <cstring>
#include "../include/family.h"
#include "../include/entity.h"

namespace CAshley {
    EntityArray Family::_filter_entities(const std::set<Entity *> & entities) {
        EntityArray v;
        std::set<Entity *>::iterator it = entities.begin(), end = entities.end();
        for (; it != end; it++) {
            if (_filter_entity(*it)) {
                v._push_back(*it);
            }
        }
        return v;
    }

    bool Family::_filter_entity(Entity * e, bool exclude_inactive) {
        if (exclude_inactive && !e->is_active()) {
            return false;
        }
        std::set<std::string>::iterator cond_it = _filter.begin(), cond_end = _filter.end();
        bool ok = true;
        for(; cond_it != cond_end && ok; cond_it++) {
            ok = e->has_component(*cond_it);
        }
        cond_it = _exclude.begin();
        cond_end = _exclude.end();
        for(; cond_it != cond_end && ok; cond_it++) {
            ok = !e->has_component(*cond_it);
        }
        for(unsigned int i = 0; i < _one.size() && ok; i++) {
            cond_it = _one[i].begin();
            cond_end = _one[i].end();
            bool _one_ok = false;
            for(; cond_it != cond_end && ! _one_ok; cond_it++) {
                _one_ok = e->has_component(*cond_it);
            }
            ok = _one_ok;
        }
        return ok;
    }
}
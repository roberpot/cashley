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

#ifndef __CASHLEY_FAMILY_H
#define __CASHLEY_FAMILY_H

#include <set>
#include <vector>
#include <string>
#include <type_traits>

#include "component.h"
#include "inmutablearray.h"

namespace CAshley {

    class Entity;

    /**
     * \brief InmutableArray of Entity *, for families.
     */
    typedef InmutableArray<Entity *> EntityArray;

    /**
     * \brief Class to query the engine about entities.
     * Make a queryset of entities. The query is not computed until
     * the engine is not asked for.
     */
    class Family {
    public:
        /**
         * \brief Filter entities that have a component.
         */
        template <class T>
        void filter() {
            if (! std::is_base_of<Component, T>::value) {
                ComponentError e("Invalid component class");
                throw e;
            }
            T t;
            _filter.insert(t.get_name());
        }

        /**
         * \brief Exclude entities that have a component.
         */
        template <class T>
        void exclude() {
            if (! std::is_base_of<Component, T>::value) {
                ComponentError e("Invalid component class");
                throw e;
            }
            T t;
            _exclude.insert(t.get_name());
        }

        /**
         * \brief Filter entities that have, at least, one of the components.
         */
        template <class T, class U>
        void one() {
            if (! std::is_base_of<Component, T>::value) {
                ComponentError e("Invalid component class");
                throw e;
            }
            if (! std::is_base_of<Component, U>::value) {
                ComponentError e("Invalid component class");
                throw e;
            }
            T t;
            U u;
            std::set<std::string> s;
            s.insert(t.get_name());
            s.insert(u.get_name());
            _one.push_back(s);
        }

        /**
         * \brief Filter entities that have, at least, one of the components.
         */
        template <class T, class U, class V>
        void one() {
            if (! std::is_base_of<Component, T>::value) {
                ComponentError e("Invalid component class");
                throw e;
            }
            if (! std::is_base_of<Component, U>::value) {
                ComponentError e("Invalid component class");
                throw e;
            }
            if (! std::is_base_of<Component, V>::value) {
                ComponentError e("Invalid component class");
                throw e;
            }
            T t;
            U u;
            V v;
            std::set<std::string> s;
            s.insert(t.get_name());
            s.insert(u.get_name());
            s.insert(v.get_name());
            _one.push_back(s);
        }

        friend class Engine;
    private:
        /**
         * \brief Return the entities that are valid for the family.
         */
        EntityArray _filter_entities(const std::set<Entity *> & entities);
        /**
         * \brief Check if a single Entity is valid for this family.
         */
        bool _filter_entity(Entity * e, bool exclude_inactive=true);
        /**
         * Set of components filtered.
         */
        std::set<std::string> _filter;
        /**
         * Set of components excluded.
         */
        std::set<std::string> _exclude;
        /**
         * Set of sets of ones.
         */
        std::vector<std::set<std::string> > _one;
    };
}

#endif //__CASHLEY_FAMILY_H

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

#ifndef __CASHLEY_ENTITYLISTENER_H
#define __CASHLEY_ENTITYLISTENER_H

namespace CAshley{

    class Entity;

    /**
     * \brief Class to inherit for EntityListeners.
     */
    class EntityListener {
    public:
        /**
         * \brief Method called when an entity is added.
         * \param e Entity added.
         */
        virtual void entity_added(Entity * e) = 0;
        /**
         * \brief Method called when an entity is removed.
         * \param e Entity removed.
         */
        virtual void entity_removed(Entity * e) = 0;
        virtual ~EntityListener() {};
    };
}

#endif //__CASHLEY_ENTITYLISTENER_H

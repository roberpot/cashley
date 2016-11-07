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

#ifndef __CASHLEY_CACHE_H
#define __CASHLEY_CACHE_H

#include <cstring>
#include <map>

#include "exceptions.h"

// TODO: Remove or rework?
#define __CASHLEY_DEBUG_MSG(x)

namespace CAshley {

    /**
     * \brief Abstract class to allow Engine store Cache * together.
     * @see Cache.
     */
    class _Cache {
    public:
        virtual void block_activate(unsigned int i) = 0;
        virtual void block_deactivate(unsigned int i) = 0;
        virtual void block_free(unsigned int i) = 0;
        virtual std::pair<void *, unsigned int> get_active_blocks() = 0;
    };

    /**
     * \brief Class to ensure data locality.
     *
     * This class ensure that enabled components are store together to
     * accelerate memory access.
     */
    template<class T>
    class Cache : public _Cache {
    public:
        /**
         * \brief Constructor to preallocate the memory of the cache.
         * \param s Size of the preallocated memory.
         */
        Cache(unsigned int s) {
            _size = s;
            _cache = new T[s];
            _active = 0;
            _allocated = 0;
            _next_id = 0;
            _typesize = sizeof(T);
        }

        /**
         * \brief Try to mark a component as used.
         *
         *  Maintain ordered the memory to ensure data locality.
         * \return UID of the component.
         */
        unsigned int block_alloc() {
            if (_allocated == _size) {
                CacheError e("Cache is full.");
                throw e;
            }
            unsigned int id = _next_id;
            _next_id++;
            _id2idx[id] = _allocated;
            _idx2id[_allocated] = id;
            _allocated++;
            return id;
        }

        /**
         * \brief Try to mark a component as not used.
         *
         * Maintain ordered the memory to ensure data locality, not fragmenting the memory.
         * \param i UID of the component to free.
         */
        void block_free(unsigned int i) {
            if (!_block_is_allocated(i)) {
                CacheError e("Trying to free an unknown block.");
                throw e;
            }
            // First deactivate.
            if (_block_is_active(i)) {
                block_deactivate(i);
            }
            // Now dealloc.
            _allocated--;
            _swap_ids(i, _idx2id[_allocated]);
            // Remove old references.
            _idx2id.erase(_allocated);
            _id2idx.erase(i);
        }

        /**
         * \brief Enables a component.
         *
         * A component not enabled, will be never on a InmutableArray.
         * \param i UID of the component to enable.
         */
        void block_activate(unsigned int i) {
            if (!_block_is_allocated(i)) {
                CacheError e("Trying to free an unknown block.");
                throw e;
            }
            if (_block_is_active(i)) {
                __CASHLEY_DEBUG_MSG("WARNING! -> Block " << i << " already active.");
                return;
            }
            // Swap.
            _swap_ids(i, _idx2id[_active]);
            _active++;
        }

        /**
         * \brief Disables a component.
         *
         * A component not enabled, will be never on a InmutableArray.
         * \param i UID of the component to enable.
         */
        void block_deactivate(unsigned int i) {
            if (!_block_is_allocated(i)) {
                CacheError e("Trying to free an unknown block.");
                __CASHLEY_DEBUG_MSG("BLOCK " << i);
                throw e;
            }
            if (!_block_is_active(i)) {
                __CASHLEY_DEBUG_MSG("WARNING! -> Block " << i << " already inactive.");
                return;
            }
            _active--;
            // Swap.
            _swap_ids(i, _idx2id[_active]);
        }

        /**
         * \brief Get a component.
         *
         * \param i UID of the component to get.
         * \return Pointer to the component.
         */
        T *get_block(unsigned int i) {

            if (!_id2idx.count(i)) {
                CacheError e("Getting an unknown block.");
                throw e;
            }
            return _cache + _typesize * _id2idx[i];
        }

        /**
         * \brief Get the list of active components and a pointer to him.
         * \return A std::pair where first element is the pointer to components and the second the count of components.
         */
        virtual std::pair<void *, unsigned int> get_active_blocks() {
            std::pair<void *, unsigned int> pair;
            pair.first = (void *)_cache;
            pair.second = _active;
            return pair;
        };

        /**
         * \brief Checks if a component is active.
         * \param i UID of the component to check.
         * \return true if the component is active, false otherwise.
         */
        bool _block_is_active(unsigned int i) {
            return _id2idx[i] < _active;
        }

        /**
         * \brief Checks if a component exists.
         * \param i UID of the component to check.
         * \return true if the component exists, false otherwise.
         */
        bool _block_is_allocated(unsigned int i) {
            return _id2idx.count(i);
        }

        /**
         * \brief Swap position of 2 components in the internal buffer.
         * This method is called when a component is activeted/deactivated/allocated/deallocated.
         * The purpose is to ensure the data locality.
         * \param i UID of the first component.
         * \param j UID of the second component.
         */
        void _swap_ids(unsigned int i, unsigned int j) {
            // Get index.
            unsigned int idx_i, idx_j;
            idx_i = _id2idx[i];
            idx_j = _id2idx[j];
            // Swap blocks. We copy directly instead of call copy constructor
            // to prevent a call to destructor.
            unsigned char *buffer = new unsigned char[sizeof(T)];
            memcpy(buffer, &_cache[idx_i], sizeof(T));
            memcpy(&_cache[idx_i], &_cache[idx_j], sizeof(T));
            memcpy(&_cache[idx_j], buffer, sizeof(T));
            delete buffer;
            // Swap references.
            _idx2id[idx_i] = j;
            _idx2id[idx_j] = i;
            _id2idx[i] = idx_j;
            _id2idx[j] = idx_i;
        }

        /**
         * \brief Pointer to the memory where the components are stored.
         *
         * The components are preallocated. Used components are stored together
         * at heading. In this set of components, active components are stored
         * together at heading.
         */
        T *_cache;
        /**
         * \brief This stores the size of the type. Needed to return a block.
         */
        unsigned int _typesize;
        /**
         * \brief This maps the UID of a component with his position on _cache.
         */
        std::map<unsigned int, unsigned int> _id2idx;
        /**
         * \brief This maps the position on _cache of a component with his UID.
         */
        std::map<unsigned int, unsigned int> _idx2id;
        /**
         * \brief Count of active components.
         */
        unsigned int _active;
        /**
         * \brief Count of used components.
         */
        unsigned int _allocated;
        /**
         * \brief UID of the next component.
         */
        unsigned int _next_id;
        /**
         * \brief Length of the _cache. AKA max of simultaneus used components.
         */
        unsigned int _size;
    };
}

#endif //__CASHLEY_CACHE_H

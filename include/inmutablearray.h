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

#ifndef __CASHLEY_INMUTABLEARRAY_H
#define __CASHLEY_INMUTABLEARRAY_H

#include <cstring>

#include "exceptions.h"

#define INMUTABLE_ARRAY_INC 10

namespace CAshley {

    class Family;

    /**
     * \brief Class for result of family querysets.
     */
    template <class T>
    class InmutableArray {
    public:
        /**
         * \brief Default constructor.
         * Alloc memory for internal data pointer.
         */
        InmutableArray() {
            _v = new T[INMUTABLE_ARRAY_INC];
            _alloc_size = INMUTABLE_ARRAY_INC;
            _size = 0;
        }

        /**
         * \brief Default destructor.
         * Remove internal data pointer.
         */
        ~InmutableArray() {
            delete _v;
        }

        /**
         * \brief Copy constructor.
         * Copy internal data pointers.
         */
        InmutableArray(InmutableArray & i) {
            _alloc_size = i._alloc_size;
            _size = i._size;
            _v = new T[_alloc_size];
            memcpy(_v, i._v, _alloc_size);
        }

        /**
         * \brief Const copy constructor.
         * Copy internal data pointers.
         */
        InmutableArray(const InmutableArray & i) {
            _alloc_size = i._alloc_size;
            _size = i._size;
            _v = new T[_alloc_size];
            memcpy(_v, i._v, _alloc_size);
        }

        /**
         * \brief Asig operator to another InmutableArray.
         * Copy internal data pointers.
         */
        InmutableArray & operator=(InmutableArray & i) {
            delete _v;
            _alloc_size = i._alloc_size;
            _size = i._size;
            _v = new T[_alloc_size];
            memcpy(_v, i._v, sizeof(T) * _alloc_size);
            return *this;
        }

        /**
         * \brief Asig operator to another const InmutableArray.
         * Copy internal data pointers.
         */
        InmutableArray & operator=(const InmutableArray & i) {
            delete _v;
            _alloc_size = i._alloc_size;
            _size = i._size;
            _v = new T[_alloc_size];
            memcpy(_v, i._v, sizeof(T) * _alloc_size);
            return *this;
        }

        /**
         * \brief Get the size of the InmutableArray.
         * \return Entity count of the InmutableArray.
         */
        inline unsigned int size() { return _size; }

        /**
         * \brief Get the Entity on a position.
         * \param i position.
         * \return Entity in position i.
         */
        T operator[](unsigned int i) {
            if (i >= _size) {
                InmutableArrayError e("Out of range.");
                throw e;
            }
            return _v[i];
        }
        friend class Family;
    private:
        /**
         * \brief Add a element to the end of the InmutableArray.
         * This method can only be called by Family.
         */
        void _push_back(T t) {
            if (_size == _alloc_size) {
                T * v = new T[INMUTABLE_ARRAY_INC + _alloc_size];
                memcpy(v, _v, _alloc_size * sizeof(T));
                delete _v;
                _v = v;
                _alloc_size += INMUTABLE_ARRAY_INC;
            }
            _v[_size] = t;
            _size++;
        }
        /**
         * \brief Internal data pointer.
         */
        T * _v;
        /**
         * \brief Current amount of data in the InmutableArray.
         */
        unsigned int _size;
        /**
         * \brief Maximun size with the current allocated memory.
         */
        unsigned int _alloc_size;
    };
}

#endif //__CASHLEY_INMUTABLEARRAY_H

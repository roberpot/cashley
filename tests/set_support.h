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

#ifndef __CASHLEY_SET_SUPPORT_H
#define __CASHLEY_SET_SUPPORT_H

#include <set>
#include "../include/cashley.h"

template <class T>
bool operator==(std::set<T> a, std::set<T> b) {
    typename std::set<T>::iterator it = a.begin(), end = a.end();
    for (; it != end; it++) {
        if (! b.count(*it)) {
            return false;
        }
    }
    return true;
}

template <class T>
    std::set<T> make_set(CAshley::InmutableArray<T> v) {
    std::set<T> s;

    for (unsigned int i = 0; i < v.size(); i++) {
        s.insert(v[i]);
    }

    return s;
}

#endif //__CASHLEY_SET_SUPPORT_H

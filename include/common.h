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

#ifndef __CASHLEY_COMMON_H
#define __CASHLEY_COMMON_H

#define _CASHLEY_VERSION_MAJOR 1
#define _CASHLEY_VERSION_MINOR 0
#define _CASHLEY_VERSION_PATCH 0

#include <string>
#include <typeinfo>


/**
 * \brief This macro puts the method get_name on involved classes.
 *
 * The method get_name is needed by the Engine for Entities and Components.
 */
#define __CASHLEY_COMMON_METHOD \
virtual std::string get_name() { \
    std::string name = typeid(this).name();\
    return name; \
}

#endif //__CASHLEY_COMMON_H

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

#ifndef __CASHLEY_ERRORS_H
#define __CASHLEY_ERRORS_H

#include <exception>

namespace CAshley {

    /**
     * \brief Base CAshley error class.
     */
    class CAshleyError : public std::exception {
    public:
        CAshleyError(const char *msg) noexcept;
        CAshleyError(const CAshleyError& e) noexcept;
        CAshleyError& operator=(const CAshleyError& e) noexcept;
        virtual ~CAshleyError() noexcept;
        const char* what() const noexcept;
    private:
        char * _msg;
    };

    /**
     * \brief Cache errors.
     */
    class CacheError : public CAshleyError {
    public:
        CacheError(const char *msg) noexcept;
    };

    /**
     * \brief Component errors.
     */
    class ComponentError : public CAshleyError {
    public:
        ComponentError(const char *msg) noexcept;
    };

    /**
     * \brief Entity errors.
     */
    class EntityError : public CAshleyError {
    public:
        EntityError(const char *msg) noexcept;
    };

    /**
     * \brief Processor errors.
     */
    class ProcessorError : public CAshleyError {
    public:
        ProcessorError(const char *msg) noexcept;
    };

    /**
     * \brief InmutableArray errors.
     */
    class InmutableArrayError : public CAshleyError {
    public:
        InmutableArrayError(const char *msg) noexcept;
    };

    /**
     * \brief EntityListener errors.
     */
    class EntityListenerError : public CAshleyError {
    public:
        EntityListenerError(const char *msg) noexcept;
    };
}

#endif //__CASHLEY_ERRORS_H

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
#include "../include/exceptions.h"

namespace CAshley {

    CAshleyError::CAshleyError(const char *msg) noexcept {
        _msg = new char[strlen(msg) + 1];
        strcpy(_msg, msg);
    }

    CAshleyError::CAshleyError(const CAshleyError& e) noexcept {
        _msg = new char[strlen(e._msg) + 1];
        strcpy(_msg, e._msg);
    }

    CAshleyError& CAshleyError::operator=(const CAshleyError& e) noexcept {
        if (_msg) {
            delete _msg;
        }
        _msg = new char[strlen(e._msg) + 1];
        strcpy(_msg, e._msg);
        return *this;
    }

    CAshleyError::~CAshleyError() noexcept {
        if (_msg) {
            delete _msg;
        }
    }

    const char* CAshleyError::what() const noexcept {
        return _msg;
    }

    CacheError::CacheError(const char *msg) noexcept : CAshleyError(msg) {
    }

    ComponentError::ComponentError(const char *msg) noexcept : CAshleyError(msg) {
    }

    EntityError::EntityError(const char *msg) noexcept : CAshleyError(msg) {
    }

    ProcessorError::ProcessorError(const char *msg) noexcept : CAshleyError(msg) {
    }

    InmutableArrayError::InmutableArrayError(const char *msg) noexcept : CAshleyError(msg) {
    }

    EntityListenerError::EntityListenerError(const char *msg) noexcept : CAshleyError(msg) {
    }
}

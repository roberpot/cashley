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

#ifndef __CASHLEY_COMPONENTTESTS_H
#define __CASHLEY_COMPONENTTESTS_H

#include <cxxtest/TestSuite.h>
#include "../include/cashley.h"

class ComponentTestSuite : public CxxTest::TestSuite {
public:
    class TestComponent : public CAshley::Component {
    public:
        bool initialized;
        TestComponent() : initialized(false) {}
        void init() { initialized = true; }
        CASHLEY_COMPONENT
    };

    class TestEntity : public CAshley::Entity {
    public:
        void init() {
            add_component<TestComponent>();
        }
        CASHLEY_ENTITY
    };

    CAshley::Engine * engine;
    TestEntity * entity;

    void setUp() {
        engine = new CAshley::Engine;
        entity = new TestEntity;
        engine->add_entity(entity);
    }

    void tearDown() {
        delete entity;
        delete engine;
    }

    void test_component_init(void) {
        TS_ASSERT(entity->get_component<TestComponent>()->initialized == true);
    }

    void test_component_get_owner(void) {
        TS_ASSERT(entity->get_component<TestComponent>()->get_owner() == entity);
    }
};

#endif //__CASHLEY_COMPONENTTESTS_H

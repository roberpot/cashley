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

#ifndef __CASHLEY_ENTITYTESTS_H
#define __CASHLEY_ENTITYTESTS_H

#include <cxxtest/TestSuite.h>
#include "../include/cashley.h"
#include "common.h"


class EntityTestSuite : public CxxTest::TestSuite {
public:
    class TestComponent : public CAshley::Component {
    public:
        unsigned int counter;
        TestComponent() : counter(0) {}
        CASHLEY_COMPONENT
    };

    class TestEntity1 : public CAshley::Entity {
    public:
        CASHLEY_ENTITY
    };

    class TestEntity2 : public CAshley::Entity {
    public:
        CASHLEY_ENTITY

        void init() {
            add_component<TestComponent>();
        }
    };

    class TestProcessor : public CAshley::Processor {
    public:
        void run_tick(unsigned int delay) {
            UNREFERENCED_PARAMETER(delay);
            CAshley::Family f;
            f.filter<TestComponent>();
            CAshley::EntityArray v = _engine->get_entities_for(f);
            for (unsigned int i = 0; i < v.size(); i++) {
                v[i]->get_component<TestComponent>()->counter++;
            }
        }
        CASHLEY_PROCESSOR
    };

    CAshley::Engine * engine;
    TestEntity1 * entity1;
    TestEntity2 * entity2;

    void setUp() {
        engine = new CAshley::Engine;
        entity1 = new TestEntity1;
        entity2 = new TestEntity2;
    }

    void tearDown() {
        delete entity1;
        delete entity2;
        delete engine;
    }

    void test_entity_add_component(void) {
        TS_ASSERT_THROWS(entity1->add_component<TestComponent>(), CAshley::EntityError);
        engine->add_entity(entity1);
        TS_ASSERT_THROWS_NOTHING(entity1->add_component<TestComponent>());
        TS_ASSERT_THROWS(entity1->add_component<TestComponent>(), CAshley::EntityError);
    }

    void test_entity_has_component(void) {
        engine->add_entity(entity1);
        TS_ASSERT(entity1->has_component<TestComponent>() == false);
        entity1->add_component<TestComponent>();
        TS_ASSERT(entity1->has_component<TestComponent>() == true);
    }

    void test_entity_remove_component(void) {
        TS_ASSERT_THROWS(entity1->remove_component<TestComponent>(), CAshley::ComponentError);
        engine->add_entity(entity1);
        TS_ASSERT_THROWS(entity1->remove_component<TestComponent>(), CAshley::ComponentError);
        entity1->add_component<TestComponent>();
        TS_ASSERT_THROWS_NOTHING(entity1->remove_component<TestComponent>());
        TS_ASSERT_THROWS(entity1->remove_component<TestComponent>(), CAshley::ComponentError);
    }

    void test_entity_init(void) {
        TS_ASSERT(entity2->has_component<TestComponent>() == false);
        engine->add_entity(entity2);
        TS_ASSERT(entity2->has_component<TestComponent>() == true);
    }

    void test_entity_remove_components() {
        engine->add_entity(entity2);
        TS_ASSERT(entity2->has_component<TestComponent>() == true);
        entity2->remove_components();
        TS_ASSERT(entity2->has_component<TestComponent>() == false);
    }

    void test_entity_activate() {
        engine->add_entity(entity2);
        engine->add_processor<TestProcessor>();
        engine->get_processor<TestProcessor>()->activate();
        engine->run_tick(1);
        TS_ASSERT(entity2->get_component<TestComponent>()->counter == 0);
        entity2->activate();
        engine->run_tick(1);
        TS_ASSERT(entity2->get_component<TestComponent>()->counter == 1);
    }

    void test_entity_deactivate() {
        engine->add_entity(entity2);
        engine->add_processor<TestProcessor>();
        engine->get_processor<TestProcessor>()->activate();
        engine->run_tick(1);
        TS_ASSERT(entity2->get_component<TestComponent>()->counter == 0);
        entity2->activate();
        engine->run_tick(1);
        TS_ASSERT(entity2->get_component<TestComponent>()->counter == 1);
        entity2->deactivate();
        engine->run_tick(1);
        TS_ASSERT(entity2->get_component<TestComponent>()->counter == 1);
    }
};
#endif //__CASHLEY_ENTITYTESTS_H

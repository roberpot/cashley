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

#ifndef __CASHLEY_ENTITYLISTENERTESTS_H
#define __CASHLEY_ENTITYLISTENERTESTS_H

#include <cxxtest/TestSuite.h>
#include "../include/cashley.h"
#include "common.h"

class EntityListenerTestSuite : public CxxTest::TestSuite {
public:
    class TestEntity : public CAshley::Entity {
    public:
        CASHLEY_ENTITY
    };
    class TestEntityListener : public CAshley::EntityListener {
    public:
        unsigned int add_counter, remove_counter;
        TestEntityListener() : add_counter(0), remove_counter(0) {}
        void entity_added(CAshley::Entity * e) { UNREFERENCED_PARAMETER(e); add_counter++; }
        void entity_removed(CAshley::Entity * e) { UNREFERENCED_PARAMETER(e); remove_counter++; }
    };

    void test_entitylistener_entity_add(void) {
        CAshley::Engine engine;
        TestEntityListener * el = new TestEntityListener;
        engine.add_listener(el);
        TS_ASSERT(el->add_counter == 0);
        CAshley::Entity * entities[5];
        for(unsigned int i = 0; i < 5; i++) {
            entities[i] = new TestEntity;
            engine.add_entity(entities[i]);
            TS_ASSERT(el->add_counter == (i + 1));
        }
        for(unsigned int i = 0; i < 5; i++) {
            delete entities[i];
        }
        engine.remove_listener(el);
        delete el;
    }

    void test_entitylistener_entity_remove(void) {
        CAshley::Engine engine;
        TestEntityListener * el = new TestEntityListener;
        engine.add_listener(el);
        TS_ASSERT(el->add_counter == 0);
        CAshley::Entity * entities[5];
        for(unsigned int i = 0; i < 5; i++) {
            entities[i] = new TestEntity;
            engine.add_entity(entities[i]);
        }
        TS_ASSERT(el->remove_counter == 0);
        for(unsigned int i = 0; i < 5; i++) {
            delete entities[i];
            TS_ASSERT(el->remove_counter == i + 1);
        }
        engine.remove_listener(el);
        delete el;
    }
};


#endif //__CASHLEY_ENTITYLISTENERTESTS_H

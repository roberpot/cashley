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

#ifndef __CASHLEY_FAMILYTESTS_H
#define __CASHLEY_FAMILYTESTS_H

#include <cxxtest/TestSuite.h>
#include "../include/cashley.h"
#include "set_support.h"

class FamilyTestSuite : public CxxTest::TestSuite {
public:
    class TestComponent1 : public CAshley::Component {
    public:
        CASHLEY_COMPONENT
    };
    class TestComponent2 : public CAshley::Component {
    public:
        CASHLEY_COMPONENT
    };
    class TestComponent3 : public CAshley::Component {
    public:
        CASHLEY_COMPONENT
    };
    class TestEntity1 : public CAshley::Entity {
    public:
        CASHLEY_ENTITY
        void init() {
            add_component<TestComponent1>();
        }
    };
    class TestEntity2 : public CAshley::Entity {
    public:
        CASHLEY_ENTITY
        void init() {
            add_component<TestComponent2>();
        }
    };
    class TestEntity3 : public CAshley::Entity {
    public:
        CASHLEY_ENTITY
        void init() {
            add_component<TestComponent3>();
        }
    };
    class TestEntity4 : public CAshley::Entity {
    public:
        CASHLEY_ENTITY
        void init() {
            add_component<TestComponent1>();
            add_component<TestComponent2>();
        }
    };
    class TestEntity5 : public CAshley::Entity {
    public:
        CASHLEY_ENTITY
        void init() {
            add_component<TestComponent1>();
            add_component<TestComponent3>();
        }
    };
    class TestEntity6 : public CAshley::Entity {
    public:
        CASHLEY_ENTITY
        void init() {
            add_component<TestComponent2>();
            add_component<TestComponent3>();
        }
    };
    class TestEntity7 : public CAshley::Entity {
    public:
        CASHLEY_ENTITY
        void init() {
            add_component<TestComponent1>();
            add_component<TestComponent2>();
            add_component<TestComponent3>();
        }
    };
    CAshley::Engine * engine;
    CAshley::Entity * entities[7];

    void setUp() {
        engine = new CAshley::Engine;
        entities[0] = new TestEntity1;
        entities[1] = new TestEntity2;
        entities[2] = new TestEntity3;
        entities[3] = new TestEntity4;
        entities[4] = new TestEntity5;
        entities[5] = new TestEntity6;
        entities[6] = new TestEntity7;
        for (unsigned int i = 0; i < 7; i++) {
            engine->add_entity(entities[i]);
            entities[i]->activate();
        }
    }

    void tearDown() {
        for (unsigned int i = 0; i < 7; i++) {
            delete entities[i];
        }
        delete engine;
    }

    void test_family_full(void) {
        CAshley::Family f;
        CAshley::EntityArray v;
        v = engine->get_entities_for(f);
        TS_ASSERT(v.size() == 7);
        std::set<CAshley::Entity *> sf;
        sf.insert(entities[0]);
        sf.insert(entities[1]);
        sf.insert(entities[2]);
        sf.insert(entities[3]);
        sf.insert(entities[4]);
        sf.insert(entities[5]);
        sf.insert(entities[6]);
        TS_ASSERT(make_set(v) == sf);
    }

    void test_family_filter(void) {
        CAshley::Family f1, f2, f3, f4, f5, f6, f7;
        CAshley::EntityArray v;
        f1.filter<TestComponent1>();
        f2.filter<TestComponent2>();
        f3.filter<TestComponent3>();
        f4.filter<TestComponent1>();
        f4.filter<TestComponent2>();
        f5.filter<TestComponent1>();
        f5.filter<TestComponent3>();
        f6.filter<TestComponent2>();
        f6.filter<TestComponent3>();
        f7.filter<TestComponent1>();
        f7.filter<TestComponent2>();
        f7.filter<TestComponent3>();
        v = engine->get_entities_for(f1);
        TS_ASSERT(v.size() == 4);
        std::set<CAshley::Entity *> sf1;
        sf1.insert(entities[0]);
        sf1.insert(entities[3]);
        sf1.insert(entities[4]);
        sf1.insert(entities[6]);
        TS_ASSERT(make_set(v) == sf1);
        v = engine->get_entities_for(f2);
        TS_ASSERT(v.size() == 4);
        std::set<CAshley::Entity *> sf2;
        sf2.insert(entities[1]);
        sf2.insert(entities[3]);
        sf2.insert(entities[5]);
        sf2.insert(entities[6]);
        TS_ASSERT(make_set(v) == sf2);
        v = engine->get_entities_for(f3);
        TS_ASSERT(v.size() == 4);
        std::set<CAshley::Entity *> sf3;
        sf3.insert(entities[2]);
        sf3.insert(entities[4]);
        sf3.insert(entities[5]);
        sf3.insert(entities[6]);
        TS_ASSERT(make_set(v) == sf3);
        v = engine->get_entities_for(f4);
        TS_ASSERT(v.size() == 2);
        std::set<CAshley::Entity *> sf4;
        sf4.insert(entities[3]);
        sf4.insert(entities[6]);
        TS_ASSERT(make_set(v) == sf4);
        v = engine->get_entities_for(f5);
        TS_ASSERT(v.size() == 2);
        std::set<CAshley::Entity *> sf5;
        sf5.insert(entities[4]);
        sf5.insert(entities[6]);
        TS_ASSERT(make_set(v) == sf5);
        v = engine->get_entities_for(f6);
        TS_ASSERT(v.size() == 2);
        std::set<CAshley::Entity *> sf6;
        sf6.insert(entities[5]);
        sf6.insert(entities[6]);
        TS_ASSERT(make_set(v) == sf6);
        v = engine->get_entities_for(f7);
        TS_ASSERT(v.size() == 1);
        std::set<CAshley::Entity *> sf7;
        sf7.insert(entities[6]);
        TS_ASSERT(make_set(v) == sf7);
    }

    void test_family_exclude(void) {
        CAshley::Family f1, f2, f3, f4, f5, f6, f7;
        CAshley::EntityArray v;
        f1.exclude<TestComponent1>();
        f2.exclude<TestComponent2>();
        f3.exclude<TestComponent3>();
        f4.exclude<TestComponent1>();
        f4.exclude<TestComponent2>();
        f5.exclude<TestComponent1>();
        f5.exclude<TestComponent3>();
        f6.exclude<TestComponent2>();
        f6.exclude<TestComponent3>();
        f7.exclude<TestComponent1>();
        f7.exclude<TestComponent2>();
        f7.exclude<TestComponent3>();
        v = engine->get_entities_for(f1);
        TS_ASSERT(v.size() == 3);
        std::set<CAshley::Entity *> sf1;
        sf1.insert(entities[1]);
        sf1.insert(entities[2]);
        sf1.insert(entities[5]);
        TS_ASSERT(make_set(v) == sf1);
        v = engine->get_entities_for(f2);
        TS_ASSERT(v.size() == 3);
        std::set<CAshley::Entity *> sf2;
        sf2.insert(entities[0]);
        sf2.insert(entities[2]);
        sf2.insert(entities[4]);
        TS_ASSERT(make_set(v) == sf2);
        v = engine->get_entities_for(f3);
        TS_ASSERT(v.size() == 3);
        std::set<CAshley::Entity *> sf3;
        sf3.insert(entities[0]);
        sf3.insert(entities[1]);
        sf3.insert(entities[3]);
        TS_ASSERT(make_set(v) == sf3);
        v = engine->get_entities_for(f4);
        TS_ASSERT(v.size() == 1);
        std::set<CAshley::Entity *> sf4;
        sf4.insert(entities[2]);
        TS_ASSERT(make_set(v) == sf4);
        v = engine->get_entities_for(f5);
        TS_ASSERT(v.size() == 1);
        std::set<CAshley::Entity *> sf5;
        sf5.insert(entities[1]);
        TS_ASSERT(make_set(v) == sf5);
        v = engine->get_entities_for(f6);
        TS_ASSERT(v.size() == 1);
        std::set<CAshley::Entity *> sf6;
        sf6.insert(entities[0]);
        TS_ASSERT(make_set(v) == sf6);
        v = engine->get_entities_for(f7);
        TS_ASSERT(v.size() == 0);
    }

    void test_family_one(void) {
        CAshley::Family f4, f5, f6, f7;
        CAshley::EntityArray v;
        f4.one<TestComponent1, TestComponent2>();
        f5.one<TestComponent1, TestComponent3>();
        f6.one<TestComponent2, TestComponent3>();
        f7.one<TestComponent1, TestComponent2, TestComponent3>();
        v = engine->get_entities_for(f4);
        TS_ASSERT(v.size() == 6);
        std::set<CAshley::Entity *> sf4;
        sf4.insert(entities[0]);
        sf4.insert(entities[1]);
        sf4.insert(entities[3]);
        sf4.insert(entities[4]);
        sf4.insert(entities[5]);
        sf4.insert(entities[6]);
        TS_ASSERT(make_set(v) == sf4);
        v = engine->get_entities_for(f5);
        TS_ASSERT(v.size() == 6);
        std::set<CAshley::Entity *> sf5;
        sf5.insert(entities[0]);
        sf5.insert(entities[2]);
        sf5.insert(entities[3]);
        sf5.insert(entities[4]);
        sf5.insert(entities[5]);
        sf5.insert(entities[6]);
        TS_ASSERT(make_set(v) == sf5);
        v = engine->get_entities_for(f6);
        TS_ASSERT(v.size() == 6);
        std::set<CAshley::Entity *> sf6;
        sf6.insert(entities[1]);
        sf6.insert(entities[2]);
        sf6.insert(entities[3]);
        sf6.insert(entities[4]);
        sf6.insert(entities[5]);
        sf6.insert(entities[6]);
        TS_ASSERT(make_set(v) == sf6);
        v = engine->get_entities_for(f7);
        TS_ASSERT(v.size() == 7);
        std::set<CAshley::Entity *> sf7;
        sf7.insert(entities[0]);
        sf7.insert(entities[1]);
        sf7.insert(entities[2]);
        sf7.insert(entities[3]);
        sf7.insert(entities[4]);
        sf7.insert(entities[5]);
        sf7.insert(entities[6]);
        TS_ASSERT(make_set(v) == sf7);
    }
};

#endif //__CASHLEY_FAMILYTESTS_H

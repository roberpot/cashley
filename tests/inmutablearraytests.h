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

#ifndef __CASHLEY_INMUTABLEARRAYTESTS_H
#define __CASHLEY_INMUTABLEARRAYTESTS_H

#include <cxxtest/TestSuite.h>
#include "../include/cashley.h"


class InmutableArrayTestSuite : public CxxTest::TestSuite {
public:
    class TestComponent : public CAshley::Component {
    public:
        CASHLEY_COMPONENT
    };

    class TestEntity : public CAshley::Entity {
    public:
        CASHLEY_ENTITY
        void init() {
            add_component<TestComponent>();
        }
    };

    void test_inmutablearray_constructor_1(void) {
        CAshley::InmutableArray<unsigned int> v;
        TS_ASSERT(v.size() == 0);
    }

    void test_inmutablearray_constructor_2(void) {
        CAshley::Engine engine;
        TestEntity * e1 = new TestEntity, * e2 = new TestEntity;
        engine.add_entity(e1);
        engine.add_entity(e2);
        e1->activate();
        e2->activate();
        CAshley::Family f;
        f.filter<TestComponent>();
        CAshley::InmutableArray<CAshley::Entity *> v(engine.get_entities_for(f));
        TS_ASSERT(v.size() == 2);
    }

    void test_inmutablearray_operator_asig(void) {
        CAshley::Engine engine;
        TestEntity * e1 = new TestEntity, * e2 = new TestEntity;
        engine.add_entity(e1);
        engine.add_entity(e2);
        e1->activate();
        e2->activate();
        CAshley::Family f;
        f.filter<TestComponent>();
        CAshley::InmutableArray<CAshley::Entity *> v1, v2(engine.get_entities_for(f));
        v1 = v2;
        TS_ASSERT(v1.size() == v2.size());
        TS_ASSERT(v1.size() == 2);
        TS_ASSERT(v1[0] == v2[0]);
        TS_ASSERT(v1[1] == v2[1]);
    }

    void test_inmutablearray_operator_subscript(void) {
        CAshley::Engine engine;
        TestEntity * e1 = new TestEntity, * e2 = new TestEntity;
        engine.add_entity(e1);
        engine.add_entity(e2);
        e1->activate();
        e2->activate();
        CAshley::Family f;
        f.filter<TestComponent>();
        CAshley::InmutableArray<CAshley::Entity *> v1(engine.get_entities_for(f));
        TS_ASSERT_THROWS_NOTHING(v1[0]);
        TS_ASSERT_THROWS_NOTHING(v1[1]);
        TS_ASSERT_THROWS(v1[2], CAshley::InmutableArrayError);
    }
};

#endif //__CASHLEY_INMUTABLEARRAYTESTS_H

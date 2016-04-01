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

#ifndef __CASHLEY_ENGINETESTS_H
#define __CASHLEY_ENGINETESTS_H

#include <string>
#include <cxxtest/TestSuite.h>
#include "../include/cashley.h"
#include "common.h"

class EngineTestSuite : public CxxTest::TestSuite {
public:
    class TestComponent : public CAshley::Component {
    public:
        CASHLEY_COMPONENT
    };
    class TestEntity : public CAshley::Entity {
    public:
        CASHLEY_ENTITY
    };
    class TestProcessor1 : public CAshley::Processor {
    public:
        unsigned int * _x;
        void set_x(unsigned int * x) { _x = x; }
        virtual void run_tick(unsigned int delay) {
            UNREFERENCED_PARAMETER(delay);
            *_x *= 10;
        }
        CASHLEY_PROCESSOR
    };
    class TestProcessor2 : public CAshley::Processor {
    public:
        unsigned int * _x;
        void set_x(unsigned int * x) { _x = x; }
        virtual void run_tick(unsigned int delay) {
            UNREFERENCED_PARAMETER(delay);
            *_x += 1;
        }
        CASHLEY_PROCESSOR
    };
    class TestEntityListener : public CAshley::EntityListener {
    public:
        void entity_added(CAshley::Entity * e) { UNREFERENCED_PARAMETER(e); }
        void entity_removed(CAshley::Entity * e) { UNREFERENCED_PARAMETER(e); }
    };

    CAshley::Engine * engine;

    void setUp() {
        engine = new CAshley::Engine;
    }

    void tearDown() {
        delete engine;
    }

    void test_engine_get_component_1() {
        std::pair<std::string, unsigned int> x, y;
        x = engine->get_component<TestComponent>();
        y = engine->get_component<TestComponent>();
        TS_ASSERT(x.second == (y.second - 1));
        TS_ASSERT(x.first == y.first);
    }

    void test_engine_get_component_2() {
        std::pair<std::string, unsigned int> x, y;
        x = engine->get_component<TestComponent>();
        y = engine->get_component<TestComponent>();
        TS_ASSERT_THROWS_NOTHING(engine->get_component<TestComponent>(x.second));
        TS_ASSERT_THROWS_NOTHING(engine->get_component<TestComponent>(y.second));
        TS_ASSERT(engine->get_component<TestComponent>(x.second) == engine->get_component<TestComponent>(x.second));
        TS_ASSERT(engine->get_component<TestComponent>(y.second) != engine->get_component<TestComponent>(x.second));
        TS_ASSERT_THROWS(engine->get_component<TestComponent>(1000), CAshley::CacheError);
    }

    void test_engine_get_component_3() {
        std::pair<std::string, unsigned int> x, y;
        std::string key = "error";
        x = engine->get_component<TestComponent>();
        y = engine->get_component<TestComponent>();
        TS_ASSERT_THROWS_NOTHING(engine->get_component(x.first, x.second));
        TS_ASSERT_THROWS_NOTHING(engine->get_component(y.first, y.second));
        TS_ASSERT(engine->get_component(x.first, x.second) == engine->get_component(x.first, x.second));
        TS_ASSERT(engine->get_component(x.first, x.second) != engine->get_component(y.first, y.second));
        TS_ASSERT_THROWS(engine->get_component(key, 1000), CAshley::ComponentError);
        TS_ASSERT_THROWS(engine->get_component(key, x.second), CAshley::ComponentError);
        TS_ASSERT_THROWS(engine->get_component(x.first, 1000), CAshley::CacheError);
    }

    void test_engine_activate_component() {
        std::pair<std::string, unsigned int> x;
        std::string key = "error";
        x = engine->get_component<TestComponent>();
        TS_ASSERT_THROWS_NOTHING(engine->activate_component(x.first, x.second));
        TS_ASSERT_THROWS_NOTHING(engine->activate_component(x.first, x.second));
        TS_ASSERT_THROWS(engine->activate_component(key, 1000), CAshley::ComponentError);
        TS_ASSERT_THROWS(engine->activate_component(key, x.second), CAshley::ComponentError);
        TS_ASSERT_THROWS(engine->activate_component(x.first, 1000), CAshley::CacheError);
    }

    void test_engine_deactivate_component() {
        std::pair<std::string, unsigned int> x;
        std::string key = "error";
        x = engine->get_component<TestComponent>();
        TS_ASSERT_THROWS_NOTHING(engine->deactivate_component(x.first, x.second));
        TS_ASSERT_THROWS_NOTHING(engine->activate_component(x.first, x.second));
        TS_ASSERT_THROWS_NOTHING(engine->deactivate_component(x.first, x.second));
        TS_ASSERT_THROWS_NOTHING(engine->deactivate_component(x.first, x.second));
        TS_ASSERT_THROWS(engine->deactivate_component(key, 1000), CAshley::ComponentError);
        TS_ASSERT_THROWS(engine->deactivate_component(key, x.second), CAshley::ComponentError);
        TS_ASSERT_THROWS(engine->deactivate_component(x.first, 1000), CAshley::CacheError);
    }

    void test_engine_remove_component() {
        std::pair<std::string, unsigned int> x;
        std::string key = "error";
        TS_ASSERT_THROWS(engine->remove_component(key, 1000), CAshley::ComponentError);
        x = engine->get_component<TestComponent>();
        TS_ASSERT_THROWS(engine->remove_component(key, x.second), CAshley::ComponentError);
        TS_ASSERT_THROWS(engine->remove_component(x.first, 1000), CAshley::CacheError);
        TS_ASSERT_THROWS_NOTHING(engine->remove_component(x.first, x.second));
        TS_ASSERT_THROWS(engine->remove_component(x.first, x.second), CAshley::CacheError);
        x = engine->get_component<TestComponent>();
        engine->activate_component(x.first, x.second);
        TS_ASSERT_THROWS_NOTHING(engine->remove_component(x.first, x.second));
        TS_ASSERT_THROWS(engine->remove_component(x.first, x.second), CAshley::CacheError);
        x = engine->get_component<TestComponent>();
        engine->activate_component(x.first, x.second);
        engine->deactivate_component(x.first, x.second);
        TS_ASSERT_THROWS_NOTHING(engine->remove_component(x.first, x.second));
        TS_ASSERT_THROWS(engine->remove_component(x.first, x.second), CAshley::CacheError);
    }

    void test_engine_add_entity() {
        TestEntity * e = new TestEntity;
        CAshley::Family f;
        CAshley::EntityArray v;
        TS_ASSERT_THROWS_NOTHING(engine->add_entity(e));
        e->activate();
        v = engine->get_entities_for(f);
        TS_ASSERT(v.size() == 1);
        TS_ASSERT_THROWS(engine->add_entity(e), CAshley::EntityError);
        v = engine->get_entities_for(f);
        TS_ASSERT(v.size() == 1);
    }

    void test_engine_remove_entity() {
        TestEntity * e = new TestEntity;
        CAshley::Family f;
        CAshley::EntityArray v;
        engine->add_entity(e);
        e->activate();
        delete e;
        v = engine->get_entities_for(f);
        TS_ASSERT(v.size() == 0);
    }

    void test_engine_get_entities_for() {
        TestEntity *e = new TestEntity;
        CAshley::Family f;
        CAshley::EntityArray v;
        v = engine->get_entities_for(f);
        TS_ASSERT(v.size() == 0);
        engine->add_entity(e);
        v = engine->get_entities_for(f);
        TS_ASSERT(v.size() == 0);
        e->activate();
        v = engine->get_entities_for(f);
        TS_ASSERT(v.size() == 1);
        e->deactivate();
        v = engine->get_entities_for(f);
        TS_ASSERT(v.size() == 0);
        delete e;
        v = engine->get_entities_for(f);
        TS_ASSERT(v.size() == 0);
    }

    void test_engine_add_processor() {
        TS_ASSERT_THROWS_NOTHING(engine->add_processor<TestProcessor1>());
        TS_ASSERT_THROWS(engine->add_processor<TestProcessor1>(), CAshley::ProcessorError);
        TS_ASSERT_THROWS(engine->add_processor<TestProcessor1>(1000), CAshley::ProcessorError);
        TS_ASSERT_THROWS_NOTHING(engine->add_processor<TestProcessor2>(10));
        TS_ASSERT_THROWS(engine->add_processor<TestProcessor2>(), CAshley::ProcessorError);
        TS_ASSERT_THROWS(engine->add_processor<TestProcessor2>(1000), CAshley::ProcessorError);
        engine->remove_processor<TestProcessor2>();
        TS_ASSERT_THROWS_NOTHING(engine->add_processor<TestProcessor2>());
        engine->remove_processor<TestProcessor1>();
    }

    void test_engine_get_processor() {
        TS_ASSERT_THROWS(engine->get_processor<TestProcessor1>(), CAshley::ProcessorError);
        engine->add_processor<TestProcessor1>();
        TS_ASSERT_THROWS_NOTHING(engine->get_processor<TestProcessor1>());
        TS_ASSERT_THROWS(engine->get_processor<TestProcessor2>(), CAshley::ProcessorError);
        engine->add_processor<TestProcessor2>();
        TS_ASSERT_THROWS_NOTHING(engine->get_processor<TestProcessor1>());
        TS_ASSERT_THROWS_NOTHING(engine->get_processor<TestProcessor2>());
        engine->remove_processor<TestProcessor1>();
        TS_ASSERT_THROWS(engine->get_processor<TestProcessor1>(), CAshley::ProcessorError);
        TS_ASSERT_THROWS_NOTHING(engine->get_processor<TestProcessor2>());
        engine->remove_processor<TestProcessor2>();
        TS_ASSERT_THROWS(engine->get_processor<TestProcessor1>(), CAshley::ProcessorError);
        TS_ASSERT_THROWS(engine->get_processor<TestProcessor2>(), CAshley::ProcessorError);
    }

    void test_engine_remove_processor() {
        TS_ASSERT_THROWS(engine->remove_processor<TestProcessor1>(), CAshley::ProcessorError);
        engine->add_processor<TestProcessor1>();
        TS_ASSERT_THROWS_NOTHING(engine->remove_processor<TestProcessor1>());
        TS_ASSERT_THROWS(engine->remove_processor<TestProcessor1>(), CAshley::ProcessorError);
        engine->add_processor<TestProcessor1>(1000);
        TS_ASSERT_THROWS_NOTHING(engine->remove_processor<TestProcessor1>());
        TS_ASSERT_THROWS(engine->remove_processor<TestProcessor1>(), CAshley::ProcessorError);
        engine->add_processor<TestProcessor1>();
        engine->add_processor<TestProcessor2>();
        TS_ASSERT_THROWS_NOTHING(engine->remove_processor<TestProcessor1>());
        TS_ASSERT_THROWS_NOTHING(engine->remove_processor<TestProcessor2>());
    }

    void test_engine_add_entitylistener() {
        TestEntityListener * l = new TestEntityListener;
        TS_ASSERT_THROWS_NOTHING(engine->add_listener(l));
        TS_ASSERT_THROWS(engine->add_listener(l), CAshley::EntityListenerError);
        engine->remove_listener(l);
        TS_ASSERT_THROWS_NOTHING(engine->add_listener(l));
        engine->remove_listener(l);
    }

    void test_engine_remove_entitylistener() {
        TestEntityListener * l = new TestEntityListener;
        TS_ASSERT_THROWS(engine->remove_listener(l), CAshley::EntityListenerError);
        engine->add_listener(l);
        TS_ASSERT_THROWS_NOTHING(engine->remove_listener(l));
        TS_ASSERT_THROWS(engine->remove_listener(l), CAshley::EntityListenerError);
    }

    void test_engine_run_tick_1() {
        unsigned int x = 1;
        engine->add_processor<TestProcessor1>(1);
        engine->add_processor<TestProcessor2>(2);
        engine->get_processor<TestProcessor1>()->set_x(&x);
        engine->get_processor<TestProcessor2>()->set_x(&x);
        engine->get_processor<TestProcessor1>()->activate();
        engine->get_processor<TestProcessor2>()->activate();
        engine->run_tick(1);
        TS_ASSERT(x == 11);
        x = 1;
        engine->remove_processor<TestProcessor1>();
        engine->remove_processor<TestProcessor2>();
        engine->add_processor<TestProcessor1>(2);
        engine->add_processor<TestProcessor2>(1);
        engine->get_processor<TestProcessor1>()->set_x(&x);
        engine->get_processor<TestProcessor2>()->set_x(&x);
        engine->get_processor<TestProcessor1>()->activate();
        engine->get_processor<TestProcessor2>()->activate();
        engine->run_tick(1);
        TS_ASSERT(x == 20);
        x = 1;
        engine->remove_processor<TestProcessor1>();
        engine->remove_processor<TestProcessor2>();
        engine->add_processor<TestProcessor1>();
        engine->add_processor<TestProcessor2>();
        engine->get_processor<TestProcessor1>()->set_x(&x);
        engine->get_processor<TestProcessor2>()->set_x(&x);
        engine->get_processor<TestProcessor1>()->activate();
        engine->get_processor<TestProcessor2>()->activate();
        engine->run_tick(1);
        TS_ASSERT(x == 20 || x == 11);
    }
};

#endif //__CASHLEY_ENGINETESTS_H

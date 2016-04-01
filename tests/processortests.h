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

#ifndef __CASHLEY_PROCESSORTESTS_H
#define __CASHLEY_PROCESSORTESTS_H

#include <cxxtest/TestSuite.h>
#include "../include/cashley.h"
#include "common.h"

class ProcessorTestSuite : public CxxTest::TestSuite {
public:
    class TestProcessor : public CAshley::Processor {
    public:
        unsigned int counter;
        virtual void run_tick(unsigned int delay) {
            UNREFERENCED_PARAMETER(delay);
            counter++;
        }
        CASHLEY_PROCESSOR
    };

    CAshley::Engine * engine;

    void setUp() {
        engine = new CAshley::Engine;
        engine->add_processor<TestProcessor>();
    }

    void tearDown() {
        delete engine;
    }

    void test_processor_is_running(void) {
        TS_ASSERT(engine->get_processor<TestProcessor>()->is_running() == false);
        engine->get_processor<TestProcessor>()->activate();
        TS_ASSERT(engine->get_processor<TestProcessor>()->is_running() == true);
        engine->get_processor<TestProcessor>()->deactivate();
        TS_ASSERT(engine->get_processor<TestProcessor>()->is_running() == false);
    }

    void test_processor_activate(void) {
        TS_ASSERT(engine->get_processor<TestProcessor>()->counter == 0);
        engine->run_tick(1);
        TS_ASSERT(engine->get_processor<TestProcessor>()->counter == 0);
        engine->get_processor<TestProcessor>()->activate();
        TS_ASSERT(engine->get_processor<TestProcessor>()->counter == 0);
        engine->run_tick(1);
        TS_ASSERT(engine->get_processor<TestProcessor>()->counter == 1);
        engine->get_processor<TestProcessor>()->activate();
        TS_ASSERT(engine->get_processor<TestProcessor>()->counter == 1);
        engine->run_tick(1);
        TS_ASSERT(engine->get_processor<TestProcessor>()->counter == 2);
    }

    void test_processor_deactivate(void) {
        TS_ASSERT(engine->get_processor<TestProcessor>()->counter == 0);
        engine->get_processor<TestProcessor>()->deactivate();
        TS_ASSERT(engine->get_processor<TestProcessor>()->counter == 0);
        engine->run_tick(1);
        TS_ASSERT(engine->get_processor<TestProcessor>()->counter == 0);
        engine->get_processor<TestProcessor>()->activate();
        engine->run_tick(1);
        TS_ASSERT(engine->get_processor<TestProcessor>()->counter == 1);
        engine->get_processor<TestProcessor>()->deactivate();
        TS_ASSERT(engine->get_processor<TestProcessor>()->counter == 1);
        engine->run_tick(1);
        TS_ASSERT(engine->get_processor<TestProcessor>()->counter == 1);
        engine->get_processor<TestProcessor>()->deactivate();
        TS_ASSERT(engine->get_processor<TestProcessor>()->counter == 1);
        engine->run_tick(1);
        TS_ASSERT(engine->get_processor<TestProcessor>()->counter == 1);
    }
};

#endif //__CASHLEY_PROCESSORTESTS_H

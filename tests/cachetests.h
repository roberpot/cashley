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

#ifndef __CASHLEY_CACHETESTS_H
#define __CASHLEY_CACHETESTS_H

#include <cxxtest/TestSuite.h>
#include "../include/cashley.h"

class CacheTestSuite : public CxxTest::TestSuite
{
public:
    void test_cache_constructor(void) {
        CAshley::Cache<unsigned int> cache(3);
        TS_ASSERT(cache._size == 3);
        TS_ASSERT(cache._active == 0);
        TS_ASSERT(cache._allocated == 0);
    }

    void test_cache_block_alloc(void) {
        CAshley::Cache<unsigned int> cache(3);
        TS_ASSERT_THROWS_NOTHING(cache.block_alloc());
        TS_ASSERT(cache._size == 3);
        TS_ASSERT(cache._active == 0);
        TS_ASSERT(cache._allocated == 1);
        TS_ASSERT_THROWS_NOTHING(cache.block_alloc());
        TS_ASSERT(cache._size == 3);
        TS_ASSERT(cache._active == 0);
        TS_ASSERT(cache._allocated == 2);
        TS_ASSERT_THROWS_NOTHING(cache.block_alloc());
        TS_ASSERT(cache._size == 3);
        TS_ASSERT(cache._active == 0);
        TS_ASSERT(cache._allocated == 3);
        TS_ASSERT_THROWS(cache.block_alloc(), CAshley::CacheError);
        TS_ASSERT(cache._size == 3);
        TS_ASSERT(cache._active == 0);
        TS_ASSERT(cache._allocated == 3);
    }

    void test_cache_block_free(void) {
        CAshley::Cache<unsigned int> cache(3);
        TS_ASSERT_THROWS(cache.block_free(1), CAshley::CacheError);
        TS_ASSERT(cache._size == 3);
        TS_ASSERT(cache._active == 0);
        TS_ASSERT(cache._allocated == 0);
        unsigned int b1;
        b1 = cache.block_alloc();
        TS_ASSERT_THROWS_NOTHING(cache.block_free(b1));
        TS_ASSERT(cache._size == 3);
        TS_ASSERT(cache._active == 0);
        TS_ASSERT(cache._allocated == 0);
        TS_ASSERT_THROWS(cache.block_free(b1), CAshley::CacheError);
        TS_ASSERT(cache._size == 3);
        TS_ASSERT(cache._active == 0);
        TS_ASSERT(cache._allocated == 0);
    }

    void test_cache_block_activate(void) {
        CAshley::Cache<unsigned int> cache(3);
        TS_ASSERT_THROWS(cache.block_activate(1), CAshley::CacheError);
        TS_ASSERT(cache._size == 3);
        TS_ASSERT(cache._active == 0);
        TS_ASSERT(cache._allocated == 0);
        unsigned int b1;
        b1 = cache.block_alloc();
        TS_ASSERT_THROWS_NOTHING(cache.block_activate(b1));
        TS_ASSERT(cache._size == 3);
        TS_ASSERT(cache._active == 1);
        TS_ASSERT(cache._allocated == 1);
        TS_ASSERT_THROWS_NOTHING(cache.block_activate(b1));
        TS_ASSERT(cache._size == 3);
        TS_ASSERT(cache._active == 1);
        TS_ASSERT(cache._allocated == 1);
        TS_ASSERT_THROWS_NOTHING(cache.block_free(b1));
        TS_ASSERT_THROWS(cache.block_activate(b1), CAshley::CacheError);
        TS_ASSERT(cache._size == 3);
        TS_ASSERT(cache._active == 0);
        TS_ASSERT(cache._allocated == 0);
    }

    void test_cache_block_deactivate(void) {
        CAshley::Cache<unsigned int> cache(3);
        TS_ASSERT_THROWS(cache.block_deactivate(1), CAshley::CacheError);
        TS_ASSERT(cache._size == 3);
        TS_ASSERT(cache._active == 0);
        TS_ASSERT(cache._allocated == 0);
        unsigned int b1;
        b1 = cache.block_alloc();
        TS_ASSERT_THROWS_NOTHING(cache.block_deactivate(b1));
        TS_ASSERT(cache._size == 3);
        TS_ASSERT(cache._active == 0);
        TS_ASSERT(cache._allocated == 1);
        TS_ASSERT_THROWS_NOTHING(cache.block_activate(b1));
        TS_ASSERT_THROWS_NOTHING(cache.block_deactivate(b1));
        TS_ASSERT(cache._size == 3);
        TS_ASSERT(cache._active == 0);
        TS_ASSERT(cache._allocated == 1);
        TS_ASSERT_THROWS_NOTHING(cache.block_deactivate(b1));
        TS_ASSERT(cache._size == 3);
        TS_ASSERT(cache._active == 0);
        TS_ASSERT(cache._allocated == 1);
        TS_ASSERT_THROWS_NOTHING(cache.block_free(b1));
        TS_ASSERT_THROWS(cache.block_deactivate(b1), CAshley::CacheError);
        TS_ASSERT(cache._size == 3);
        TS_ASSERT(cache._active == 0);
        TS_ASSERT(cache._allocated == 0);
    }

    void test_cache_get_block(void) {
        CAshley::Cache<unsigned int> cache(3);
        unsigned int b1;
        TS_ASSERT_THROWS(cache.get_block(1), CAshley::CacheError);
        TS_ASSERT(cache._size == 3);
        TS_ASSERT(cache._active == 0);
        TS_ASSERT(cache._allocated == 0);
        b1 = cache.block_alloc();
        TS_ASSERT_THROWS_NOTHING(cache.get_block(b1));
        TS_ASSERT_THROWS_NOTHING(cache.block_activate(b1));
        TS_ASSERT_THROWS_NOTHING(cache.get_block(b1));
        TS_ASSERT(cache._size == 3);
        TS_ASSERT(cache._active == 1);
        TS_ASSERT(cache._allocated == 1);
        TS_ASSERT_THROWS_NOTHING(cache.block_deactivate(b1));
        TS_ASSERT_THROWS_NOTHING(cache.get_block(b1));
        TS_ASSERT(cache._size == 3);
        TS_ASSERT(cache._active == 0);
        TS_ASSERT(cache._allocated == 1);
        *cache.get_block(b1) = 53;
        TS_ASSERT(*cache.get_block(b1) == 53);
        TS_ASSERT_THROWS_NOTHING(cache.block_free(b1));
        TS_ASSERT_THROWS(cache.get_block(b1), CAshley::CacheError);
        TS_ASSERT(cache._size == 3);
        TS_ASSERT(cache._active == 0);
        TS_ASSERT(cache._allocated == 0);
    }

    void test_cache_get_active_blocks(void) {
        CAshley::Cache<unsigned int> cache(3);
        TS_ASSERT(cache.get_active_blocks().second == 0);
        unsigned int b1 = cache.block_alloc(), b2 = cache.block_alloc();
        TS_ASSERT(cache.get_active_blocks().second == 0);
        *cache.get_block(b1) = 57;
        *cache.get_block(b2) = 63;
        cache.block_activate(b2);
        TS_ASSERT(cache._cache[0] == 63);
        TS_ASSERT(cache.get_active_blocks().second == 1);
        unsigned int * x = static_cast<unsigned int *>(cache.get_active_blocks().first);
        TS_ASSERT(*x == 63);
        cache.block_deactivate(b2);
        TS_ASSERT(cache.get_active_blocks().second == 0);
    }
};


#endif //__CASHLEY_CACHETESTS_H

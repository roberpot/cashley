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

#ifndef __CASHLEY_PROCESSOR_H
#define __CASHLEY_PROCESSOR_H

#include "common.h"

#define CASHLEY_PROCESSOR \
__CASHLEY_COMMON_METHOD \
friend class CAshley::Engine;

namespace CAshley {

    class Engine;

    /**
     * \brief Class to inherit for processors.
     */
    class Processor {
    public:
        /**
         * \brief Default constructor.
         * By default, processors are unactivated.
         */
        Processor();
        /**
         * \brief Default destructor.
         */
        virtual ~Processor();
        /**
         * \brief Activate the processor.
         * Make its available to process an engine tick.
         */
        void activate();
        /**
         * \brief Deactivate the processor.
         * Make its unavailable to process an engine tick.
         */
        void deactivate();
        /**
         * \brief Check if active status.
         * \return true if active, false otherwise.
         */
        bool is_running();
        /**
         * \brief Run a tick.
         * Run an engine tick.
         * \param delay Delay passed to engine.
         */
        virtual void run_tick(unsigned int delay) = 0;

        /**
         * \brief Get the class string.
         */
        virtual std::string get_name();

        friend class Engine;
    protected:
        /**
         * \brief Engine linked with the Processor.
         */
        Engine * _engine;
    private:
        /**
         * \brief Active status of the Processor.
         */
        bool _running;
    };

}

#endif //__CASHLEY_PROCESSOR_H

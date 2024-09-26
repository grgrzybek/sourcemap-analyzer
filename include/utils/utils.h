/*
 * Copyright 2024 Grzegorz Grzybek
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef SMA_UTILS_H
#define SMA_UTILS_H

#include <getopt.h>
#include <fstream>
#include <iostream>

#include "utils/utils.h"

namespace sma {
    class Options {
        int argc;
        char **argv;

        bool _valid = true;
        bool _finish = false;
        int exit_code = EXIT_SUCCESS;

        void parse();

        void print_help() const;

    public:
        const char *program_name = nullptr;

        Options(int _argc, char *_argv[]);

        ~Options();

        [[nodiscard]] bool check() const;

        [[nodiscard]] int result() const;
    };
}

#endif //SMA_UTILS_H

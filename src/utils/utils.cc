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

#include <cstring>
#include <iostream>

#include "config.h"
#include "utils/utils.h"

namespace sma {
    static constexpr option longopts[] = {
        { "help", no_argument, nullptr, 'h' },
        { "version", no_argument, nullptr, 'v' }
    };

    /**
     * Constructs generic option parser
     * @param _argc
     * @param _argv
     */
    Options::Options(const int _argc, char *_argv[])
        : argc(_argc), argv(_argv) {
        parse();
    }

    Options::~Options() = default;

    /**
     * Parses command line arguments - mostly to find a file with puzzle data to read
     */
    void Options::parse() {
        program_name = argv[0];
        int optc, lose = 0;

        while ((optc = getopt_long(argc, argv, "hv", longopts, nullptr)) != -1) {
            switch (optc) {
                case 'h':
                    print_help();
                    _finish = true;
                    return;
                case 'v':
                    std::cout << PACKAGE_STRING << std::endl;
                    _finish = true;
                    return;
                default:
                    lose = 1;
            }
        }

        if (lose || optind < argc) {
            if (optind < argc) {
                std::cerr << program_name << ": extra operand: " << argv[optind] << std::endl;
            }
            std::cerr << "Try `" << program_name << " --help' for more information." << std::endl;
            _valid = false;
            exit_code = EXIT_FAILURE;
        }
    }

    /**
     * Checks whether to continue or to exit immediately.
     * @return
     */
    bool Options::check() const {
        return _valid && !_finish;
    }

    /**
     * @return
     */
    int Options::result() const {
        return exit_code;
    }

    /**
     * Instructs about program usage
     */
    void Options::print_help() const {
        std::cout << "Version: " << SMA_VERSION << "\n";
        std::cout << "Usage: " << program_name << " [OPTION]...\n";

        std::cout << "-h, --help      display this help and exit\n";
        std::cout << "-v, --version   display version information and exit\n\n";

        std::cout << "Report bugs to <" << PACKAGE_BUGREPORT << ">.\n";
        std::cout.flush();
    }
}

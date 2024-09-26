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

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "utils/utils.h"

using namespace std;

const char *const symbols = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

struct line {
    vector<string> *fields = nullptr;
    vector<vector<int> *> *decoded = nullptr;

    line() {
        fields = new vector<string>;
        decoded = new vector<vector<int> *>;
    }
    ~line() {
        delete fields;
        for (auto _v: *decoded) {
            delete _v;
        }
        delete decoded;
    }

    void decode() const;
};

void line::decode() const {
    for (auto _f : *fields) {
        auto dv = new vector<int>;
        decoded->emplace_back(dv);
        for (size_t i = 0; i < _f.length(); i++) {
            int shift = 0;
            int value = 0;
            int pos = 0;
            while (symbols[pos++] != _f[i]);
            int v = (pos-1) & 0x3F;
            value |= (v & ~0x20) << shift;
            while (v & 0x20) {
                shift+=5;
                i++;
                pos = 0;
                while (symbols[pos++] != _f[i]);
                v = (pos - 1) & 0x3F;
                value |= (v & ~0x20) << shift;
            }
            if (value & 0x01) {
                value = -(value >> 1);
            } else {
                value = value >> 1;
            }
            dv->push_back(value);
        }
    }
}

int main(const int argc, char *argv[]) {
    const sma::Options options(argc, argv);
    if (!options.check())
        return options.result();

    cout << "Fields:\n";
    cout << " 1. column in generated file\n";
    cout << " 2. index in \"sources\"\n";
    cout << " 3. line in original file\n";
    cout << " 4. column in original file\n";
    cout << " 5. index in \"names\"\n";

    // https://docs.google.com/document/d/1U1RGAehQwRypUTovF1KRlpiOFze0b-_2gc6fAH0KY0k
    // https://web.dev/articles/source-maps
    // https://ryanseddon.com/javascript/source-mapping/
    // https://lucid.co/techblog/2019/08/22/decode-encoding-base64-vlqs-source-maps
    // https://sokra.github.io/source-map-visualization/

    // lines are separated by ';' and contains segments
    // segments are separated by ',' and contains fields
    // fields are VLQ base 64 encoded strings - symbol by symbol

    // fields are:
    // 1) The zero-based starting column of the line in the generated code
    // 2) If present, a zero-based (relative if not first) index into the “sources” list
    // 3) If present, a zero-based (relative if not first) starting line in the original source represented
    // 4) If present, a zero-based (relative if not first) starting column of the line in the source represented
    // 5) If present, a zero-based (relative if not first) index into the “names” list associated with this segment

    char c;
    auto lines = new vector<line *>;
    auto current_line = new line;
    string field;
    line *l;
    while (!cin.eof()) {
        cin.get(c);
        if (cin.eof()) {
            break;
        } else if (c == ';') {
            // new line
            current_line->fields->emplace_back(field);
            l = current_line;
            current_line = new line;
            lines->push_back(l);
            field.clear();
        } else if (c == ',') {
            // new field within a line
            current_line->fields->emplace_back(field);
            field.clear();
        } else {
            // VLQ data
            field += c;
        }
    }
    current_line->fields->emplace_back(field);
    lines->push_back(current_line);

    cout << "lines: " << lines->size() << endl;
    for (auto _l: *lines) {
        cout << "line: ";
        bool first = true;
        for (const auto &_f: *_l->fields) {
            if (!first) {
                cout << ", ";
            }
            first = false;
            cout << "[" << _f << "]";
        }
        cout << endl;
        _l->decode();
    }
    cout << "decoded:\n";
    int lc = 1;
    // to calculate relative values for each field (0th is reset on each line, 1-4 are relative between lines)
    int rel[5] = { -1, -1, -1, -1, -1 };
    for (auto _l: *lines) {
        rel[0] = -1;
        cout << "line " << lc++ << ":\n";
        bool first = true;
        for (const auto &_d: *_l->decoded) {
            if (!first) {
                cout << ", ";
            }
            first = false;
            cout << "[";
            bool first2 = true;
            int fc = 0;
            for (const auto &_v : *_d) {
                if (!first2) {
                    cout << " ";
                }
                first2 = false;
                if (rel[fc] == -1) {
                    rel[fc] = _v;
                } else {
                    rel[fc] += _v;
                }
                cout << rel[fc++];
            }
            cout << "]";
        }
        cout << endl;
    }

    for (auto _l: *lines) {
        delete _l;
    }
    delete lines;

    return EXIT_SUCCESS;
}

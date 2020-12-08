/*
 Copyright 2016-2020 Intel Corporation
 
 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 
     http://www.apache.org/licenses/LICENSE-2.0
 
 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
*/
#pragma once

#include <string>

namespace ccl {

namespace preview {

// struct float16 {
//     constexpr float16() : data(0) {}
//     constexpr float16(uint16_t v) : data(v) {}
//     uint16_t data;

//     friend std::ostream& operator<<(std::ostream& out, const float16& v) {
//         out << v.data;
//         return out;
//     }

//     friend bool operator==(const float16& v1, const float16& v2) {
//         return (v1.data == v2.data) ? true : false;
//     }

//     friend bool operator!=(const float16& v1, const float16& v2) {
//         return !(v1 == v2);
//     }

// } __attribute__((packed));

} // namespace preview

namespace v1 {

struct bfloat16 {
    constexpr bfloat16() : data(0) {}
    constexpr bfloat16(uint16_t v) : data(v) {}
    uint16_t data;

    friend std::ostream& operator<<(std::ostream& out, const bfloat16& v) {
        out << v.data;
        return out;
    }

    friend bool operator==(const bfloat16& v1, const bfloat16& v2) {
        return (v1.data == v2.data) ? true : false;
    }

    friend bool operator!=(const bfloat16& v1, const bfloat16& v2) {
        return !(v1 == v2);
    }

} __attribute__((packed));

} // namespace v1

using v1::bfloat16;

} // namespace ccl

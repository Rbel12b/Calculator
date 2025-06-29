/*
 * -----------------------------------------------------------------------------
 *  File:           Expression.h
 *  Project:        Calculator
 *  Author:         Rbel12b (https::/github.com/rbel12b)
 *  Description:    A class to represent a number
 * -----------------------------------------------------------------------------
 *  License:        MIT License
 *
 *  Copyright (c) 2025 Rbel12b
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 *  THE SOFTWARE.
 * -----------------------------------------------------------------------------
 */
#pragma once
#ifndef _NUMBER_H_
#define _NUMBER_H_

#pragma once

#include <boost/multiprecision/cpp_int.hpp>
#include <cmath>
#include <optional>
#include <iostream>

class NumberClass;

using Number = NumberClass;
// using Number = double;

class NumberClass {
public:
    using BigRational = boost::multiprecision::cpp_rational;

    enum class Tag {
        None,
        Pi,
        E,
        Sqrt2,
        Custom  // For future extension
    };

    BigRational rationalPart;     // e.g., 42, 1/3
    BigRational irrationalPart;   // e.g., 2 → 2π
    Tag tag = Tag::None;

    // Constructors
    NumberClass() = default;
    NumberClass(int val) : rationalPart(val) {}
    NumberClass(const BigRational& val) : rationalPart(val) {}
    NumberClass(double val) : rationalPart(BigRational(val)) {}
    NumberClass(BigRational rational, BigRational irrational, Tag t)
        : rationalPart(std::move(rational)), irrationalPart(std::move(irrational)), tag(t) {}
    NumberClass(std::string str) {
        try {
            rationalPart = BigRational(str);
            tag = Tag::None;
            irrationalPart = 0; // No irrational part
        } catch (const std::exception& e) {
            // If parsing fails, we assume it's a string with a decimal
            if (str.find('.') != std::string::npos) {
                rationalPart = BigRational(std::stod(str));
                tag = Tag::None;
                irrationalPart = 0; // No irrational part
            }
        }
        // Attempt to parse the string for irrational parts
        if (str.find("pi") != std::string::npos || str.find("π") != std::string::npos) {
            tag = Tag::Pi;
            irrationalPart = 1; // Default coefficient for π
        } else if (str.find("e") != std::string::npos) {
            tag = Tag::E;
            irrationalPart = 1; // Default coefficient for e
        } else if (str.find("sqrt(2)") != std::string::npos || str.find("√2") != std::string::npos) {
            tag = Tag::Sqrt2;
            irrationalPart = 1; // Default coefficient for √2
        }
    }

    static NumberClass pi(BigRational coeff = 1) { return NumberClass(0, coeff, Tag::Pi); }
    static NumberClass e(BigRational coeff = 1)  { return NumberClass(0, coeff, Tag::E); }
    static NumberClass sqrt2(BigRational coeff = 1) { return NumberClass(0, coeff, Tag::Sqrt2); }

    // Arithmetic
    NumberClass operator+(const NumberClass& other) const {
        if (tag == other.tag) {
            return NumberClass(rationalPart + other.rationalPart,
                          irrationalPart + other.irrationalPart,
                          tag);
        } else if (isPureRational() && other.isPureRational()) {
            return NumberClass(rationalPart + other.rationalPart);
        }
        return approximate() + other.approximate();
    }

    NumberClass operator-(const NumberClass& other) const {
        if (tag == other.tag) {
            return NumberClass(rationalPart - other.rationalPart,
                          irrationalPart - other.irrationalPart,
                          tag);
        } else if (isPureRational() && other.isPureRational()) {
            return NumberClass(rationalPart - other.rationalPart);
        }
        return approximate() - other.approximate();
    }

    // For now, multiplication is numeric fallback (can be extended)
    NumberClass operator*(const NumberClass& other) const {
        return NumberClass(approximate() * other.approximate());
    }

    NumberClass operator/(const NumberClass& other) const {
        return NumberClass(approximate() / other.approximate());
    }

    // Convert to double for evaluation
    double approximate() const {
        double rat = rationalPart.convert_to<double>();
        double irr = 0;
        switch (tag) {
            case Tag::Pi:     irr = M_PI; break;
            case Tag::E:      irr = M_E; break;
            case Tag::Sqrt2:  irr = std::sqrt(2); break;
            default:          irr = 0;
        }
        return rat + (irr * irrationalPart.convert_to<double>());
    }

    bool isPureRational() const {
        return tag == Tag::None || irrationalPart == 0;
    }

    // Stream output
    friend std::ostream& operator<<(std::ostream& os, const NumberClass& n) {
        os << n.rationalPart.convert_to<double>();
        if (n.tag != Tag::None && n.irrationalPart != 0) {
            os << " + " << n.irrationalPart << "*";
            switch (n.tag) {
                case Tag::Pi: os << "π"; break;
                case Tag::E: os << "e"; break;
                case Tag::Sqrt2: os << "√2"; break;
                default: os << "?";
            }
        }
        return os;
    }

    // Unary negation
    NumberClass operator-() const {
        return NumberClass(-rationalPart, -irrationalPart, tag);
    }

    // Equality and inequality
    bool operator==(const NumberClass& other) const {
        if (tag != other.tag) return false;
        return rationalPart == other.rationalPart && irrationalPart == other.irrationalPart;
    }

    bool operator!=(const NumberClass& other) const {
        return !(*this == other);
    }

    // Relational operators (approximate comparison)
    bool operator<(const NumberClass& other) const {
        return this->approximate() < other.approximate();
    }

    bool operator<=(const NumberClass& other) const {
        return this->approximate() <= other.approximate();
    }

    bool operator>(const NumberClass& other) const {
        return this->approximate() > other.approximate();
    }

    bool operator>=(const NumberClass& other) const {
        return this->approximate() >= other.approximate();
    }

    // Implicit conversion to bool for logical expressions
    explicit operator bool() const {
        return rationalPart != 0 || irrationalPart != 0;
    }
};

#endif
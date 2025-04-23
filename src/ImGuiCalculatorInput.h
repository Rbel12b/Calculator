/*
 * -----------------------------------------------------------------------------
 *  File:           ImGuiCalculatorImput.h
 *  Project:        Calculator
 *  Author:         Rbel12b (https::/github.com/rbel12b)
 *  Description:    a numpad input imlementation for imgui
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
#ifndef _IMGUI_CALCULATOR_INPUT_H_
#define _IMGUI_CALCULATOR_INPUT_H_

#include <string>
#include <imgui.h>
#include <unordered_map>
#include <vector>

struct CalcInputData
{
    std::string text;
    std::string lastExpr;
    bool enterPressed = false;
    bool error = false;
    bool processed = false;
};

namespace ImGuiCalculatorInput
{
    void init();
    void render(const char *name, ImGuiID id, bool useImGuiBegin = true,
                       ImGuiWindowFlags flags = 0, ImGuiChildFlags childFlags = 0);
    CalcInputData& getInput(ImGuiID id);

    void _render(ImGuiID id);
    void addCharachter(CalcInputData &data, ImWchar c);

    extern std::unordered_map<ImGuiID, CalcInputData> inputData;

    struct keyData
    {
        bool exists = true;
        std::string text;
        char encoded;
        keyData() : exists(false) {}
        keyData(std::string _text, char _c)
            : exists(true), text(_text), encoded(_c) {}
    };

    typedef std::vector<keyData> InputRow;

    extern std::vector<InputRow> inputRows;
    extern bool fontsReady;
    extern ImFont* font13;
    extern ImFont* font18;
    extern ImFont* font26;
    extern ImFont* font30;
    extern ImFont* font32;
    extern ImFont* font40;
    extern ImFont* font48;
    extern ImFont* font60;

    extern const unsigned int numFonts;
    extern ImFont** fontMap[8];
};

#endif
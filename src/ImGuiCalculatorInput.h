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
    bool enterPressed = false;
};

class ImGuiCalculatorInput
{
public:
    static void init();
    static void render(const char *name, ImGuiID id, bool useImGuiBegin = true,
                       ImGuiWindowFlags flags = 0, ImGuiChildFlags childFlags = 0);
    static CalcInputData getInput(ImGuiID id);

private:
    static void _render(ImGuiID id);

private:
    static std::unordered_map<ImGuiID, CalcInputData> inputData;

    struct keyData
    {
        bool exists = true;
        std::string text;
        char encoded;
        keyData()
            : exists(false)
        {
        }
        keyData(std::string _text, char _c)
            : exists(true), text(_text), encoded(_c)
        {
        }
    };

    typedef std::vector<keyData> InputRow;
    static std::vector<InputRow> inputRows;
    static bool fontsReady;
    static ImFont* font13; // Default size (13.0f)
    static ImFont* font18;
    static ImFont* font26;
    static ImFont* font32;
    static ImFont* font30;
    static ImFont* font40;
    static ImFont* font48;
    static ImFont* font60;
};

#endif
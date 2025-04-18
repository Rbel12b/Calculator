/*
 * -----------------------------------------------------------------------------
 *  File:           ImGuiCalculatorImput.cpp
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
#include "ImGuiCalculatorInput.h"
#include "dejavusans_ttf.h" 

std::unordered_map<ImGuiID, CalcInputData> ImGuiCalculatorInput::inputData;

std::vector<ImGuiCalculatorInput::InputRow> ImGuiCalculatorInput::inputRows = {
    {keyData("(", '('), keyData(")", ')'), keyData(u8"\u232B", '\b')},
    {keyData("7", '7'), keyData("8", '8'), keyData("9", '9')},
    {keyData("4", '4'), keyData("5", '5'), keyData("6", '6')},
    {keyData("1", '1'), keyData("2", '2'), keyData("3", '3')},
    {keyData("0", '0'), keyData(".", '.'), keyData("=", '=')}};

bool ImGuiCalculatorInput::fontsReady = false;
ImFont* ImGuiCalculatorInput::font13 = nullptr; // Default size (13.0f)
ImFont* ImGuiCalculatorInput::font18 = nullptr;
ImFont* ImGuiCalculatorInput::font26 = nullptr;
ImFont* ImGuiCalculatorInput::font32 = nullptr;
ImFont* ImGuiCalculatorInput::font40 = nullptr;
ImFont* ImGuiCalculatorInput::font48 = nullptr;
ImFont* ImGuiCalculatorInput::font60 = nullptr;

void ImGuiCalculatorInput::init()
{
    if (!fontsReady)
    {
        ImGuiIO& io = ImGui::GetIO();

        static const ImWchar ranges[] = {
            0x0020, 0x00FF, // Basic Latin + Latin Supplement
            0x2200, 0x22FF, // Mathematical Operators
            0x2300, 0x23FF, // Misc Technical (⌫ is here at U+232B)
            0,             // End of ranges
        };
        ImFontConfig config;
        config.FontDataOwnedByAtlas = false; // Don't let ImGui free the memory

        config.SizePixels = 13.0f;
        font13 = io.Fonts->AddFontFromMemoryTTF((void*)DejaVuSans_ttf, DejaVuSans_ttf_len, 13.0f, &config, ranges);
        config.SizePixels = 18.0f;
        font18 = io.Fonts->AddFontFromMemoryTTF((void*)DejaVuSans_ttf, DejaVuSans_ttf_len, 18.0f, &config, ranges);
        config.SizePixels = 26.0f;
        font26 = io.Fonts->AddFontFromMemoryTTF((void*)DejaVuSans_ttf, DejaVuSans_ttf_len, 26.0f, &config, ranges);
        config.SizePixels = 32.0f;
        font32 = io.Fonts->AddFontFromMemoryTTF((void*)DejaVuSans_ttf, DejaVuSans_ttf_len, 32.0f, &config, ranges);
        config.SizePixels = 40.0f;
        font40 = io.Fonts->AddFontFromMemoryTTF((void*)DejaVuSans_ttf, DejaVuSans_ttf_len, 40.0f, &config, ranges);
        config.SizePixels = 48.0f;
        font48 = io.Fonts->AddFontFromMemoryTTF((void*)DejaVuSans_ttf, DejaVuSans_ttf_len, 48.0f, &config, ranges);
        config.SizePixels = 60.0f;
        font60 = io.Fonts->AddFontFromMemoryTTF((void*)DejaVuSans_ttf, DejaVuSans_ttf_len, 60.0f, &config, ranges);

        io.Fonts->Build();
        fontsReady = true;
    }
}

void ImGuiCalculatorInput::render(const char *name, ImGuiID id, bool useImGuiBegin,
                                  ImGuiWindowFlags flags, ImGuiChildFlags childFlags)
{
    if (!fontsReady)
    {
        return;
    }
    if (inputData.find(id) == inputData.end())
    {
        inputData[id] = CalcInputData();
    }
    bool display = true;
    if (useImGuiBegin)
    {
        display = ImGui::Begin(name, nullptr, flags);
    }
    ImGui::PushID(ImGui::GetID("calcChild"));
    if (display && ImGui::BeginChild(ImGui::GetID(name), ImVec2(0, 0), childFlags, flags))
    {
        _render(id);
    }
    if (display)
    {
        ImGui::EndChild();
    }
    ImGui::PopID();
    if (useImGuiBegin)
    {
        ImGui::End();
    }
}

CalcInputData ImGuiCalculatorInput::getInput(ImGuiID id)
{
    return inputData[id];
}

void ImGuiCalculatorInput::_render(ImGuiID id)
{
    auto &data = inputData[id];

    ImGui::Dummy(ImVec2(0, ImGui::GetContentRegionAvail().y / 3));

    float height = ImGui::GetContentRegionAvail().y / (inputRows.size() + 1);
    ImFont* selectedFont = font13;
    if (height > 74) selectedFont = font60;
    else if (height > 62) selectedFont = font48;
    else if (height > 54) selectedFont = font40;
    else if (height > 46) selectedFont = font32;
    else if (height > 40) selectedFont = font26;
    else if (height > 32) selectedFont = font18;
    ImGui::PushFont(selectedFont); // Get default font
    ImGui::Text(data.text.c_str());
    ImGui::NewLine();

    // Total available width and height
    float totalWidth = ImGui::GetContentRegionAvail().x;
    float totalHeight = ImGui::GetContentRegionAvail().y;

    // Calculate button width to fill the whole window evenly
    int maxButtonsInRow = 0;
    for (auto &row : inputRows)
    {
        maxButtonsInRow = std::max(maxButtonsInRow, (int)row.size());
    }

    // Calculate the available space for buttons (accounting for vertical spacing between rows)
    float spacing = ImGui::GetStyle().ItemSpacing.y; // Vertical spacing
    float availableHeightForButtons = totalHeight - (spacing * (inputRows.size() - 1));
    float buttonHeight = availableHeightForButtons / inputRows.size(); // Distribute height evenly across rows

    // Calculate button width to fill the whole window evenly
    float buttonWidth = (totalWidth - spacing * (maxButtonsInRow - 1)) / maxButtonsInRow;

    for (auto &row : inputRows)
    {
        for (int i = 0; i < row.size(); ++i)
        {
            const auto &key = row[i];

            if (key.exists)
            {
                if (ImGui::Button(key.text.c_str(), ImVec2(buttonWidth, buttonHeight)))
                {
                    if (key.encoded == '\b')
                    {
                        if (!data.text.empty())
                        {
                            data.text.pop_back();
                        }
                    }
                    else
                    {
                        data.text += key.encoded;
                    }
                }
            }
            else
            {
                ImGui::Dummy(ImVec2(buttonWidth, buttonHeight));
            }

            if (i < row.size() - 1)
                ImGui::SameLine();
        }
    }

    ImGui::PopFont();
}

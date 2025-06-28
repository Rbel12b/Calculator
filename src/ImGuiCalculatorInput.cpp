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

namespace ImGuiCalculatorInput
{
    std::unordered_map<ImGuiID, CalcInputData> inputData;

    std::vector<InputRow> inputRows = {
        {keyData("(", '('), keyData(")", ')'), keyData(u8"\u232B", '\b')},
        {keyData("7", '7'), keyData("8", '8'), keyData("9", '9'), keyData("÷", '/')},
        {keyData("4", '4'), keyData("5", '5'), keyData("6", '6'), keyData("×", '*')},
        {keyData("1", '1'), keyData("2", '2'), keyData("3", '3'), keyData("+", '+')},
        {keyData("0", '0'), keyData(".", '.'), keyData("=", '='), keyData("-", '-')},
    };

    bool fontsReady = false;
    ImFont* defaultFont = nullptr;

    void init()
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
            defaultFont = io.Fonts->AddFontFromMemoryTTF((void*)DejaVuSans_ttf, DejaVuSans_ttf_len, 13.0f, &config, ranges);

            io.Fonts->Build();
            fontsReady = true;
        }
    }

    void render(const char *name, ImGuiID id, bool useImGuiBegin,
        ImGuiWindowFlags flags, ImGuiChildFlags childFlags)
    {
        if (!fontsReady)
        {
            return;
        }
        auto &io = ImGui::GetIO();
        if (inputData.find(id) == inputData.end())
        {
            inputData[id] = CalcInputData();
        }
        auto &data = inputData[id];
        bool display = true;
        if (useImGuiBegin)
        {
            display = ImGui::Begin(name, nullptr, flags);
        }
        ImGui::PushID(ImGui::GetID("calcChild"));
        if (display && ImGui::BeginChild(ImGui::GetID(name), ImVec2(0, 0), childFlags, flags))
        {
            if (!io.WantCaptureKeyboard && ImGui::IsWindowFocused())
            {
                for (int i = 0; i < io.InputQueueCharacters.Size; ++i)
                {
                    ImWchar c = io.InputQueueCharacters[i];

                    addCharachter(data, c);
                }

                if (ImGui::IsKeyPressed(ImGuiKey_Backspace))
                {
                    if (data.error)
                    {
                        data.text.clear();
                        data.error = false;
                    }
                    if (data.processed)
                    {
                        data.text.clear();
                        data.processed = false;
                    }
                    if (!inputData[id].text.empty())
                    {
                        inputData[id].text.pop_back();
                    }
                }

                if (ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::IsKeyPressed(ImGuiKey_KeypadEnter))
                {
                    if (data.error)
                    {
                        data.text.clear();
                        data.error = false;
                    }
                    data.enterPressed = true;
                    data.lastExpr = data.text;
                }

                io.InputQueueCharacters.resize(0); // clear the queue after processing
            }
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

    CalcInputData& getInput(ImGuiID id)
    {
        return inputData[id];
    }

    void _render(ImGuiID id)
    {
        auto &data = inputData[id];
        const float minButtonAreaHeight = 300.0f; // Minimum height for the button area
        float buttonAreaHeight = std::max(minButtonAreaHeight, ImGui::GetContentRegionAvail().y * 3 / 4 - 20); // Ensure the button area is at least minButtonAreaHeight
        float textHeight = buttonAreaHeight / inputRows.size(); // Calculate text height based on the number of rows

        float blankHeight = std::max(0.0f, ImGui::GetContentRegionAvail().y - (buttonAreaHeight + textHeight));

        ImGui::Dummy(ImVec2(0, blankHeight)); // Add dummy space to fill the available height

        // Render the last expression with a 11px font size
        // Calculate indentation based on the window size and the text size
        int indent = ImGui::GetWindowSize().x - (defaultFont->CalcTextSizeA(
            13.0f, FLT_MAX, 0.0f,
            data.lastExpr.c_str()).x + 20);
        
        ImGui::Dummy(ImVec2(indent, 0)); // Add dummy space for indentation
        ImGui::SameLine();
        ImGui::PushFont(defaultFont, 11.0f); 
        ImGui::Text(data.lastExpr.c_str()); // Render the last expression with a 13px font size
        ImGui::PopFont();
        ImGui::NewLine();

        // Calculate font size based on available height (make text fit nicely above the buttons)
        float fontSize = textHeight * 0.7f;
        ImVec2 textSize = defaultFont->CalcTextSizeA(fontSize, FLT_MAX, 0.0f, data.text.c_str());

        if (textSize.x > ImGui::GetContentRegionAvail().x - 20)
        {
            // If the text is too wide, reduce the font size
            fontSize = std::max(11.0f, (ImGui::GetContentRegionAvail().x - 40) / textSize.x * fontSize);
        }

        if (fontSize < 11.0f)
        {
            fontSize = 11.0f; // Ensure a minimum font size
        }

        indent = ImGui::GetWindowSize().x - (defaultFont->CalcTextSizeA(
            fontSize, FLT_MAX, 0.0f,
            data.text.c_str()).x + 20);

        ImGui::Dummy(ImVec2(indent, 0));
        ImGui::SameLine();
        ImGui::PushFont(defaultFont, fontSize);
        ImGui::Text(data.text.c_str());
        ImGui::PopFont();
        ImGui::Dummy(ImVec2(0, 80 - fontSize)); // Add some space after the text

        if (buttonAreaHeight > 600)
        {
            ImGui::Dummy(ImVec2(0, buttonAreaHeight - 600));
        }

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

        fontSize = buttonHeight * 0.7f; // Adjust font size to fit within button height
        if (fontSize < 11.0f)
        {
            fontSize = 11.0f; // Ensure a minimum font size
        }
        ImGui::PushFont(defaultFont, fontSize);

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
                        if (data.error)
                        {
                            data.text.clear();
                            data.error = false;
                        }
                        if (key.encoded == '\b')
                        {
                            if (!data.text.empty())
                            {
                                data.text.pop_back();
                            }
                            data.enterPressed = false;
                        }
                        else if (key.encoded == '=')
                        {
                            data.enterPressed = true;
                            data.lastExpr = data.text;
                        }
                        else
                        {
                            addCharachter(data, key.encoded);
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

    void addCharachter(CalcInputData &data, ImWchar c)
    {
        if (data.error)
        {
            data.text.clear();
            data.error = false;
        }
        if (isalnum(c) || c == ',' || c == '.' || c == ';')
        {
            if (data.processed)
            {
                data.text.clear();
                data.processed = false;
            }
        }
        data.processed = false;
        if (c == '\r' || c == '\n' || c == '=')
        {
            data.enterPressed = true;
            data.lastExpr = data.text;
            return;
        }

        data.enterPressed = false;
        data.lastExpr = "";
        
        if (c == '(' || c == ')')
        {
            if (data.text.empty())
            {
                if (c == ')')
                {
                    return;
                }
                data.text += c;
                return;
            }

            char lastChar = data.text.back();

            if (c == '(' && (isalnum(lastChar) || lastChar == '('))
            {
                data.text += " * ";
            }

            data.text += c;
        }
        else if (!isalnum(c) && c != '.' && c != ',' && c != ';')
        {

            if (c == ' ')
            {
                return;
            }

            data.text += ' ';
            data.text += c;
            data.text += ' ';
        }
        else if (c != ' ')
        {
            if (!data.text.empty() && data.text.back() == ')' && isalnum(c))
            {
                data.text += " * ";
            }
            data.text += (char)c;
        }
    }
}
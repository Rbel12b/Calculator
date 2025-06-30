/*
 * -----------------------------------------------------------------------------
 *  File:           Expression.h
 *  Project:        Calculator
 *  Author:         Rbel12b (https::/github.com/rbel12b)
 *  Description:    Expression parrser and evaulator
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
#ifndef _EXPRESSION_H_
#define _EXPRESSION_H_

#include <string>
#include <memory>
#include <vector>
#include <stdexcept>
#include "Number.h"

class ASTNode
{
public:
    virtual ~ASTNode() = default;
    virtual Number evaluate() = 0;
};

class BinaryOpNode : public ASTNode
{
public:
    std::string op;
    std::unique_ptr<ASTNode> left, right;

    BinaryOpNode(std::string op, std::unique_ptr<ASTNode> lhs, std::unique_ptr<ASTNode> rhs);

    Number evaluate() override;
};

class UnaryOpNode : public ASTNode
{
public:
    std::string op;
    std::unique_ptr<ASTNode> operand;

    UnaryOpNode(std::string op, std::unique_ptr<ASTNode> operand);

    Number evaluate() override;
};

class LiteralNode : public ASTNode
{
public:
    Number value;

    LiteralNode(Number val);
    LiteralNode(std::string val);

    Number evaluate() override;
};

enum class TokenType
{
    NUMBER,
    OPERATOR,
    UNARY_OPERATOR,
    PARENTHESIS,
    ASSIGNMENT,
    FUNCTION_CALL,
    STRING_LITERAL
};

struct Token
{
    TokenType type;
    std::string value;
};

class Expression
{
public:
    Expression() {}
    Expression(std::string expr) : m_expr(expr) {}

    void set(std::string expr)
    {
        m_expr = expr;
    }

    Number eval();

private:
    void tokenize();
    void parse();
    std::unique_ptr<ASTNode> parsePrimary();
    std::unique_ptr<ASTNode> parseExpression(int minPrecedence);

private:
    std::string m_expr;
    std::unique_ptr<ASTNode> m_ASThead;
    std::vector<Token> m_Tokens;
    size_t index; // index in m_Tokens
};

#endif

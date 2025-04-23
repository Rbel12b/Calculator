/*
 * -----------------------------------------------------------------------------
 *  File:           Expression.cpp
 *  Project:        Calculator
 *  Author:         Rbel12b (https::/github.com/rbel12b)
 *  Description:    Expression parrser and evaulator implementation
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
#include "Expression.h"
#include <unordered_map>

int getPrecedence(const Token &token)
{
    const std::unordered_map<std::string, int> precedence = {
        {"++", 15}, {"--", 15}, {"*", 14}, {"/", 14}, {"%", 14},
        {"+", 13}, {"-", 13}, {"<<", 12}, {">>", 12}, {"<", 11}, {">", 11}, {"<=", 11}, {">=", 11},
        {"==", 10}, {"!=", 10}, {"&", 9}, {"^", 8}, {"|", 7}, {"&&", 6}, {"||", 5}, {"?", 4}, {":", 4},
        {"=", 3}, {"+=", 3}, {"-=", 3}, {"*=", 3}, {"/=", 3}, {"%=", 3}, {"<<=", 3}, {">>=", 3},
        {"&=", 3}, {"^=", 3}, {"|=", 3}, {",", 2},
    };

    if (token.type == TokenType::PARENTHESIS)
    {
        return 0;
    }
    else if (token.type == TokenType::UNARY_OPERATOR)
    {    
        return 16;
    }
    else if (precedence.find(token.value) != precedence.end())
    {
        return precedence.at(token.value);
    }
    return 0;
}

Number evalUnaryOperator(const Number &operand, const std::string &op);
Number evalBinaryOperator(Number &left, const Number &right, const std::string &op);
Number evalArithmeticOperator(const Number &left, const Number &right, const std::string &op);

Number evalUnaryOperator(const Number &operand, const std::string &op)
{
    if (op == "-")
    {
        return -operand;
    }
    else if (op == "+")
    {
        return operand;
    }
    throw std::runtime_error("Unsupported unary operator: " + op);
}

Number evalBinaryOperator(Number &left, const Number &right, const std::string &op)
{
    if (op == "=")
    {
        return left = right;
    }
    else if (op == "+=")
    {
        return left = (left + right);
    }
    else if (op == "-=")
    {
        return left = (left - right);
    }
    else if (op == "*=")
    {
        return left = (left * right);
    }
    else if (op == "/=")
    {
        return left = (left / right);
    }
    return evalArithmeticOperator(left, right, op);
}

Number evalArithmeticOperator(const Number &left, const Number &right, const std::string &op)
{
    if (op == "+")
    {
        return left + right;
    }
    else if (op == "-")
    {
        return left - right;
    }
    else if (op == "*")
    {
        return left * right;
    }
    else if (op == "/")
    {
        return left / right;
    }
    else if (op == "&&")
    {
        return left && right;
    }
    else if (op == "||")
    {
        return left || right;
    }
    else if (op == "==")
    {
        return left == right;
    }
    else if (op == "!=")
    {
        return left != right;
    }
    else if (op == "<")
    {
        return left < right;
    }
    else if (op == "<=")
    {
        return left <= right;
    }
    else if (op == ">")
    {
        return left > right;
    }
    else if (op == ">=")
    {
        return left >= right;
    }
    throw std::runtime_error("Unsupported binary operator: " + op);
}

BinaryOpNode::BinaryOpNode(std::string op, std::unique_ptr<ASTNode> lhs, std::unique_ptr<ASTNode> rhs)
    : op(std::move(op)), left(std::move(lhs)), right(std::move(rhs)) {}

Number BinaryOpNode::evaluate()
{
    Number lhsVal = left->evaluate();
    Number rhsVal = right->evaluate();

    return evalBinaryOperator(lhsVal, rhsVal, op);
}

UnaryOpNode::UnaryOpNode(std::string op, std::unique_ptr<ASTNode> operand)
    : op(std::move(op)), operand(std::move(operand)) {}

Number UnaryOpNode::evaluate()
{
    Number value = operand->evaluate();

    return evalUnaryOperator(value, op);
}

LiteralNode::LiteralNode(Number val) : value(std::move(val)) {}

LiteralNode::LiteralNode(std::string val)
{
    value = std::strtod(val.c_str(), nullptr);
}

Number LiteralNode::evaluate()
{
    return value;
}

Number Expression::eval()
{
    m_Tokens.clear();
    tokenize();
    index = 0;
    parse();
    return m_ASThead->evaluate();
}

void Expression::tokenize()
{
    std::string token;
    Token currentToken;
    uint64_t i = 0;
    while (i < m_expr.size())
    {
        char c = m_expr[i];
        if (std::isspace(c))
        {
            i++;
            continue;
        }
        if (std::isdigit(c))
        {
            token.clear();
            while (i < m_expr.size() && std::isdigit(m_expr[i]))
            {
                token += m_expr[i];
                i++;
            }
            m_Tokens.push_back({TokenType::NUMBER, token});
            continue;
        }
        if (c == '(' || c == ')')
        {
            m_Tokens.push_back({TokenType::PARENTHESIS, std::string(1, c)});
            i++;
            continue;
        }
        if (std::isalpha(c) || c == '_')
        {
            token.clear();
            while (i < m_expr.size() && (std::isalnum(m_expr[i]) || m_expr[i] == '_'))
            {
                token += m_expr[i];
                i++;
            }
            m_Tokens.push_back({TokenType::NUMBER, token});
            continue;
        }
        if (c == '+' || c == '-' || c == '*' || c == '&' || c == '=')
        {
            if (m_Tokens.empty() || m_Tokens.back().type != TokenType::NUMBER)
            {
                m_Tokens.push_back({TokenType::UNARY_OPERATOR, std::string(1, c)});
            }
            else
            {
                m_Tokens.push_back({TokenType::OPERATOR, std::string(1, c)});
            }
            i++;
            continue;
        }
        token.clear();
        while (i < m_expr.size() && std::ispunct(m_expr[i]) && m_expr[i] != '(' && m_expr[i] != ')' && m_expr[i] != '[' && m_expr[i] != ']')
        {
            token += m_expr[i];
            i++;
        }
        m_Tokens.push_back({TokenType::OPERATOR, token});
    }
}

std::unique_ptr<ASTNode> Expression::parsePrimary()
{
    if (index >= m_Tokens.size())
        throw std::runtime_error("Unexpected end of input, index: " + std::to_string(index));

    Token token = m_Tokens[index++];
    if (token.type == TokenType::NUMBER)
    {
        return std::make_unique<LiteralNode>(token.value);
    }
    else if (token.type == TokenType::PARENTHESIS && token.value == "(")
    {
        auto expr = parseExpression(1);
        if (index >= m_Tokens.size() || (m_Tokens[index].type != TokenType::PARENTHESIS && m_Tokens[index].value != ")"))
        {
            if (index >= m_Tokens.size())
            {
                throw std::runtime_error("Expected closing parenthesis, index: " + std::to_string(index));
            }
            throw std::runtime_error("Expected closing parenthesis: " + m_Tokens[index].value + ", index: " + std::to_string(index));
        }
        ++index;
        return expr;
    }
    throw std::runtime_error("Unexpected token in primary expression: " + m_Tokens[index].value + ", index: " + std::to_string(index));
}

std::unique_ptr<ASTNode> Expression::parseExpression(int minPrecedence)
{
    if (minPrecedence == 0)
    {
        minPrecedence = 1;
    }
    std::unique_ptr<ASTNode> lhs;

    if (index >= m_Tokens.size())
        throw std::runtime_error("Unexpected end of input");

    Token token = m_Tokens[index];
    if (token.type == TokenType::UNARY_OPERATOR)
    {
        index++; // Consume the operator
        auto operand = parseExpression(getPrecedence(token) + 1);
        lhs = std::make_unique<UnaryOpNode>(token.value, std::move(operand));
    }
    else
    {
        lhs = parsePrimary();
    }

    // Handle binary operators
    while (index < m_Tokens.size() && getPrecedence(m_Tokens[index]) >= minPrecedence)
    {
        Token opToken = m_Tokens[index++];
        int precedence = getPrecedence(opToken);
        auto rhs = parseExpression(precedence + 1);
        lhs = std::make_unique<BinaryOpNode>(opToken.value, std::move(lhs), std::move(rhs));
    }

    return lhs;
}

void Expression::parse()
{
    index = 0;
    m_ASThead = parseExpression(1);
}
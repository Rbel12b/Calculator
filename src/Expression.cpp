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
#include <cstdint>
#include <set>

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
    value = Number(val);
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
    std::vector<std::string> rawTokens;
    std::string token;
    uint64_t i = 0;

    static const std::set<std::string> multiCharOperators = {
        "==", "!=", "<=", ">=", "&&", "||", "->", "+=", "-=", "*=", "/=", "%=", "&=", "|=", "^=", "<<", ">>"
    };

    // First Pass: Raw token extraction
    while (i < m_expr.size())
    {
        char c = m_expr[i];

        if (std::isspace(c))
        {
            i++;
            continue;
        }

        // --- STRING LITERALS ---
        if (c == '"' || c == '\'')
        {
            char quote = c;
            token.clear();
            token += quote;
            i++;
            while (i < m_expr.size())
            {
                char ch = m_expr[i];
                token += ch;
                i++;
                if (ch == '\\' && i < m_expr.size())  // Escape character
                {
                    token += m_expr[i];
                    i++;
                    continue;
                }
                if (ch == quote)
                    break;
            }
            rawTokens.push_back(token);
            continue;
        }

        // --- FLOATING POINT OR INTEGER NUMBERS ---
        if (std::isdigit(c) || (c == '.' && i + 1 < m_expr.size() && std::isdigit(m_expr[i + 1])) || 
            (c == '0' && i + 1 < m_expr.size() && (m_expr[i + 1] == 'x' || m_expr[i + 1] == 'X' || m_expr[i + 1] == 'b' || m_expr[i + 1] == 'B')))
        {
            token.clear();

            if (m_expr[i] == '0' && i + 1 < m_expr.size() && (m_expr[i + 1] == 'x' || m_expr[i + 1] == 'X'))
            {
                // Hex literal: 0x...
                token += m_expr[i++];
                token += m_expr[i++];
                while (i < m_expr.size() && std::isxdigit(m_expr[i]))
                {
                    token += m_expr[i++];
                }
            }
            else if (m_expr[i] == '0' && i + 1 < m_expr.size() && (m_expr[i + 1] == 'b' || m_expr[i + 1] == 'B'))
            {
                // Binary literal: 0b...
                token += m_expr[i++];
                token += m_expr[i++];
                while (i < m_expr.size() && (m_expr[i] == '0' || m_expr[i] == '1'))
                {
                    token += m_expr[i++];
                }
            }
            else
            {
                // Decimal or floating point
                bool hasDot = false;
                bool hasExp = false;
                while (i < m_expr.size())
                {
                    char ch = m_expr[i];
                    if (std::isdigit(ch))
                    {
                        token += ch;
                        i++;
                    }
                    else if (ch == '.' && !hasDot)
                    {
                        hasDot = true;
                        token += ch;
                        i++;
                    }
                    else if ((ch == 'e' || ch == 'E') && !hasExp)
                    {
                        hasExp = true;
                        token += ch;
                        i++;
                        if (i < m_expr.size() && (m_expr[i] == '+' || m_expr[i] == '-'))
                        {
                            token += m_expr[i++];
                        }
                    }
                    else
                    {
                        break;
                    }
                }

                // Optional float suffix (f/F, l/L)
                if (i < m_expr.size() && (m_expr[i] == 'f' || m_expr[i] == 'F' || m_expr[i] == 'l' || m_expr[i] == 'L'))
                {
                    token += m_expr[i++];
                }
            }

            // Optional integer suffix (u/U, l/L, ul/UL, etc.)
            while (i < m_expr.size() && (m_expr[i] == 'u' || m_expr[i] == 'U' || m_expr[i] == 'l' || m_expr[i] == 'L'))
            {
                token += m_expr[i++];
            }

            rawTokens.push_back(token);
            continue;
        }

        // --- SYMBOLS (identifiers) ---
        if (std::isalpha(c) || c == '_')
        {
            token.clear();
            while (i < m_expr.size() && (std::isalnum(m_expr[i]) || m_expr[i] == '_'))
            {
                token += m_expr[i];
                i++;
            }
            rawTokens.push_back(token);
            continue;
        }

        // --- MULTI-CHARACTER OPERATORS ---
        bool matched = false;
        for (int len = 3; len >= 2; --len)
        {
            if (i + len <= m_expr.size())
            {
                std::string op = m_expr.substr(i, len);
                if (multiCharOperators.count(op))
                {
                    rawTokens.push_back(op);
                    i += len;
                    matched = true;
                    break;
                }
            }
        }
        if (matched) continue;

        // --- SINGLE CHARACTER TOKEN ---
        rawTokens.push_back(std::string(1, m_expr[i]));
        i++;
    }

    // Second Pass: Type assignment
    for (size_t j = 0; j < rawTokens.size(); ++j)
    {
        const std::string &tok = rawTokens[j];
        TokenType type;

        if (tok == "(" || tok == ")")
        {
            type = TokenType::PARENTHESIS;
        }
        else if ((tok.front() == '"' && tok.back() == '"') || (tok.front() == '\'' && tok.back() == '\''))
        {
            type = TokenType::STRING_LITERAL;
        }
        else if (std::isdigit(tok[0]) || tok[0] == '.')
        {
            type = TokenType::NUMBER;
        }
        else if (tok == "+" || tok == "-" || tok == "*" || tok == "&" || tok == "=")
        {
            if (m_Tokens.empty() || (m_Tokens.back().type != TokenType::NUMBER &&
                                m_Tokens.back().type != TokenType::PARENTHESIS &&
                                m_Tokens.back().value != ")"))
                type = TokenType::UNARY_OPERATOR;
            else
                type = TokenType::OPERATOR;
        }
        else
        {
            type = TokenType::OPERATOR;
        }

        m_Tokens.push_back({type, tok});
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
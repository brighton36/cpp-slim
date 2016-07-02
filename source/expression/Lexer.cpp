#include "expression/Lexer.hpp"
#include "expression/Token.hpp"
#include "Error.hpp"
#include <cassert>
namespace slim
{
    namespace expr
    {
        namespace
        {
            bool is_ws_chr(char c)
            {
                return c == ' ' || c == '\t' || c == '\r' || c == '\n';
            }
            bool is_digit(char c)
            {
                return c >= '0' && c <= '9';
            }
            bool is_symbol_start_chr(char c)
            {
                return c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
            }
            bool is_symbol_chr(char c)
            {
                return is_symbol_start_chr(c) || is_digit(c);
            }

        }
        
        void Lexer::error(const std::string &msg)
        {
            throw SyntaxError(msg);
        }

        Token Lexer::next()
        {
            skip_ws();
            if (p == end)
            {
                ++p;
                return Token::END;
            }
            else if (p > end) error("Unexpected end");

            char peek = *p;
            switch (peek)
            {
            case '(': ++p; return Token::LPAREN;
            case ')': ++p; return Token::RPAREN;
            case ',': ++p; return Token::COMMA;
            case '.': ++p; return Token::DOT;
            case '-': ++p; return Token::MINUS;
            case '+': ++p; return Token::PLUS;
            case '*': ++p; return Token::MUL;
            case '/': ++p; return Token::DIV;
            case '%': ++p; return Token::MOD;
            case '\'':
            case '\"':
                return quoted_string();
            case '&':
                if (p + 1 >= end) error("Unexpected end");
                if (p[1] != '&') error("Expected &&");
                p += 2;
                return Token::LOGICAL_AND;
            case '|':
                if (p + 1 >= end) error("Unexpected end");
                if (p[1] != '|') error("Expected ||");
                p += 2;
                return Token::LOGICAL_OR;
            case '!':
                ++p;
                if (p >= end) error("Unexpected end");
                if (p[0] == '=') { ++p; return Token::CMP_NE; }
                else return Token::LOGICAL_NOT;
            case '=':
                if (p + 1 >= end) error("Unexpected end");
                if (p[1] != '=') error("Expected ==");
                p += 2;
                return Token::CMP_EQ;
            case '<':
                ++p;
                if (p >= end) error("Unexpected end");
                if (p[0] == '=')
                {
                    ++p;
                    if (p >= end) error("Unexpected end");
                    if (p[0] == '>')
                    {
                        ++p;
                        return Token::CMP;
                    }
                    else return Token::CMP_LE;
                }
                else return Token::CMP_LT;
            case '>':
                ++p;
                if (p >= end) error("Unexpected end");
                if (p[0] == '=') { ++p; return Token::CMP_GE; }
                else return Token::CMP_GT;
            default:
                if (is_symbol_start_chr(peek)) return symbol();
                else if (is_digit(peek)) return number();
                else error(std::string("Unexpected: ") + peek);
            }
        }
        void Lexer::skip_ws()
        {
            while (p < end && is_ws_chr(*p)) ++p;
        }
        Token Lexer::quoted_string()
        {
            assert(*p == '\'' || *p == '"');
            char delim = *p;
            std::string str;
            ++p;
            while (true)
            {
                if (p >= end) error("Unexpected end in string");
                else if (*p == delim)
                {
                    ++p;
                    break;
                }
                else if (*p == '\\')
                {
                    if (p + 1 >= end) error("Unexpected end in string");
                    switch (p[1])
                    {
                    case '\\': str.push_back('\\'); break;
                    case '\'': str.push_back('\''); break;
                    case '"': str.push_back('\"'); break;
                    case 't': str.push_back('\t'); break;
                    case 'r': str.push_back('\r'); break;
                    case 'n': str.push_back('\n'); break;
                    default: error(std::string("Unknown string escape code ") + p[1]);
                    }
                    p += 2;
                }
                else
                {
                    str.push_back(*p);
                    ++p;
                }
            }
            return { Token::STRING, str };
        }
        Token Lexer::symbol()
        {
            assert(is_symbol_start_chr(*p));
            auto start = p;
            while (p < end && is_symbol_chr(*p)) ++p;
            return { Token::SYMBOL, std::string(start, p - start) };
        }
        Token Lexer::number()
        {
            assert(is_digit(*p));
            auto start = p;
            while (p < end && is_digit(*p)) ++p;

            if (p + 1 < end && p[0] == '.' && is_digit(p[1])) //decimal
            {
                p += 2;
                while (p < end && is_digit(*p)) ++p;
            }

            return{ Token::NUMBER, std::string(start, p - start) };
        }
    }
}
#include "Lexer.h"
#include <algorithm>

// Constructor
Lexer::Lexer(const std::string source, const std::string fileName) : m_current_line_index(0), m_current_line_number(1), m_sourceFileName(fileName)
{
    std::cout << "Create Lexer" << std::endl;
    this->m_sourceText = source;

    // Initialize keyword list
    this->initKeywords();

    std::ofstream out(m_sourceFileName + ".outlexerrors", std::ofstream::out);

    m_errorOutputFile = &out;

    // Tokenize source file 
    this->tokenize();   

    out.close();
    m_errorOutputFile = nullptr;
}

Token* Lexer::getNextToken()
{
    if (m_tokens.empty())
    {
        std::cout << "Error: No tokens found! Must the source file be tokenized? " << std::endl;
        return nullptr;
    }

    Token* token = m_tokens[m_tokenIndex];

    m_tokenIndex++;

    return token;
}

bool Lexer::isFinished()
{
    return m_tokenIndex > m_tokens.size()-1;
}

bool Lexer::isDigit(char chr)
{
    return (chr >= '0' && chr <= '9');
}

bool Lexer::isNonzero(char chr)
{
    return (chr >= 1 || chr <= 9);
}

bool Lexer::isInvalidChar(char chr)
{
    return (chr == '@' || chr == '#' || chr == '$' || chr == '\\' || chr == '~' || chr == '\'' || chr == '_');
}

bool Lexer::isWhiteSpace(char chr)
{
    return chr == ' ' || chr == '\r' || chr == '\t';
}

bool Lexer::isAlpha(char chr)
{
    return (chr >= 'a' && chr <= 'z' || (chr >= 'A' && chr <= 'Z'));
}

bool Lexer::isAlphaNumeric(char chr)
{
    return isAlpha(chr) || isDigit(chr) || chr == '_';
}

bool Lexer::isOperator(char chr)
{
    return (chr == '+' || chr == '-' || chr == '/' || chr == '*');
}

bool Lexer::isPunctuation(char chr)
{
    return (chr == '(' || chr == ')' || chr == ';' || chr == ',');
}

std::string Lexer::getNextWord()
{
    int start = m_current_line_index;
    std::string temp = "";

    while (m_current_line_index < m_sourceText.length() && isAlphaNumeric(m_sourceText[m_current_line_index]) && !isWhiteSpace(m_sourceText[m_current_line_index]))
    {
        if (peekNextChar() == '\n')
        {
            temp += m_sourceText[m_current_line_index];
            return temp;
        }


        temp += m_sourceText[m_current_line_index];
        m_current_line_index++;
    }
    
    return temp;
}

std::string Lexer::getNextNumber()
{
    int start = m_current_line_index;

    bool decimal = false; 

    while (m_current_line_index < m_sourceText.length() && 
        (isDigit(m_sourceText[m_current_line_index])) || 
        m_sourceText[m_current_line_index] == '.')
    {
    
        if (peekNextChar() == '\n')
        {
            return m_sourceText.substr(start, m_current_line_index - (start-1));
        }

        if (m_sourceText[m_current_line_index] == '.')
        {
            if (decimal) break;
            decimal = true;
        }

        m_current_line_index++;
    }

    // Optional e+/-
    if (m_sourceText[m_current_line_index] == 'e')
    {
        m_current_line_index++;

        if (m_sourceText[m_current_line_index] == '+' || m_sourceText[m_current_line_index] == '-')
        {
            m_current_line_index++;
        }
    }

    // Fix this (TODO) - Loops over the end of a float checking trailing 0s
    while (m_current_line_index < m_sourceText.length() &&
        (isDigit(m_sourceText[m_current_line_index])) ||
        m_sourceText[m_current_line_index] == '.')
    {

        if (peekNextChar() == '\n')
        {
            return m_sourceText.substr(start, m_current_line_index - (start - 1));
        }

        if (m_sourceText[m_current_line_index] == '.')
        {
            if (decimal) break;
            decimal = true;
        }

        m_current_line_index++;
    }
    

    return m_sourceText.substr(start, m_current_line_index - start);
}

std::string Lexer::getNextLine()
{  
    std::string temp = m_sourceText;
   
    std::string buffer;

    int buffer_index = 0;

    for (int i = m_current_line_index; i < temp.length(); i++)
    {
        if (temp[i] != '\n')
        {
            buffer.insert(buffer_index, 1, temp[i]);
            buffer_index++;
            
        }
        else
        {
            m_current_line_index = i + 1;
            m_current_line_number++;
            return buffer;
        }
    } 

    return buffer;
}

std::string Lexer::getNextBlock()
{
    std::string temp = m_sourceText;

    std::string buffer;

    int buffer_index = 0;

    for (int i = m_current_line_index; i < temp.length(); i++)
    {    
        if (temp[i] == '*' && peekNextChar() == '/')
        {
            buffer.insert(buffer_index, 1, temp[i]);
            buffer_index++;
            buffer.insert(buffer_index, 1, '/');
            buffer_index++;
            m_current_line_index++;

            // handle imbricated (TODO)
            while (temp[m_current_line_index] != '/')
            {
                if (temp[i] == '\n')
                {
                    buffer.insert(buffer_index, 1, '\\');
                    buffer_index++;
                    buffer.insert(buffer_index, 1, 'n');
                    buffer_index++;
                    m_current_line_index++;
                    m_current_line_number++;
                }
                else 
                {
                    buffer.insert(buffer_index, 1, temp[i]);
                    buffer_index++;
                    m_current_line_index++;
                }
                
            }

            break;

        }        

        // Check newline 
        else if (temp[i] == '\n')
        {
            buffer.insert(buffer_index, 1, '\\');
            buffer_index++;
            buffer.insert(buffer_index, 1, 'n');
            buffer_index++;
            m_current_line_index++;
            m_current_line_number++;
        } 

        else
        {
            buffer.insert(buffer_index, 1, temp[i]);
            buffer_index++;
            m_current_line_index++;
        }
    }

    m_current_line_index++;
    return buffer;
    
}

char Lexer::getNextChar()
{
    if (m_current_line_index != m_sourceText.length())
    {
        char currentChar = m_sourceText[m_current_line_index];
        m_current_line_index++;
        return currentChar;
    }
    else
    {
        std::cout << "End of source file";
        return ' ';
    }
}

char Lexer::peekBackupChar()
{
    if (m_current_line_index != 0)
    {
        int temp_pos = m_current_line_index;
        temp_pos--;
        char backChar = m_sourceText[temp_pos];
        return backChar;
    }
    std::cout << "LOG: Cannot backtrack! " << std::endl;
    return NULL;
}

char Lexer::peekNextChar()
{
    if (m_current_line_index != 0)
    {
        int temp_pos = m_current_line_index;
        temp_pos++;
        char nextChar = m_sourceText[temp_pos];
        return nextChar;
    }
    std::cout << "LOG: Cannot peek next char! " << std::endl;
    return NULL;
}

void Lexer::tokenize()
{
    while (m_current_line_index < m_sourceText.length())
    {
        char currentChar = m_sourceText[m_current_line_index];

        // Skip whitespace 
        if (isWhiteSpace(currentChar))
        {
            m_current_line_index++;
            continue;
        }

        // Check Letter
        else if (isAlpha(currentChar))
        {
            std::string word = getNextWord();

            // Check if it's an operator 

            // Look through map for word
            if (m_keywords.find(word) != m_keywords.end())
            {
                // Add Keywords
                Token* token = createToken(m_keywords[word], word, m_current_line_number);
                m_tokens.push_back(token);  
                m_current_line_index++;
            }
            else
            {
                // Add Identifier
                Token* token = createToken(TokenType::id, word, m_current_line_number);
                m_tokens.push_back(token);
                m_current_line_index++;
            }

        }

        else if (isDigit(currentChar))
        {
            std::string number = getNextNumber();

            std::string s(1, currentChar);
            Token* token = createToken(TokenType::intnum, number, m_current_line_number);
            m_tokens.push_back(token);
            m_current_line_index++;
        
        }

        // Check PLUS
        else if (currentChar == '+')
        {
            Token* token = createToken(TokenType::PLUS, "+", m_current_line_number);
            m_tokens.push_back(token);
            m_current_line_index++;
        }

        // Check Minus or negative number
        else if (currentChar == '-')
        {
            m_current_line_index++;
        }

        // Check MULTI
        else if (currentChar == '*')
        {
            Token* token = createToken(TokenType::MULTI, "*", m_current_line_number);
            m_tokens.push_back(token);
            m_current_line_index++;
        }

        else if (currentChar == ')')
        {
            Token* token = createToken(TokenType::CLOSEPAR, ")", m_current_line_number);
            m_tokens.push_back(token);
            m_current_line_index++;
        }

        else if (currentChar == '(')
        {
            Token* token = createToken(TokenType::OPENPAR, "(", m_current_line_number);
            m_tokens.push_back(token);
            m_current_line_index++;
        }

        else if (currentChar == '[')
        {
            Token* token = createToken(TokenType::OPENSQBR, "[", m_current_line_number);
            m_tokens.push_back(token);
            m_current_line_index++;
        }

        else if (currentChar == ']')
        {
            Token* token = createToken(TokenType::CLOSESQBR, "]", m_current_line_number);
            m_tokens.push_back(token);
            m_current_line_index++;
        }

        else if (currentChar == '{')
        {
            Token* token = createToken(TokenType::OPENCUBR, "{", m_current_line_number);
            m_tokens.push_back(token);
            m_current_line_index++;
        }

        else if (currentChar == '}')
        {
            Token* token = createToken(TokenType::CLOSECUBR, "}", m_current_line_number);
            m_tokens.push_back(token);
            m_current_line_index++;
        }

        else if (currentChar == ';')
        {
            Token* token = createToken(TokenType::SEMI, ";", m_current_line_number);
            m_tokens.push_back(token);
            m_current_line_index++;
        }

        else if (currentChar == ',')
        {
            Token* token = createToken(TokenType::COMMA, ",", m_current_line_number);
            m_tokens.push_back(token);
            m_current_line_index++;
        }

        // Check DOT
        else if (currentChar == '.')
        {
            Token* token = createToken(TokenType::DOT, ".", m_current_line_number);
            m_tokens.push_back(token);
            m_current_line_index++;
        }

        // Check COLON and ASSIGN
        else if (currentChar == ':')
        {
            
            char c = peekNextChar();

            if (c == '=')
            {
                Token* token = createToken(TokenType::ASSIGN, ":=", m_current_line_number);
                m_tokens.push_back(token);
                m_current_line_index++;
            }
            else
            {
                Token* token = createToken(TokenType::COLON, ":", m_current_line_number);
                m_tokens.push_back(token);
                m_current_line_index++;

            }
        }

        // Check GT, GEQ
        else if (currentChar == '>')
        {
            char c = peekNextChar();

            if (c == '=')
            {
                m_current_line_index++;
                Token* token = createToken(TokenType::GEQ, ">=", m_current_line_number);
                m_tokens.push_back(token);
            }
            else
            {
                Token* token = createToken(TokenType::GT, ">", m_current_line_number);
                m_tokens.push_back(token);
            }

            m_current_line_index++;
        }

        // Check LT, LEQ, NOTEQ
        else if (currentChar == '<')
        {

            char c = peekNextChar();

            if (c == '=')
            {
                m_current_line_index++;
                Token* token = createToken(TokenType::LEQ, "<=", m_current_line_number);
                m_tokens.push_back(token);
            }
            else if (c == '>')
            {
                m_current_line_index++;
                Token* token = createToken(TokenType::NOTEQ, "<>", m_current_line_number);
                m_tokens.push_back(token);
                
                
            }
            else
            {
                Token* token = createToken(TokenType::LT, "<", m_current_line_number);
                m_tokens.push_back(token); 
            }

            m_current_line_index++;
        }

        // Check equals and arrow
        else if (currentChar == '=')
        {
            m_current_line_index++;

            char c = m_sourceText[m_current_line_index];

            if (c == '>')
            {
                Token* token = createToken(TokenType::ARROW, "=>", m_current_line_number);
                m_tokens.push_back(token);
                
            }
            else if(c == '=')
            {
                Token* token = createToken(TokenType::EQ, "==", m_current_line_number);
                m_tokens.push_back(token);
                
            }

            m_current_line_index++;
        }

        // Check INLINE CMT, BLOCK CMT, OR DIV
        else if (currentChar == '/')
        {
            char c = peekNextChar();

            // INLINE CMT
            if (c == '/')
            {
                std::string cmt = getNextLine();
                Token* token = createToken(TokenType::inlinecmt, cmt, m_current_line_number);
                m_tokens.push_back(token);
            }

            // BLOCK CMT
            else if (c == '*')
            {
                std::string cmt = getNextBlock();
                Token* token = createToken(TokenType::blockcmt, cmt, m_current_line_number);
                m_tokens.push_back(token);
            }

            //DIV
            else
            {
                Token* token = createToken(TokenType::DIV, "/", m_current_line_number);
                m_tokens.push_back(token);
            }

            m_current_line_index++;

        }

        // Invalid Char
        else if (isInvalidChar(currentChar))
        {
            std::string s(1, currentChar);
            Token* token = createToken(TokenType::invalidchar, s, m_current_line_number);
            m_tokens.push_back(token);

            // LOG 
            std::string output = "Lexical error: Invalid Character: " + s + ": line" + std::to_string(m_current_line_number) + "\n";
            *m_errorOutputFile << output;
            

            m_current_line_index++;
        }

        // Check cartridge and incre. line position - Need to be at the end
        else if (currentChar == '\n')
        {
            m_current_line_number++;
            m_current_line_index++;
            continue;
        }
        
    }

    std::cout << "Finished tokenization.... " << std::endl;
}

Token* Lexer::createToken(TokenType type, std::string value, int position)
{
    Token* token = new Token(type, value, position);
    return token;
}

void Lexer::initKeywords()
{
    // Keywords
    m_keywords["int"] = TokenType::INT;
    m_keywords["float"] = TokenType::FLOAT;
    m_keywords["void"] = TokenType::VOID;
    m_keywords["class"] = TokenType::CLASS;
    m_keywords["self"] = TokenType::SELF;
    m_keywords["isa"] = TokenType::ISA;
    m_keywords["implementation"] = TokenType::IMPLEMENTATION;
    m_keywords["while"] = TokenType::WHILE;
    m_keywords["if"] = TokenType::IF;
    m_keywords["then"] = TokenType::THEN;
    m_keywords["else"] = TokenType::ELSE;
    m_keywords["read"] = TokenType::READ;
    m_keywords["write"] = TokenType::WRITE;
    m_keywords["return"] = TokenType::RETURN;
    m_keywords["local"] = TokenType::LOCAL;
    m_keywords["constructor"] = TokenType::CONSTRUCTOR;
    m_keywords["attribute"] = TokenType::ATTRIBUTE;
    m_keywords["function"] = TokenType::FUNCTION;
    m_keywords["public"] = TokenType::PUBLIC;
    m_keywords["private"] = TokenType::PRIVATE;
    
    // Bitwise OPs
    m_keywords["and"] = TokenType::AND;
    m_keywords["or"] = TokenType::OR;
    m_keywords["not"] = TokenType::NOT;
}

TokenType Lexer::getTokenType(std::string str)
{
    for (auto& it : m_keywords)
    {
        if (it.first == str)
        {
            return it.second;
        }
    }

    // Error is a token type....
    return TokenType::ERROR;
        
}



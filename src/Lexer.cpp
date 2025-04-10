#include "Lexer.h"
#include <algorithm>

// Constructor
Lexer::Lexer(const std::string source, const std::string fileName) : m_current_line_index(0), m_current_line_number(1), m_sourceFileName(fileName)
{
    std::cout << "Create Lexer" << std::endl;
    this->m_sourceText = source;

    // Initialize keyword list
    this->initKeywords();

    std::ofstream outErrors(m_sourceFileName + ".outlexerrors", std::ofstream::out);
    std::ofstream outTokens(m_sourceFileName + ".outlextokens", std::ofstream::out);

    m_errorOutputFile = &outErrors;
    m_tokenOutputFile = &outTokens;

    // Tokenize source file 
    this->tokenize(); 

    Token* EOF_Token = createToken(TokenType::END_OF_FILE, "$", m_current_line_number);
    
    std::cout << "Finished tokenization.... " << std::endl;

    // Clean up output files / nullptrs 
    outErrors.close();
    outTokens.close();

    m_errorOutputFile = nullptr;
    m_tokenOutputFile = nullptr; 

}

Token* Lexer::getNextToken()
{
    if (m_tokenIndex > m_tokens.size()-1)
    {
        std::cout << "Error: No tokens found! Has the source file be tokenized? " << std::endl;
        return nullptr;
    }

    
    Token* token = m_tokens[m_tokenIndex];
    m_tokenIndex++;

    // Skip all comments tokens
    while (token->type == TokenType::blockcmt || token->type == TokenType::inlinecmt)
    {
        token = m_tokens[m_tokenIndex];
        m_tokenIndex++;
    }

    return token;
}

bool Lexer::isFinished()
{
    return m_tokenIndex > m_tokens.size()-1;
}

Token* Lexer::peekAheadToken()
{
    int temp = m_tokenIndex;
    return m_tokens[temp];
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

std::string Lexer::getNextWord()
{
    int start = m_current_line_index;
    std::string temp = "";

    while (m_current_line_index < m_sourceText.length() && isAlphaNumeric(m_sourceText[m_current_line_index]) && !isWhiteSpace(m_sourceText[m_current_line_index]))
    {
        char c = peekNextChar();

        if (c == '\n' || !(isAlphaNumeric(c)))
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
    int end = m_current_line_index;
    bool decimal = false; 

    char c = peekNextChar();
    
    // Return if it's just single digit number
    if (!isDigit(c) && c != '.') return std::string (1,m_sourceText[m_current_line_index]);

    while (end < m_sourceText.length() &&
        (isDigit(m_sourceText[end])) ||
        m_sourceText[end] == '.')
    {
        char c = peekNextChar();
        if (c == '\n' && c != 'e' && !isDigit(c))
        {
            m_current_line_index = end-1;
            return m_sourceText.substr(start, end - (start-1));
        }

        if (m_sourceText[end] == '.')
        {
            if (decimal) break;
            decimal = true;
        }

        end++;
    }

    // Skip over Optional e+/- exponent
    if (m_sourceText[end] == 'e')
    {
        end++;

        if (m_sourceText[end] == '+' || m_sourceText[end] == '-')
        {
            end++;
        }
    }

    // Check E notation exponent 
    while (end < m_sourceText.length() &&
        (isDigit(m_sourceText[end])) ||
        m_sourceText[end] == '.')
    {
        if (peekNextChar() == '\n')
        {
            m_current_line_index = end-1;
            return m_sourceText.substr(start, end - (start - 1));
        }

        end++;
    }    

    m_current_line_index = end-1;
    return m_sourceText.substr(start, end - start);
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
            m_current_line_index++;
        }
        else
        {
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

    int blockcnt = 0;

    while(m_current_line_index < m_sourceText.length())
    {
        if (temp[m_current_line_index] == '*')
        {
            if (blockcnt == 0) blockcnt++;
            else blockcnt--;

            buffer.insert(buffer_index, 1, temp[m_current_line_index]);
            buffer_index++;
        }
        else if (temp[m_current_line_index] == '\n')
        {
            buffer.insert(buffer_index, 1, '\\');
            buffer_index++;
            buffer.insert(buffer_index, 1, 'n');
            buffer_index++;
        }
        else
        {
            buffer.insert(buffer_index, 1, temp[m_current_line_index]);
            buffer_index++;
        }

        m_current_line_index++;

        if (temp[m_current_line_index] == '/' && blockcnt == 0)
        {
            buffer.insert(buffer_index, 1, temp[m_current_line_index]);
            buffer_index++;
            return buffer;
        }
    }

    // Return empty but should be a cmt and flag that the cmt is invalid 
    if (blockcnt != 0)
    {
        return "";
    }

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
        std::cout << "LOG: Cannot get next character - End of source file";
        return NULL;
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
    std::cout << "LOG: Cannot peer backup character! " << std::endl;
    return NULL;
}

char Lexer::peekNextChar()
{
    if (m_current_line_index >= 0)
    {
        int temp_pos = m_current_line_index;
        temp_pos++;
        char nextChar = m_sourceText[temp_pos];
        return nextChar;
    }
    std::cout << "LOG: Cannot peek next character! " << std::endl;
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

            // Look through map for keyword
            if (m_keywords.find(word) != m_keywords.end())
            {
                // Add Keywords
                Token* token = createToken(m_keywords[word], word, m_current_line_number);    
            }
            else
            {
                // Add Identifier
                Token* token = createToken(TokenType::id, word, m_current_line_number);
            }

        }

        // Check number 
        else if (isDigit(currentChar))
        {
            std::string number = getNextNumber();

            bool isFloat = false;
            int decimalIndex = 0;   
            bool hasTrailingZero = false;
            bool hasLeadingZero = false;
            bool contains_E_Notation = false;

            // Check whether it's a float or INT
            for (int i = 0; i < number.length(); i++)
            {
                if (number[i] == '.')
                {
                    isFloat = true;
                    decimalIndex = i;
                }

                // Check valid e notation 
                else if (number[i] == 'e' && number[i+1] != '0')
                {
                    contains_E_Notation = true;
                }
            }

            // Create floatnum
            if (isFloat)
            {
                // Check leading zero float
                if (number[0] == '0' && number.length() > 1 && number[1] != '.')
                {
                    Token* token = createToken(TokenType::invalidnum, number, m_current_line_number);

                    // LOG ERROR
                    logMessage(number, TokenType::invalidnum);
                }
                // Check trailing zero float
                else if (number[number.length()-1] == '0' && number[number.length() - 2] != '.' && !contains_E_Notation)
                {
                    Token* token = createToken(TokenType::invalidnum, number, m_current_line_number);

                    // LOG ERROR
                    logMessage(number, TokenType::invalidnum);

                }
                else
                {
                    Token* token = createToken(TokenType::floatnum, number, m_current_line_number);
                }
            }

            // Create intnum
            else
            {
                // Check for leading int zeros
                if (number[0] == '0' && number.length() > 1)
                {
                    Token* token = createToken(TokenType::invalidnum, number, m_current_line_number);

                    // LOG ERROR
                    logMessage(number, TokenType::invalidnum);
                }
                else
                {
                    Token* token = createToken(TokenType::intnum, number, m_current_line_number);
                }
            }
        
        }

        // Check PLUS
        else if (currentChar == '+')
        {
            Token* token = createToken(TokenType::PLUS, "+", m_current_line_number);
        }

        // Check Minus or negative number
        else if (currentChar == '-')
        {
            Token* token = createToken(TokenType::MINUS, "-", m_current_line_number);
        }

        // Check MULTI
        else if (currentChar == '*')
        {
            Token* token = createToken(TokenType::MULTI, "*", m_current_line_number);
        }

        else if (currentChar == ')')
        {
            Token* token = createToken(TokenType::CLOSEPAR, ")", m_current_line_number);
        }

        else if (currentChar == '(')
        {
            Token* token = createToken(TokenType::OPENPAR, "(", m_current_line_number);
        }

        else if (currentChar == '[')
        {
            Token* token = createToken(TokenType::OPENSQBR, "[", m_current_line_number);
        }

        else if (currentChar == ']')
        {
            Token* token = createToken(TokenType::CLOSESQBR, "]", m_current_line_number);
        }

        else if (currentChar == '{')
        {
            Token* token = createToken(TokenType::OPENCUBR, "{", m_current_line_number);
        }

        else if (currentChar == '}')
        {
            Token* token = createToken(TokenType::CLOSECUBR, "}", m_current_line_number);
        }

        else if (currentChar == ';')
        {
            Token* token = createToken(TokenType::SEMI, ";", m_current_line_number);
        }

        else if (currentChar == ',')
        {
            Token* token = createToken(TokenType::COMMA, ",", m_current_line_number);
        }

        // Check DOT
        else if (currentChar == '.')
        {
            Token* token = createToken(TokenType::DOT, ".", m_current_line_number);
        }

        // Check COLON and ASSIGN
        else if (currentChar == ':')
        {
            char c = peekNextChar();

            if (c == '=')
            {
                Token* token = createToken(TokenType::ASSIGN, ":=", m_current_line_number);
            }
            else
            {
                Token* token = createToken(TokenType::COLON, ":", m_current_line_number);
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
            }
            else
            {
                Token* token = createToken(TokenType::GT, ">", m_current_line_number);
            }

        }

        // Check LT, LEQ, NOTEQ
        else if (currentChar == '<')
        {
            char c = peekNextChar();

            if (c == '=')
            {
                m_current_line_index++;
                Token* token = createToken(TokenType::LEQ, "<=", m_current_line_number);
            }
            else if (c == '>')
            {
                m_current_line_index++;
                Token* token = createToken(TokenType::NOTEQ, "<>", m_current_line_number); 
            }
            else
            {
                Token* token = createToken(TokenType::LT, "<", m_current_line_number);
            }

        }

        // Check equals and arrow
        else if (currentChar == '=')
        {
            m_current_line_index++;

            char c = m_sourceText[m_current_line_index];

            if (c == '>')
            {
                Token* token = createToken(TokenType::ARROW, "=>", m_current_line_number);
            }
            else if(c == '=')
            {
                Token* token = createToken(TokenType::EQ, "==", m_current_line_number);
            }

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
            }

            // BLOCK CMT
            else if (c == '*')
            {

                std::string cmt = getNextBlock();

                // Count all newline counter to be added after creating the token
                int newlineCnt = 0;
                for (int i = 0; i < cmt.length(); i++)
                {
                    if (cmt[i] == '\\' && cmt[i+1] == 'n') newlineCnt++;
                }

                if (cmt == "")
                {
                    Token* token = createToken(TokenType::invalidchar, cmt, m_current_line_number);
                }
                else
                {
                    Token* token = createToken(TokenType::blockcmt, cmt, m_current_line_number);
                }

                m_current_line_number = m_current_line_number + newlineCnt;
            }

            //DIV
            else
            {
                Token* token = createToken(TokenType::DIV, "/", m_current_line_number);
            }

        }

        // Invalid Char
        else if (isInvalidChar(currentChar))
        {
            std::string s(1, currentChar);
            Token* token = createToken(TokenType::invalidchar, s, m_current_line_number);

            // LOG ERROR
            logMessage(s, TokenType::invalidchar);

        }

        // Check cartridge and incre. line position - Need to be at the end
        else if (currentChar == '\n')
        {
            m_current_line_number++;
            m_current_line_index++;
            continue;
        }
        
    }

}

Token* Lexer::createToken(TokenType type, std::string value, int position)
{
    Token* token = new Token(type, value, position);
    m_tokens.push_back(token);
    m_current_line_index++;

    if (m_file_line_number < token->position)
    {
        *m_tokenOutputFile << '\n';
        m_file_line_number++;
    }

    std::string output = "[" + token->convertTokenTypeToString() + ", " + token->lexem + ", " + std::to_string(token->position) + "] ";
    std::cout << output << std::endl;
    *m_tokenOutputFile << output;

    return token;
}

void Lexer::logMessage(std::string s, TokenType tokenType)
{
    switch (tokenType) 
    {
    
        case(TokenType::invalidchar):
        {
            std::string output = "Lexical error: Invalid Character: " + s + ": line" + std::to_string(m_current_line_number) + "\n";
            *m_errorOutputFile << output;
            break;
        }

        case(TokenType::invalidid):
        {
            std::string output = "Lexical error: Invalid ID: " + s + ": line" + std::to_string(m_current_line_number) + "\n";
            *m_errorOutputFile << output;
            break;
        }

        case(TokenType::invalidnum):
        {
            std::string output = "Lexical error: Invalid Number: " + s + ": line" + std::to_string(m_current_line_number) + "\n";
            *m_errorOutputFile << output;
            break;
        }

        default:
        {
            std::string output = "Logging error: No TokenType matching invalid types.... check logMessage()\n";
            *m_errorOutputFile << output;
        }
    
    }

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



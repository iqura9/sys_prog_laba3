#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cctype>

// Типи лексем
enum TokenType {
    NUMBER,
    STRING,
    DIRECTIVE,
    IDENTIFIER,
    COMMENT,
    OPERATOR,
    PUNCTUATION,
    ERROR
};

// Клас лексеми
struct Token {
    std::string lexeme;
    TokenType type;
};

// Функція для визначення типу лексеми
TokenType getTokenType(const std::string& token) {
    if (isdigit(token[0])) {
        // Перевірка чисел, включаючи десяткові
        bool hasDecimal = false;
        for (char ch : token) {
            if (!isdigit(ch)) {
                if (ch == '.' && !hasDecimal) {
                    hasDecimal = true;
                } else {
                    // Якщо знайдено нецифровий символ, і не є це десятковий роздільник,
                    // то лексема не є числом.
                    return IDENTIFIER;
                }
            }
        }

        return hasDecimal ? NUMBER : NUMBER;
    } else if (token.front() == '"' && token.back() == '"') {
        // Check if the token is a string
        return STRING;
    } else if (token[0] == '#') {
        // Перевірка директив препроцесора
        return DIRECTIVE;
    } else if (token.substr(0, 2) == "//") {
        // Перевірка однорядкового коментаря
        return COMMENT;
    } else if (token.substr(0, 2) == "/*") {
        // Перевірка багаторядкового коментаря
        return COMMENT;
    } else if (isalpha(token[0]) || token[0] == '_') {
        // Перевірка ідентифікаторів
        return IDENTIFIER;
    } else if (token.size() == 1 && ispunct(token[0])) {
        // Перевірка операторів
        return OPERATOR;
    }else if (ispunct(token[0])) {
        // Перевірка розділових знаків
        return PUNCTUATION;
    }
    else {
        // Помилка для нерозпізнаваних символів
        return ERROR;
    }
}// Функція для розділення рядка на лексеми
std::vector<Token> tokenize(const std::string& input) {
    std::vector<Token> tokens;
    std::string token;
    bool inSingleLineComment = false;
    bool inMultiLineComment = false;
    bool inStringLiteral = false;  // Track if we are inside a string literal

    for (size_t i = 0; i < input.size(); ++i) {
        if (inSingleLineComment) {
            // Пропуск символів під час однорядкового коментаря
            if (input[i] == '\n' || i == input.size() - 1) {
                inSingleLineComment = false;
                tokens.push_back({token, COMMENT});
                token.clear();
            } else {
                token += input[i];
            }
        } else if (inMultiLineComment) {
            // Обробка багаторядкового коментаря
            token += input[i];
            if (input.substr(i, 2) == "*/") {
                inMultiLineComment = false;
                tokens.push_back({token, COMMENT});
                token.clear();
                i += 1;  // Skip the '/'
            }
        } else if (inStringLiteral) {
            // Обробка строкового літералу
            token += input[i];
            if (input[i] == '"' && input[i - 1] != '\\') {
                // Кінець строкового літералу
                inStringLiteral = false;
                tokens.push_back({token, STRING});
                token.clear();
            }
        } else if (isspace(input[i])) {
            // Пропуск пробілів
            if (!token.empty()) {
                TokenType type = getTokenType(token);
                tokens.push_back({token, type});
                token.clear();
            }
        } else if (input.substr(i, 2) == "//") {
            // Обробка однорядкового коментаря
            inSingleLineComment = true;
            token += input[i];
        } else if (input.substr(i, 2) == "/*") {
            // Обробка початку багаторядкового коментаря
            inMultiLineComment = true;
            token += input[i];
            tokens.push_back({token, COMMENT});
            token.clear();
        } else if (input[i] == '"') {
            // Початок строкового літералу
            inStringLiteral = true;
            token += input[i];
        } else if (ispunct(input[i])) {
            // Додавання символу розділового знаку або оператора
            if (!token.empty()) {
                TokenType type = getTokenType(token);
                tokens.push_back({token, type});
                token.clear();
            }

            // Лексема - одиночний розділовий знак або оператор
            token = input[i];
            TokenType type;

            if (input[i] == '+' || input[i] == '-' || input[i] == '*' || input[i] == '/' || input[i] == '%' ||
                input[i] == '<' || input[i] == '>' || input[i] == '=' || input[i] == '&' || input[i] == '|' ||
                input[i] == '!' || input[i] == '^') {
                type = OPERATOR;
            } else {
                type = PUNCTUATION;
            }

            tokens.push_back({token, type});
            token.clear();
        } else {
            // Додавання символів до поточної лексеми
            token += input[i];
        }
    }

    // Обробка останньої лексеми
    if (!token.empty()) {
        TokenType type = getTokenType(token);
        tokens.push_back({token, type});
    }

    return tokens;
}





// Функція для виводу результатів
void printTokens(const std::vector<Token>& tokens) {
    for (const auto& token : tokens) {
        std::cout << "< " << token.lexeme << ", ";

        // Вивід типу лексеми
        switch (token.type) {
            case NUMBER:
                std::cout << "NUMBER";
                break;
            case STRING:
                std::cout << "STRING";
                break;
            case DIRECTIVE:
                std::cout << "DIRECTIVE";
                break;
            case IDENTIFIER:
                std::cout << "IDENTIFIER";
                break;
            case OPERATOR:
                std::cout << "OPERATOR";
                break;
            case COMMENT:
                std::cout << "COMMENT";
                break;
            case PUNCTUATION:
                std::cout << "PUNCTUATION";
                break;
            case ERROR:
                std::cout << "ERROR";
                break;
        }

        std::cout << " >" << std::endl;
    }
}

int main() {
    std::string filename;
    std::cout << "Enter the name of the file containing C++ code: ";
    std::cin >> filename;


    // Open the file
    std::ifstream file("../"+filename);

    if (!file.is_open()) {
        std::cerr << "Error opening file " << filename << std::endl;
        return 1;
    }

    // Read content of the file
    std::string input((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    file.close();

    // Tokenize and print the results
    std::vector<Token> tokens = tokenize(input);
    printTokens(tokens);

    return 0;
}

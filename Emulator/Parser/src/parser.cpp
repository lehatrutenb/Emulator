#include <iostream>
#include <string>
#include <vector>
#include <regex>

namespace tokenparser {

enum class TokenType {
    UNDEFINED,
    END_OF_FILE,
    WORD,
    INTEGER,
    DELIMITER,
};

class Token {
    public:
        Token(TokenType tt, std::string t_val) : type(tt), val(t_val) {};
        TokenType type;
        std::string val;
};

class TokenParser {
public:
    TokenParser(const std::string& input) : input(input) {};

    Token parse() {
        if (last_index >= input.size()) {
            return Token(TokenType::END_OF_FILE, "");
        }

        Token to_return = Token(TokenType::UNDEFINED, "");
        int ind = -1;
        for (auto tt : token_regex) {
            std::smatch match;
            if (std::regex_search(input.cbegin() + last_index, input.cbegin() + last_index + max_regex_len, match, tt.second)) {
                if (ind == -1 || ind > match.position()) {
                    ind = match.position();
                    to_return = Token(tt.first, match.str());
                }
            }
        }

        if (ind != -1) {
            last_index += ind + to_return.val.size();
        }

        return to_return;
    }

    void set_max_regex_len(int x) {
        max_regex_len = x;
    }

private:
    const std::string &input;
    int max_regex_len = 20;
    int last_index = 0;
    std::smatch last_match_;
    std::vector<std::pair<TokenType, std::regex>> token_regex = {
            {TokenType::DELIMITER,     std::regex("[\n ]")},
            {TokenType::INTEGER, std::regex("[-+]?[0-9]+")},
            {TokenType::WORD,  std::regex("[a-zA-Z0-9_][a-zA-Z0-9_]*")},
    }; // order matters - more exect should go first
};

}

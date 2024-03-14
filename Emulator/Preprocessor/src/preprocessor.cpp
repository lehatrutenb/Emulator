#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <variant>

#include "../../Emulator/include/commands.hpp"
#include "../../Parser/src/parser.cpp"

namespace preprocessor {
    typedef std::vector<std::variant<commands::Command*, int>> program_t;

    class Preprocessor {
        public:
            program_t preprocess(const std::string&& input) {
                //std::cout << input << std::endl;
                program_t program;
                auto parser = tokenparser::TokenParser(input);
                tokenparser::Token token = parser.parse();
                std::vector<tokenparser::Token> tokens = {token};
                int labelIndex = 0;
                std::map<std::string, int> label_convert;
                while (token.type != tokenparser::TokenType::END_OF_FILE) {
                    //std::cout << token.val << std::endl;
                    std::pair<bool, int> reg_check = check_for_reg(token.val);
                    if (reg_check.first) {
                        //std::cout << 1 << std::endl;
                        program.push_back(reg_check.second);
                    } else if (convert.find(token.val) != convert.end()) {
                        program.push_back(convert[token.val]);
                    }  else if (token.type == tokenparser::TokenType::INTEGER) {
                        program.push_back(std::stoi(token.val));
                    } else if (token.type == tokenparser::TokenType::WORD) {
                        if (label_convert.find(token.val) != label_convert.end()) {
                            label_convert[token.val] = labelIndex++;
                        }
                        program.push_back(label_convert[token.val]);
                    }

                    token = parser.parse();
                    tokens.push_back(token);
                }

                for (int i = 0; i < tokens.size();i++) {
                    if (tokens[i].type == tokenparser::TokenType::WORD) {
                        std::cout << "word" << std::endl;
                    }
                    if (tokens[i].type == tokenparser::TokenType::DELIMITER) {
                        std::cout << "DELIM" << std::endl;
                    }
                    if (tokens[i].type == tokenparser::TokenType::END_OF_FILE) {
                        std::cout << "END" << std::endl;
                    }
                }

                if (!check_begin_end(std::move(tokens))) {
                    throw std::invalid_argument("Program must begin with BEGIN and end with END commands");
                }

                for (int i = 0; i < program.size(); i++) {
                    if (std::holds_alternative<int>(program[i])) {
                        continue;
                    }
                    if (std::get<commands::Command*>(program[i])->GetArgsAmt() == 1) {
                        if (i + 1 >= program.size() || !std::holds_alternative<int>(program[i + 1])) {
                            throw std::invalid_argument("Don't have correct args after command");
                        }
                    }
                }

                return program;
            }

        private:
            std::pair<bool, int> check_for_reg(std::string arg) {
                if (arg.size() == 4 && std::string{arg[0], arg[1], arg[2]} == "reg" && arg[3] >= '0' && arg[3] <= '3') {
                    return std::make_pair(true, arg[3] - '0');
                }
                return std::make_pair(false, 0);
            }

            bool check_begin_end(const std::vector<tokenparser::Token>&& tokens) {
                if (tokens.empty()) {
                    return false;
                }
                int firstNonDelim = -1;
                int lastNonDelim = tokens.size() - 1;
                for (;firstNonDelim + 1 < tokens.size(); firstNonDelim++) {
                    if (tokens[firstNonDelim + 1].type != tokenparser::TokenType::DELIMITER) {
                        break;
                    }
                }
                for (;lastNonDelim - 1 >= 0; lastNonDelim--) {
                    if (tokens[lastNonDelim - 1].type != tokenparser::TokenType::DELIMITER) {
                        break;
                    }
                }
                for (int i = firstNonDelim + 2; i < lastNonDelim - 1; i++) {
                    if (tokens[i].val == "BEGIN" || tokens[i].val == "END") {
                        return false;
                    }
                }
                return firstNonDelim + 1 < tokens.size() && lastNonDelim - 1 >= 0 && tokens[firstNonDelim + 1].val == "BEGIN" && tokens[lastNonDelim - 1].val == "END";
            }

            std::map<std::string, commands::Command*> convert = {
                {"BEGIN", new commands::Begin_c()},
                {"END", new commands::End_c()},
                {"PUSH", new commands::Push_c()},
                {"POP", new commands::Pop_c()},
                {"PUSHR", new commands::Pushr_c()},
                {"POPR", new commands::Popr_c()},
                {"ADD", new commands::Add_c()},
                {"SUB", new commands::Sub_c()},
                {"MUL", new commands::Mul_c()},
                {"DIV", new commands::Div_c()},
                {"OUT", new commands::Out_c()},
                {"IN", new commands::In_c()},
                {"JMP", new commands::Jmp_c()},
                {"JEQ", new commands::Jeq_c()},
                {"JNE", new commands::Jne_c()},
                {"JA", new commands::Ja_c()},
                {"JAE", new commands::Jae_c()},
                {"JB", new commands::Jb_c()},
                {"JBE", new commands::Jbe_c()},
                {"CALL", new commands::Call_c()},
                {"RET", new commands::Ret_c()}
            };
    };
}
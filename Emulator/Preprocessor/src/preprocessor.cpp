#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <variant>

#include "../../Emulator/include/commands.hpp"
#include "../../Parser/src/parser.cpp"

namespace preprocessor {
    class Preprocessor {
        public:
            std::vector<std::variant<std::shared_ptr<commands::Command>, int>> preprocess(const std::string&& input) {
                std::vector<std::variant<std::shared_ptr<commands::Command>, int>> program;
                auto parser = tokenparser::TokenParser(input);
                tokenparser::Token token = parser.parse();
                std::vector<tokenparser::Token> tokens = {token};
                std::vector<std::pair<int, std::string>> labels_index_value;

                std::map<std::string, int> label_convert;
                int need_args_amt = 0;
                while (token.type != tokenparser::TokenType::END_OF_FILE) {
                    std::pair<bool, int> reg_check = check_for_reg(token.val);
                    if (reg_check.first) {
                        program.push_back(reg_check.second);
                        need_args_amt--;
                    } else if (convert.find(token.val) != convert.end()) {
                        program.push_back(convert[token.val]);
                        need_args_amt += std::get<std::shared_ptr<commands::Command>>(program.back())->GetArgsAmt();
                    }  else if (token.type == tokenparser::TokenType::INTEGER) {
                        program.push_back(std::stoi(token.val));
                        need_args_amt--;
                    } else if (token.type == tokenparser::TokenType::WORD) {
                        if (need_args_amt == 0) {
                            label_convert[token.val] = program.size();
                        } else {
                            need_args_amt--;
                        }
                        program.push_back(0);
                        labels_index_value.push_back(std::make_pair(program.size() - 1, token.val));
                    }

                    token = parser.parse();
                    tokens.push_back(token);
                }

                // write labels integers to program
                for (auto label : labels_index_value) {
                    if (label_convert.find(label.second) == label_convert.end()) {
                        throw std::invalid_argument("Try to use unplaced label");
                    }
                    program[label.first] = label_convert[label.second];
                }

                if (!check_begin_end(std::move(tokens))) {
                    throw std::invalid_argument("Program must have BEGIN and END commands");
                }

                if (!check_args_amount(std::move(program))) {
                    throw std::invalid_argument("Don't have correct args after command");
                }

                if (!check_tokens_undefined(std::move(tokens))) {
                    throw std::invalid_argument("Got undefined text in program");
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

                int begin_ind = -1, end_ind = -1;
                for (int i = 0; i < tokens.size(); i++) {
                    if (tokens[i].val == "BEGIN") {
                        if (begin_ind != -1) {
                            return false;
                        }
                        begin_ind = i;
                    }
                    if (tokens[i].val == "END") {
                        if (end_ind != -1) {
                            return false;
                        }
                        end_ind = i;
                    }
                }
                return begin_ind != -1 && end_ind != -1 && begin_ind < end_ind;
            }

            bool check_tokens_undefined(const std::vector<tokenparser::Token>&& tokens) {
                for (auto token : tokens) {
                    if (token.type == tokenparser::TokenType::UNDEFINED) {
                        return false;
                    }
                }
                return true;
            }

            bool check_args_amount(const std::vector<std::variant<std::shared_ptr<commands::Command>, int>>&& program) {
                for (int i = 0; i < program.size(); i++) {
                    if (std::holds_alternative<int>(program[i])) {
                        continue;
                    }
                    if (std::get<std::shared_ptr<commands::Command>>(program[i])->GetArgsAmt() == 1) {
                        if (i + 1 >= program.size() || !std::holds_alternative<int>(program[i + 1])) {
                            return false;
                        }
                    }
                }
                return true;
            }

            std::map<std::string, std::shared_ptr<commands::Command>> convert = {
                {"BEGIN", std::make_shared<commands::Begin_c>()},
                {"END", std::make_shared<commands::End_c>()},
                {"PUSH", std::make_shared<commands::Push_c>()},
                {"POP", std::make_shared<commands::Pop_c>()},
                {"PUSHR", std::make_shared<commands::Pushr_c>()},
                {"POPR", std::make_shared<commands::Popr_c>()},
                {"ADD", std::make_shared<commands::Add_c>()},
                {"SUB", std::make_shared<commands::Sub_c>()},
                {"MUL", std::make_shared<commands::Mul_c>()},
                {"DIV", std::make_shared<commands::Div_c>()},
                {"OUT", std::make_shared<commands::Out_c>()},
                {"IN", std::make_shared<commands::In_c>()},
                {"JMP", std::make_shared<commands::Jmp_c>()},
                {"JEQ", std::make_shared<commands::Jeq_c>()},
                {"JNE", std::make_shared<commands::Jne_c>()},
                {"JA", std::make_shared<commands::Ja_c>()},
                {"JAE", std::make_shared<commands::Jae_c>()},
                {"JB", std::make_shared<commands::Jb_c>()},
                {"JBE", std::make_shared<commands::Jbe_c>()},
                {"CALL", std::make_shared<commands::Call_c>()},
                {"RET", std::make_shared<commands::Ret_c>()},
                {"DEBUG", std::make_shared<commands::Debug_c>()}
            };
    };
}
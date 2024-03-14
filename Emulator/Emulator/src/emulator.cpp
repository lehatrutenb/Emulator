#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "../../Preprocessor/src/preprocessor.cpp"

namespace emulator {

class Emulator {
    public:
        Emulator(const std::string&& path) : pathToFile(path) {}
        void inline run() {
            preprocessor::Preprocessor preprocessor;
            runtime::State state;

            preprocessor::program_t program = preprocessor::Preprocessor().preprocess(std::move(read_file()));
            std::vector<int> labels;
            for (int i = 0; i < program.size(); i++) {
                auto command = program[i];
                if (std::holds_alternative<int>(command)) {
                    continue;
                } else {
                    if (std::get<commands::Command*>(command)->GetArgsAmt() == 1) {
                        state.arg = std::get<int>(program[i + 1]);
                    }
                    std::get<commands::Command*>(command)->Do(std::move(state));
                }
            }
        }
    
    private:
        const std::string pathToFile;
        const std::string read_file() {
            std::string text, line;
            std::ifstream file(pathToFile);

            if (file.is_open()) {
                while (getline(file, line)) {
                    text += line + '\n';
                }
            }
            file.close();

            return text;
        }
};

}
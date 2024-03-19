#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stack>
#include "../../Preprocessor/src/preprocessor.cpp"
#include "../include/emulator.hpp"

namespace emulator {

Emulator::Emulator(const std::string&& path) : pathToFile(path) {}

void Emulator::run() {
    preprocessor::Preprocessor preprocessor;
    runtime::State state;

    auto program = preprocessor::Preprocessor().preprocess(std::move(read_file()));
    std::vector<int> labels;
    for (int i = 0; i < program.size(); i++) {
        auto instruction = program[i];
        if (std::holds_alternative<int>(instruction)) {
            continue;
        } else {
            auto command = std::get<std::shared_ptr<commands::Command>>(instruction);
            if (command->GetArgsAmt() == 1) {
                state.regs[4] = std::get<int>(program[i + 1]);
            }
            handle_execution_branching(std::move(state), command, i);

            if (!this->emulState.isProgrammRunning()) {
                continue;
            }

            command->Do(std::move(state));
            if (state.regs[5] != -1) {
                i = state.regs[5];
                state.regs[5] = -1;
            }
        }
    }
}

void Emulator::handle_execution_branching(runtime::State&& state, std::shared_ptr<commands::Command> command, int command_index) {
    switch (command->GetCommandType()) {
        case commands::CommandType::BEGIN_COMMAND:
            this->emulState.setProgrammIsRunningState(true);
            break;
        case commands::CommandType::END_COMMAND:
            this->emulState.setProgrammIsRunningState(false);
            break;
        case commands::CommandType::CALL_COMMAND:
            if (this->emulState.isProgrammRunning()) {
                this->emulState.setState(command_index, state.stack.size());
            }
            break;
        case commands::CommandType::RETURN_COMMAND:
            if (this->emulState.isProgrammRunning()) {
                state.regs[4] = this->emulState.getSavedStackIndex();
                state.regs[5] = this->emulState.getSavedCommandIndex();
            }
            break;
        default:
            break;
    }
}

const std::string Emulator::read_file() {
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

}
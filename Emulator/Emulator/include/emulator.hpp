#ifndef EMULATOR
#define EMULATOR

#include <string>

namespace emulator {

class EmulatorState {
    public:
        void setState(int commandIndex, int stackLastIndex) {
            functionCallCommandIndexStack.push(commandIndex);
            functionCallIndexStack.push(stackLastIndex);
        }
        int getSavedStackIndex() {
            int value = functionCallIndexStack.top();
            functionCallIndexStack.pop();
            return value;
        }
        int getSavedCommandIndex() {
            int value = functionCallCommandIndexStack.top();
            functionCallCommandIndexStack.pop();
            return value;
        }
        bool isProgrammRunning() {
            return programIsRunning;
        }
        void setProgrammIsRunningState(bool state) {
            programIsRunning = state;
        }
    private:
        std::stack<int> functionCallCommandIndexStack;
        std::stack<int> functionCallIndexStack;
        bool programIsRunning = false;
};

class Emulator {
    public:
        Emulator(const std::string&&);
        void run();

    private:
        const std::string pathToFile;
        const std::string read_file();
        EmulatorState emulState;
        void handle_execution_branching(runtime::State&&, std::shared_ptr<commands::Command>, int);
};

}

#endif
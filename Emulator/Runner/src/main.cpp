//#include "../../Emulator/include/emulator.hpp"
#include "../../Emulator/src/emulator.cpp"
#include "../include/runner.hpp"

int main() {
    emulator::Emulator emulator(PATH_TO_PROGRAM);
    emulator.run();
}
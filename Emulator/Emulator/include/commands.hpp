#include <iostream>
#include <string>
#include <vector>
#include "../../Stack/include/stack.hpp"

namespace runtime {
class State {
    public:
        myStack::stack<int> stack;
        int regs[6] = {0, 0, 0, 0, -1, -1}; // 0-4 : user regs 5-6: emulator regs (4: command arg, 5: command index)
};
}

namespace commands {

enum class CommandType {
    BASE_COMMAND,
    BEGIN_COMMAND,
    END_COMMAND,
    CALL_COMMAND,
    RETURN_COMMAND
};

class Command {
    public:
        virtual inline int GetArgsAmt() = 0;
        virtual inline void Do(runtime::State&&) = 0;
        virtual inline CommandType GetCommandType() {
            return CommandType::BASE_COMMAND;
        };
};

class Begin_c : public Command { // _c like _t
    int GetArgsAmt() override { return 0; }
    void Do(runtime::State&& _) override {}
    CommandType GetCommandType() override {
        return CommandType::BEGIN_COMMAND;
    };
};

class End_c : public Command {
    int GetArgsAmt() override { return 0; }
    void Do(runtime::State&& _) override {}
    CommandType GetCommandType() override {
        return CommandType::END_COMMAND;
    };
};

class Push_c : public Command {
    int GetArgsAmt() override {
        return 1;
    }
    void Do(runtime::State&& state) override {
        state.stack.push(state.regs[4]);
    }
};

class Pop_c : public Command {
    int GetArgsAmt() override { return 0; }
    void Do(runtime::State&& state) override {
        state.stack.pop();
    }
};

class Pushr_c : public Command {
    int GetArgsAmt() override {
        return 1;
    }
    void Do(runtime::State&& state) override {
        state.stack.push(state.regs[state.regs[4]]);
    }
};

class Popr_c : public Command {
    int GetArgsAmt() override {
        return 1;
    }
    void Do(runtime::State&& state) override {
        state.regs[state.regs[4]] = state.stack.pop();
    }
};

class Add_c : public Command {
    int GetArgsAmt() override { return 0; }
    void Do(runtime::State&& state) override {
        state.stack.push(state.stack.pop() + state.stack.pop());
    }
};

class Sub_c : public Command {
    int GetArgsAmt() override { return 0; }
    void Do(runtime::State&& state) override {
        state.stack.push(state.stack.pop() - state.stack.pop());
    }
};

class Mul_c : public Command {
    int GetArgsAmt() override { return 0; }
    void Do(runtime::State&& state) override {
        state.stack.push(state.stack.pop() * state.stack.pop());
    }
};

class Div_c : public Command {
    int GetArgsAmt() override { return 0; }
    void Do(runtime::State&& state) override {
        int a = state.stack.pop(), b = state.stack.pop();
        if (b == 0 || a % b != 0) {
            throw std::invalid_argument("Arguments in divide must divide mathimatically");
        } else {
            state.stack.push(a / b);
        }
    }
};

class Out_c : public Command {
    int GetArgsAmt() override { return 0; }
    void Do(runtime::State&& state) override {
        std::cout << state.stack.pop();
    }
};

class In_c : public Command {
    int GetArgsAmt() override { return 0; }
    void Do(runtime::State&& state) override {
        int x;
        std::cin >> x;
        state.stack.push(x);
    }
};

class Jmp_c : public Command {
    int GetArgsAmt() override { return 1; }
    void Do(runtime::State&& state) override {
        state.regs[5] = state.regs[4];
    }
};

class Jeq_c : public Command {
    int GetArgsAmt() override { return 1; }
    void Do(runtime::State&& state) override {
        if (state.stack.pop() == state.stack.pop()) {
            state.regs[5] = state.regs[4];
        }
    }
};

class Jne_c : public Command {
    int GetArgsAmt() override { return 1; }
    void Do(runtime::State&& state) override {
        if (state.stack.pop() != state.stack.pop()) {
            state.regs[5] = state.regs[4];
        }
    }
};

class Ja_c : public Command {
    int GetArgsAmt() override { return 1; }
    void Do(runtime::State&& state) override {
        if (state.stack.pop() > state.stack.pop()) {
            state.regs[5] = state.regs[4];
        }
    }
};

class Jae_c : public Command {
    int GetArgsAmt() override { return 1; }
    void Do(runtime::State&& state) override {
        if (state.stack.pop() >= state.stack.pop()) {
            state.regs[5] = state.regs[4];
        }
    }
};

class Jb_c : public Command {
    int GetArgsAmt() override { return 1; }
    void Do(runtime::State&& state) override {
        if (state.stack.pop() < state.stack.pop()) {
            state.regs[5] = state.regs[4];
        }
    }
};

class Jbe_c : public Command {
    int GetArgsAmt() override { return 1; }
    void Do(runtime::State&& state) override {
        if (state.stack.pop() <= state.stack.pop()) {
            state.regs[5] = state.regs[4];
        }
    }
};

class Call_c : public Command {
    int GetArgsAmt() override { return 1; }
    void Do(runtime::State&& state) override {
        state.regs[5] = state.regs[4];
    }
    CommandType GetCommandType() override {
        return CommandType::CALL_COMMAND;
    };
};

class Ret_c : public Command {
    int GetArgsAmt() override { return 0; }
    void Do(runtime::State&& state) override {
        while (state.stack.size() > state.regs[4]) {
            state.stack.pop();
        }
    }
    CommandType GetCommandType() override {
        return CommandType::RETURN_COMMAND;
    };
};

class Debug_c : public Command {
    int GetArgsAmt() override { return 0; }
    void Do(runtime::State&& state) override {
        std::cout << std::endl;
        for (int i = 0; i < 6; i++) {
            std::cout << "reg" << i << ": " << state.regs[i] << std::endl;
        }

        std::cout << std::endl << "stack:" << std::endl;
        std::vector<int> tmp_stack;
        while (state.stack.size() != 0) {
            tmp_stack.push_back(state.stack.pop());
            std::cout << tmp_stack.back();
        }
        while (!tmp_stack.empty()) {
            state.stack.push(tmp_stack.back());
            tmp_stack.pop_back();
        }
        std::cout << '\n' << std::endl;
    }
};

}
#include <iostream>
#include <string>
#include <vector>
#include "../../Stack/include/stack.hpp"

namespace runtime {
class State {
    public:
        myStack::stack<int> s;
        int regs[4] = {0, 0, 0, 0};
        int arg;
};
}

namespace commands {

class Command {
    public:
        virtual inline int GetArgsAmt() = 0;
        virtual inline void Do(runtime::State&&) = 0;
};

class Begin_c : public Command { // _c like _t
    int GetArgsAmt() override { return 0; }
    void Do(runtime::State&& _) override {}
};

class End_c : public Command {
    int GetArgsAmt() override { return 0; }
    void Do(runtime::State&& _) override {}
};

class Push_c : public Command {
    int GetArgsAmt() override {
        return 1;
    }
    void Do(runtime::State&& s) override {
        s.s.push(s.arg);
    }
};

class Pop_c : public Command {
    int GetArgsAmt() override { return 0; }
    void Do(runtime::State&& s) override {
        s.s.pop();
    }
};

class Pushr_c : public Command {
    int GetArgsAmt() override {
        return 1;
    }
    void Do(runtime::State&& s) override {
        s.s.push(s.regs[s.arg]);
    }
};

class Popr_c : public Command {
    int GetArgsAmt() override {
        return 1;
    }
    void Do(runtime::State&& s) override {
        s.regs[s.arg] = s.s.pop();
    }
};

class Add_c : public Command {
    int GetArgsAmt() override { return 0; }
    void Do(runtime::State&& s) override {
        s.s.push(s.s.pop() + s.s.pop());
    }
};

class Sub_c : public Command {
    int GetArgsAmt() override { return 0; }
    void Do(runtime::State&& s) override {
        s.s.push(s.s.pop() - s.s.pop());
    }
};

class Mul_c : public Command {
    int GetArgsAmt() override { return 0; }
    void Do(runtime::State&& s) override {
        s.s.push(s.s.pop() * s.s.pop());
    }
};

class Div_c : public Command {
    int GetArgsAmt() override { return 0; }
    void Do(runtime::State&& s) override {
        int a = s.s.pop(), b = s.s.pop();
        if (b == 0 || a % b != 0) {
            throw std::invalid_argument("Arguments in divide must divide mathimatically");
        } else {
            s.s.push(a / b);
        }
    }
};

class Out_c : public Command {
    int GetArgsAmt() override { return 0; }
    void Do(runtime::State&& s) override {
        std::cout << s.s.pop();
    }
};

class In_c : public Command {
    int GetArgsAmt() override { return 0; }
    void Do(runtime::State&& s) override {
        int x;
        std::cin >> x;
        s.s.push(x);
    }
};

class Jmp_c : public Command {
    int GetArgsAmt() override { return 1; }
    void Do(runtime::State&& _) override {}
};

class Jeq_c : public Command {
    int GetArgsAmt() override { return 1; }
    void Do(runtime::State&& _) override {}
};

class Jne_c : public Command {
    int GetArgsAmt() override { return 1; }
    void Do(runtime::State&& _) override {}
};

class Ja_c : public Command {
    int GetArgsAmt() override { return 1; }
    void Do(runtime::State&& _) override {}
};

class Jae_c : public Command {
    int GetArgsAmt() override { return 1; }
    void Do(runtime::State&& _) override {}
};

class Jb_c : public Command {
    int GetArgsAmt() override { return 1; }
    void Do(runtime::State&& _) override {}
};

class Jbe_c : public Command {
    int GetArgsAmt() override { return 1; }
    void Do(runtime::State&& _) override {}
};

class Call_c : public Command {
    int GetArgsAmt() override { return 1; }
    void Do(runtime::State&& _) override {}
};

class Ret_c : public Command {
    int GetArgsAmt() override { return 0; }
    void Do(runtime::State&& _) override {}
};

}
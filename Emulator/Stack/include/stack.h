#include <stdexcept>

namespace myStack {
    template<class T>
    class stack {
        public:
            stack<T>() {}
            ~stack<T>() {
                free(s);
            }

            stack<T>(const stack<T>& o) : ind(o.ind), cap(o.cap) {
                s = new T[o.cap];
                memcpy(s, o.s, ind * sizeof(T));
            }

            stack<T>(stack<T>&& o) noexcept : ind(o.ind), cap(o.cap), s(o.s) {
                o.s = nullptr;
            }

            stack<T>& operator=(const stack<T>& o) {
                ind = o.ind;
                cap = o.cap;

                s = new T[o.cap];
                memcpy(s, o.s, ind * sizeof(T));

                return *this;
            }

            stack<T>& operator=(stack<T>&& o) noexcept {
                ind = o.ind;
                cap = o.cap;
                s = o.s;

                o.s = nullptr;

                return *this;
            }

            void push(T& x) {
                if (this->ind == this->cap) {
                    if (this->cap == 0) {
                        this->cap = 1;
                        this->s = new T[1];
                        this->s[this->ind++] = x;
                        return;
                    }

                    T* tmp = new T[this->cap * 2];
                    memcpy(tmp, this->s, ind * sizeof(T));
                    delete[] this->s;
                    this->s = tmp;

                    this->cap *= 2;
                }
                this->s[this->ind++] = x;
            }

            void push(T&& x) {
                if (this->ind == this->cap) {
                    if (this->cap == 0) {
                        this->cap = 1;
                        this->s = new T[1];
                        this->s[this->ind++] = x;
                        return;
                    }

                    T* tmp = new T[this->cap * 2];
                    memcpy(tmp, this->s, ind * sizeof(T));
                    delete[] this->s;
                    this->s = tmp;

                    this->cap *= 2;
                }
                this->s[this->ind++] = x;
            }

            T& pop() {
                if (ind <= 0 || s == nullptr) {
                    throw std::out_of_range("Try to pop from empty stack");
                }

                return s[--ind];
            }

            T& top() {
                if (s == nullptr || ind <= 0) {
                    throw std::out_of_range("Try to get top from empty stack");
                }
                return s[ind - 1];
            }
        private:
            int ind = 0;
            int cap = 0;
            T* s = nullptr; // unique_pointer?
    };
}

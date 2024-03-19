#include <stdexcept>
#include <type_traits>

namespace myStack {
    template<class T>
    class stack {
        public:
            stack<T>() {
                if (std::is_pointer<T>::value) {
                    throw std::invalid_argument("Argument mustn't be pointer");
                }
            }

            ~stack<T>() {
                delete[] arr;
            }

            stack<T>(const stack<T>& o) : ind(o.ind), cap(o.cap) {
                arr = new T[o.cap];
                memcpy(arr, o.arr, ind * sizeof(T));
            }

            stack<T>(stack<T>&& o) noexcept : ind(o.ind), cap(o.cap), arr(o.arr) {
                o.arr = nullptr;
            }

            stack<T>& operator=(const stack<T>& o) {
                ind = o.ind;
                cap = o.cap;

                arr = new T[o.cap];
                memcpy(arr, o.arr, ind * sizeof(T));

                return *this;
            }

            stack<T>& operator=(stack<T>&& o) noexcept {
                ind = o.ind;
                cap = o.cap;
                arr = o.arr;

                o.arr = nullptr;

                return *this;
            }

            void resize() {
                T* tmp = new T[cap * 2];
                memcpy(tmp, arr, ind * sizeof(T));
                delete[] arr;
                arr = tmp;
                cap *= 2;
            }

            int size() {
                return ind;
            }

            void push(T& x) {
                if (this->ind == this->cap) {
                    if (this->cap == 0) {
                        this->cap = 1;
                        this->arr = new T[1];
                        this->arr[this->ind++] = x;
                        return;
                    }

                    resize();
                }
                this->arr[this->ind++] = x;
            }

            void push(T&& x) {
                push(x);
            }

            T pop() {
                if (ind <= 0 || arr == nullptr) {
                    throw std::out_of_range("Try to pop from empty stack");
                }

                return arr[--ind];
            }

            T& top() {
                if (arr == nullptr || ind <= 0) {
                    throw std::out_of_range("Try to get top from empty stack");
                }
                return arr[ind - 1];
            }
        private:
            int ind = 0;
            int cap = 0;
            T* arr = nullptr;
    };
}

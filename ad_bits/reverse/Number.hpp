#ifndef NUMBER_H
#define NUMBER_H

#include "tape.hpp"

namespace reverse {

template<typename T>
class Number {
	private:
		T value;
		T gradient;
		std::vector<Tape<T>> tape;
	public:

    Number(T val = 0, T grad = 0) : value(val), gradient(grad) {}
	T GetGradient() const {
		return gradient;
	}
	T GetValue() const {
		return value;
	}

	Number& SetValue(T val) {
		this -> value = val;
		return *this;
	}

	Number& SetGradient(T grad) {
		this -> gradient = grad;
		return *this;
	}

	Number& AddGradient(T grad) {
		this -> gradient += grad;
		return *this;
	}

    Number& operator=(const Number n) {
        this -> value = n.GetValue();
        this -> gradient = n.GetGradient();
        this -> tape = n.tape;
        return *this;
    }

	    

    void print() {
        std::cout << value << " " << gradient << std::endl;
    }
};

};

#endif
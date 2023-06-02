#include "etr.hpp"
using namespace etr;

typedef VEC<double> sexp;

sexp f(sexp& a, sexp& b) {
	sexp c = a * b + sin(a);
	return c;
}

int main() {
	sexp a = colon(1, 3);
	sexp b = vector(3.14, 3);
	sexp c = f(a, b);
	sexp vec_derivs = derivs(c, list(a, b));
}
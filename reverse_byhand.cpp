#include <iostream>
#include <cmath>

double f(double x) {
	return x*x + sin(x*x);
}

int main() {
	// Step 1: Evaluate the function at a specific value of x. Let's choose x = 2.
	double x = 2;
	double res = f(x);
	std::cout << res << std::endl;

	// Step 2: Compute the gradient of f with respect to each intermediate variable in the computation graph, working backwards from the output.

	// a)
	// Compute the gradient of f with respect to the output value of sin(x^2):
	// df/d(sin(x^2)) = 1

	// b)
	// Compute the gradient of sin(x^2) with respect to the intermediate value of x^2:
	// d(sin(x^2))/d(x^2) = cos(x^2)

	// c)
	// Compute the gradient of x^2 with respect to the input value of x:
	// d(x^2)/d(x) = 2x


	// Step 3: Combine the gradients using the chain rule to obtain the gradient of f with respect to x:
	/*	
	df/dx = df/d(sin(x^2)) * d(sin(x^2))/d(x^2) * d(x^2)/d(x)
	df/dx = 1 * cos(x^2) * 2x
	df/dx = 2xcos(x^2)
	*/

	/*
	Step 4: Evaluate the gradient at the chosen value of x (x = 2):
	df/dx = 2xcos(x^2) = 2(2)cos(2^2) ≈ -2.914
	So the gradient of f(x) = x^2 + sin(x^2) with respect to x at x = 2 is approximately -2.914.
	*/
}
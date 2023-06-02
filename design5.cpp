#include <iostream>
#include <vector>
#include <functional>


class Foo{

public:
	int a;
	int b;
	int size;
	int* c;
	
	Foo(int a_, int b_, int size_) : a(a_), b(b_), size(size_), c(nullptr) {
		c = new int[size];
        for (int i = 0; i < size; i++) c[i] = 0;
	}

	Foo(const Foo& other) : a(other.a), b(other.b), size(other.size), c(nullptr) {
		c = new int[size];
        for (int i = 0; i < size; i++) c[i] = other.c[i];
	}

	~Foo() {
		delete[] c;
		c = nullptr;
	}

	friend std::ostream& operator<<(std::ostream& os, const Foo& f) {
		os << "a " << f.a << " b " << f.b << " size " << f.size << std::endl;
		for(int i = 0; i < f.size; i++) {
			os << f.c[i] << std::endl;
		}
		return os;
	}
};


void backprop(const std::vector<Foo*>& ptrs, int tape) {
	for(auto& i: ptrs) {
		std::cout << i << std::endl;
	}
	// backprop over tape 
	// within the tape are the VVPLUS classes etc. 
	// in VVPLUS the intermediate results and the reference of the involved variables
	// calc gradients
}


void back_prop_outer(int tape) {
	// hard coded std::vector<Foo*> f{sexp a, sexp b} // not working as a and b are unknown
	// call backrop
} 

void actual_fct() {

}


int main() {

	Foo foo1(1, 2, 3);
	Foo foo2(5, 6, 5);

	std::vector<Foo*> f{&foo1, &foo2};

	auto bound_f = std::bind(backprop, f, std::placeholders::_1);

	bound_f(1);
}



/*
I have an R package called ast2ast. This package translates R code into C++ code. On the C++ side it is an expression template library.
Thus, if i translate this function
f <- function(a, b) {
  c <- a + (b-a)^2
} 
then during calling the assignment operator and storing the results into c the following vector is passed to c
N3etr3VECIdNS_6VVPLUSIdNS_5STOREIdEENS_5VVEXPIdNS_7VVMINUSIdS3_S3_EEdEEEEEE.
VEC (
    VVPLUS (
        STORE(a),
        VVEXP (
            VVMINUS (
                STORE(b),
                STORE(a)
            )
        )
    )
)
Thus, the expression tree is already existing.
Furthermore i can store the results of the intermediate results of the different terms within classses such as VVPLUS.
Thus, the forward pass is almost implemented. My idea is to do the backward pass within the assignment method of the central VEC class.
Assuming the user want to calculate the derivatives of c with respect to a and b. 
Therefore i would write std::vector<STORE*> vars{a.data(), b.data()} and than add this to the VEC c. c->vars = &vars.
Because the tape is known this is the expression tree specified above.
In order to know for which variables have to be consiers the attribute var has to be used. What do you think about this approach?
ANd could you show me how the backpropagation for the specified tree above
*/


/*
Input: 
- `c`: expression tree representing the output value of the computation
- `vars`: vector of references to input variables used in the computation
- `tape`: vector of intermediate values and the input variables they depend on
- `dC`: vector of length `n` representing the gradient of `c` with respect to each input variable, initialized to 0

For each intermediate node in the tape, in reverse order:
  - If the node is a VEC, continue to next node
  - If the node is a STORE corresponding to an input variable `v`:
    - If `v` is not in `vars`, continue to next node
    - Otherwise, set the corresponding entry in `dC` to 1
  - If the node is a VVPLUS or VVMINUS:
    - Propagate gradients from the parent node to the children nodes
    - Update the corresponding entries in `dC` for the input variables involved in the operation
  - If the node is a VVEXP:
    - Compute the gradient of the operation using the chain rule: 
        `dC_parent * VVEXP(expr$arg) * d(VVMINUS(expr$arg))`
    - Propagate gradients from the parent node to the child node
    - Update the corresponding entries in `dC` for the input variables involved in the operation

This pseudo code iterates through the intermediate nodes in reverse order, starting from the output c, and
uses the chain rule to compute the gradients of the output with respect to the input variables.
The gradients are accumulated in the dC vector, which should contain the final gradient values
for each input variable after the reverse pass is completed.
*/


/*
a <- 2
b <- 3
c <- a + (b-a)^2

STORE(a)
STORE(b)
VVMINUS(b-a)
VVEXP((b-a)^2)
VVPLUS(a + (b-a)^2)
STORE(c)

We want to compute the gradient of c with respect to a and b. We initialize dC to c(0, 0):
dC <- c(0, 0)

Starting from the output c, we iterate through the intermediate nodes in reverse order:
The last node is a STORE corresponding to c. We skip it since it's not an input variable.
The next node is a VVPLUS.
We get references to the children nodes (left is the STORE corresponding to a, and right is a VVEXP node).
We propagate the gradient from the parent node (1, since c is the output)
to the children nodes (dC_left gets 1, and dC_right gets 2*(b-a)*1 (the factor of 2 comes from the chain rule).
Next, we move to the VVEXP node.
We get references to the children nodes (left is the VVMINUS node, and right is a constant node with the value 2).
We propagate the gradient from the parent node (dC_right) to the children nodes, using the chain rule.
dC_VVMINUS gets dC_right * 2 * (b-a)
dC_2 (the constant node) gets dC_right * (b-a)^2
Next, we move to the VVMINUS node.
We get references to the children nodes (left is the STORE corresponding to b, and right is the STORE corresponding to a).
We propagate the gradient from the parent node (dC_VVMINUS) to the children nodes, using the chain rule.
dC_b gets dC_VVMINUS * -1
dC_a gets dC_VVMINUS * 1

Finally, we return the gradients dC_a and dC_b, which represent the rate of change of c with respect to a and b, respectively.
In this case, dC_a = -6 and dC_b = 10.
*/





/*
Yes, you will need to define class traits for each of the node types in your expression tree, including STORE, VVMINUS, VVEXP, and VVPLUS.

Class traits provide a way to specialize behavior for specific types within a template class.
In your case, you will want to define a set of traits for each of your node types,
so that you can implement the reverse pass for each type in a generic way.

The traits for each node type should specify a few key pieces of information, such as:

- The input types of the node (i.e. what type of data it takes as input)
- The output type of the node (i.e. what type of data it returns)
- The number of inputs the node has
- A way to compute the output value of the node given its inputs
- A way to compute the gradient of the node with respect to its inputs, given the gradient of the parent node with respect to its output
- Once you have defined the traits for each node type,
  you can use them to implement a generic reverse pass algorithm that can
  be applied to any expression tree built from these node types.
*/


/*
traits for VVEXP

template<typename T, typename L, typename R>
struct traits<VVEXP<T, L, R>> {
  using inputs = type_list<L>;
  using output = T;
};


This tells the backpropagation algorithm that VVEXP takes in one input
(the left-hand side of the exponentiation) and outputs a scalar value.
With this information, the algorithm can compute the gradient of the output with respect to the inputs.

You will need to define similar traits classes for all the other node types in
your expression tree (e.g. STORE, VVMINUS, etc.) to use them in the backpropagation.

1. Using a traits struct for type dispatchin
template<typename T>
struct traits {
    static const bool is_matrix = false;
};

template<typename T>
struct traits<Eigen::Matrix<T, Eigen::Dynamic, Eigen::Dynamic>> {
    static const bool is_matrix = true;
};

You can then use this struct to dispatch to different implementations of a function based on the type of the template parameter. For example:
template<typename T>
void some_function(T x) {
    if (traits<T>::is_matrix) {
        // matrix implementation
    } else {
        // scalar implementation
    }
}

2. Specializing a function template based on the traits:
template<typename T, bool is_matrix = traits<T>::is_matrix>
void some_function(T x) {
    // scalar implementation
}

template<typename T>
void some_function(T x) {
    // matrix implementation
}


3. Using SFINAE (Substitution Failure Is Not An Error) to enable/disable function overloads based on the traits:
template<typename T, typename std::enable_if<!traits<T>::is_matrix>::type* = nullptr>
void some_function(T x) {
    // scalar implementation
}

template<typename T, typename std::enable_if<traits<T>::is_matrix>::type* = nullptr>
void some_function(T x) {
    // matrix implementation
}

*/

sexp f() {
	sexp a, sexp b;

	vector<ref> v(&a, &b);

	a = a+b;
	return a;
}
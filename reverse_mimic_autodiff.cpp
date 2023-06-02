#include <cmath>
#include <tuple>
#include <array>
#include <iostream>

template <std::size_t N, typename F>
constexpr void For(F&& f) {
    For<N - 1>(std::forward<F>(f));
    f(std::integral_constant<std::size_t, N - 1>{});
}



template <typename F>
constexpr void For(std::integral_constant<std::size_t, 0>, F&&) {}

/*
template <std::size_t N, typename F>
constexpr void For(F&& f) {
    For(std::integral_constant<std::size_t, N - 1>{}, std::forward<F>(f));
}

*/

template <typename T>
class ExprBase {
public:
    virtual T value() const = 0;
    virtual void propagate(T d) = 0;
    virtual void bind_value(T* p) {}
};



template<typename T>
class Variable {
public:
    T value() const { return m_value; }

    void bind_value(T* ptr) const {
        m_value_ptr = ptr;
    }

    void propagate(T derivative) {
        if (m_value_ptr) {
            *m_value_ptr += derivative * m_derivative;
        }

        for (const auto& input : m_inputs) {
            input.propagate(derivative * m_derivative);
        }
    }

    void set_derivative(T derivative) {
        m_derivative = derivative;
    }

    template<typename E>
    void set_expression(E&& expr) {
        m_expr = std::make_shared<std::decay_t<E>>(std::forward<E>(expr));
        m_inputs = m_expr->inputs();
    }

private:
    T m_value;
    T* m_value_ptr = nullptr;
    T m_derivative = 0;
    std::shared_ptr<const Expression<T>> m_expr;
    std::vector<Variable> m_inputs;

    friend class Expression<T>;

    explicit Variable(T value) : m_value(value) {}
};






template <typename T>
Variable<T> operator+(const Variable<T>& lhs, const Variable<T>& rhs) {
    Variable<T> result(lhs.value() + rhs.value());
    lhs.expr->propagate(1.0);
    rhs.expr->propagate(1.0);
    return result;
}

template <typename T>
Variable<T> operator*(const Variable<T>& lhs, const Variable<T>& rhs) {
    Variable<T> result(lhs.value() * rhs.value());
    lhs.expr->propagate(rhs.value());
    rhs.expr->propagate(lhs.value());
    return result;
}

template <typename T>
Variable<T> exp_own(const Variable<T>& lhs, const Variable<T>& rhs) {
    Variable<T> result(lhs.value() / rhs.value());
    lhs.expr->propagate(1.0 / rhs.value());
    rhs.expr->propagate(-1.0 * lhs.value() / (rhs.value() * rhs.value()));
    return result;
}

/*

template <typename T>
class Wrt {
public:
    Wrt(std::tuple<const Variable<T>&, const Variable<T>&, const Variable<T>&> args)
        : args_(args) {}

    std::tuple<const Variable<T>&, const Variable<T>&, const Variable<T>&> args_;
};

template <typename T>
auto wrt(const Variable<T>& x, const Variable<T>& y, const Variable<T>& z) {
    return Wrt<T>(std::make_tuple(x, y, z));
}

template<typename T, typename... Vars>
auto derivatives(const Variable<T>& y, const Wrt<Vars...>& wrt)
{
    constexpr auto N = sizeof...(Vars);
    std::array<T, N> values;
    values.fill(0.0);

    // Bind each variable to an element in the array.
    For<N>([&](auto i) constexpr {
        std::get<i>(wrt.args_).bind_value(&values.at(i));
    });

    // Propagate the derivative.
    y.expr->propagate(1.0);

    // Unbind each variable.
    For<N>([&](auto i) constexpr {
        std::get<i>(wrt.args_).bind_value(nullptr);
    });

    // Return the values.
    return values;
}

*/
int main() {
    
    Variable<double> x(1.0);
    Variable<double> y(2.0);
    Variable<double> z(3.0);

    Variable<double> u = (x + y + z) * exp_own(x * y * z);
    /*
    auto [ux, uy, uz] = derivatives(u, wrt(x, y, z));

    std::cout << "ux = " << ux << ", uy = " << uy << ", uz = " << uz << std::endl;

    */
}

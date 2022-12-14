// https://www.sandordargo.com/blog/2021/02/10/cpp-concepts-motivations

#include <boost/multiprecision/cpp_int.hpp>
#include <boost/rational.hpp>
#include <complex>
#include <concepts>
#include <iostream>
#include <iterator>
#include <ranges>

//
#include <vector>

template <typename T>
concept Number = std::integral<T> || std::floating_point<T>;

template <typename T>
requires Number<T> auto add(T a, T b) {
    return a + b;
}

// type mix

// template <typename T, typename U>
// requires Number<T> && Number<U>
//     auto add(T a, U b) {
//     return a+b;
// }

// trailing requires

// template <typename T, typename U>
// auto add(T a, U b) requires Number<T>&& Number<U> {
//     return a + b;
// }

// template <Number T, Number U>
// auto add(T a, U b) {
//     return a + b;
// }

auto add(Number auto a, Number auto b) {  // allow mixing
    return a + b;
}

template <typename T>
requires Number<T> class WrapperdNumber {
public:
    WrapperdNumber(T num) : m_num{num} {}

private:
    T m_num;
};

// !
// std::convertible_to

template <typename T>
concept Any = true;

template <typename T>
concept Integer = std::integral<T>;

template <typename T>
concept Float = std::floating_point<T>;

template <typename T>
concept Number2 = Integer<T> || Float<T>;

template <typename T>
concept Addable = requires(T a, T b) {
    a + b;
};

auto add1(Addable auto x, Addable auto y) { return x + y; }

template <typename T>
concept HasSquare = requires(T t) {
    //    t.square();
    //    t.sqrt();  // or? and?

    { t.square() } -> std::convertible_to<int>;
};

// void printSquare(HasSquare auto number ) {}

template <typename T>
concept HasPower = requires(T t, int exponent) {
    t.power(exponent);
};

template <typename Base, typename Exponent>
concept HasPower1 = std::integral<Exponent>&& requires(Base base, Exponent exponent) {
    base.power(exponent);
};

template <typename Exponent>  // exponent only
void printPower(HasPower1<Exponent> auto number, Exponent exponent) {}

// PII
class IntWithoutSquare {
public:
    IntWithoutSquare(int num) : m_num{num} {}
    int square() { return m_num * m_num; }

private:
    int m_num;
};

class IntWithSquare {
public:
    IntWithSquare(int num) : m_num(num) {}
    int square() { return m_num * m_num; }

private:
    int m_num;
};

template <typename T>
using Reference = T&;

template <typename T>
requires(!std::same_as<T, std::vector<int>>) struct Other;

// Type req
template <typename T>
concept TypeRequirement = requires {
    typename T::value_type;
    typename Other<T>;  // Other instantitaed with T
};

namespace {
void test() {
    add(1, 1.0f);

    WrapperdNumber wn{42};

    add1(true, true);
}

void printSquare(HasSquare auto number) {
    //    std::cout << number.square() << '\n';
}

int test1() {
    printSquare(
        IntWithoutSquare{4});  // error: use of function 'void printSquare(auto:11) [with auto:11 = IntWithoutSquare]'
                               // with unsatisfied constraints, the required expression 't.square()' is invalid
    printSquare(IntWithSquare{5});

    //    TypeRequirement auto myVec = std::vector<int>{1, 2, 3};

    return {};
}
}  // namespace

// nested

template <typename BusinessObjectWithEncodeableStuff>
requires requires(BusinessObjectWithEncodeableStuff bo) {
    bo.interfaceA();
    bo.interfaceB();
    { bo.interfaceC() } -> std::same_as<int>;
}
void encodeSomeStuff(BusinessObjectWithEncodeableStuff iBusinessObject) {
    //    doStuff();
    // ...
}

// Logical ops

// !a not opposite to a

namespace {
template <typename T, typename U>
    requires std::unsigned_integral<typename T::Blah> ||
    std::unsigned_integral<typename U::Blah> void foo(T bar, U baz) {
    // ...
}

class MyType {
public:
    using Blah = unsigned int;
    // ...
};

int test3() {
    MyType mt;
    foo(mt, 5);
    foo(5, mt);
    // error: no operand of the disjunction is satisfied
    //     foo(5, 3);
}
}  // namespace

//==================================================================================
namespace akrzemi {
// Concept archetypes
// Concepts in the form added in C++20 used to be called lite.

// https://akrzemi1.wordpress.com/2020/10/26/semantic-requirements-in-concepts/

template <typename T>
concept Addable = std::regular<T>&& std::totally_ordered<T>&& requires(T x, T y) {
    // syntactic // COMPILER CARE ABOUT IT
    { x += y } -> std::same_as<T&>;
    { x + y } -> std::convertible_to<T>;

    // semantic
    // O(n) - semantic
    // No way to express it
    // Num is brocken
    requires requires {  // USER HAVE TO CARE ABOUT IT
                         //        (x + y) == (y + x);
        (x + y) == (x += y);
    };
};

template <typename T>
T sum_impl(T a, T b) {
    assert(a >= b);
    //    a += b;
    //    return a;

    auto r = a + b;
    return r;
}

template <Addable T>
T sum(T a, T b) {
    if (b > a) {
        return sum_impl(b, a);
    } else {
        return sum_impl(a, b);
    }
}

// Architype

// "My observation here is that making sure that the archetype has really the minimum
// interface is a manual job, and it is difficult. But even if the archetype is
// not 100% minimum, it can still help us detect many bugs early in the development cycle."
class A {
public:
    // semi-regular
    A() = default;
    A(A&&) = default;
    A(const A&) = default;
    A& operator=(A&&) = default;
    A& operator=(const A&) = default;
    ~A() = default;

    void operator&() const = delete;  // ??
    friend void operator,(const A&, const A&) = delete;

    // regular
    friend bool operator==(A const&, A const&) = default;

    // Addable
    struct Rslt {
        operator A() { return {}; }

        // Extras
        Rslt(Rslt&&) = delete;
        void operator&() const = delete;
        friend void operator,(Rslt, Rslt) = delete;
        static Rslt make() { throw 0; }
    };
    A& operator+=(A const&) { return *this; }
    friend Rslt operator+(A const&, A const&) { return Rslt::make(); }

    // Totally ordered
    friend auto operator<=>(A const&, A const&) = default;
};

using AddableArchetype = A;

static_assert(std::semiregular<AddableArchetype>);
static_assert(std::regular<A>);
static_assert(Addable<AddableArchetype>);  // lack of Rslt convertability

inline void test_concept_usige(AddableArchetype a, AddableArchetype b) { sum(a, b); }

// verify if the type can be used with the library
static_assert(Addable<boost::rational<int>>);
// static_assert(Addable<boost::multiprecision::cpp_int>);  // doesn't work/not because of assert
// static_assert(Addable<std::complex<double>>);  // Ok, but

struct Num {
    int i;
    friend auto operator<=>(Num, Num) = default;
    Num& operator+=(Num const& n) {
        i += n.i;
        return *this;
    }
};

struct NumAddExpr {
    Num const& lhs;
    Num const& rhs;

    operator Num() { return Num{lhs} += rhs; }

    NumAddExpr(Num const& x, Num const& y) : lhs{x}, rhs{y} {}

    NumAddExpr(NumAddExpr&&) = delete;
};

NumAddExpr operator+(Num const& x, Num const& y) { return {x, y}; }

static_assert(Addable<Num>);

void test() {
    int i = 1, j = 2;
    std::cout << sum(i, j) << std::endl;

    double x = 2.25, y = 5.25;
    std::cout << sum(x, y) << std::endl;

    //    std::complex<double> z1{1, 0}, z2{0, 1};
    //    std::cout << sum(z1, z2);

    //    std::cout << sum(Num{1}, Num{2}).i;
    //    sum(Num{1}, Num{2});

    sum(std::string("a"), std::string("n"));
}

}  // namespace akrzemi

namespace sem {

// OOP like
template <typename T>
constexpr bool enable_addable = false;

template <std::integral T>
constexpr bool enable_addable<T> = true;

template <std::floating_point T>
constexpr bool enable_addable<T> = true;

template <typename T>
concept Addable = std::regular<T>&& requires(T x, T y) {
    { x += y } -> std::same_as<T&>;
    { x + y } -> std::convertible_to<T>;

    requires enable_addable<T>;
};

// users specialization
template <typename T>
constexpr bool enable_addable<boost::rational<T>> = true;

// Way 2
// Inversion
template <typename T>
constexpr bool disable_addable = false;

template <typename C, typename T, typename A>
constexpr bool disable_addable<std::basic_string<C, T, A>> = true;

template <typename T>
concept Addable2 = std::regular<T>&& requires(T x, T y) {
    { x += y } -> std::same_as<T&>;
    { x + y } -> std::convertible_to<T>;

    requires !disable_addable<T>;
};

// check ranges::view and ranges::range
}  // namespace sem

namespace groups {
template <typename T>
concept OrderedAddable = akrzemi::Addable<T>&& std::totally_ordered<T>&& requires(T x, T y, T z) {
    /* axiom */ (x < y) == (x + z < y + z);
};

using akrzemi::Addable;
template <Addable T>
bool is_commutative(const T& a, const T& b) {
    return (a + b) == (b + a);
}

template <OrderedAddable T>
void is_ordered_group(T a, T b, T c) {
    return (a < b) == (a + c < b + c);
}

// Generic
template <Addable T>
T sum(T a, T b) {
    assert(is_commutative(a, b));
    return a + b;
}

// specialized version of the algorithm:

template <typename T>
T sum_impl(T a, T b) {
    assert(a >= b);
    a += b;
    return a;
}

template <OrderedAddable T>
T sum(T a, T b) {
    assert(is_commutative(a, b));
    assert(is_ordered_group(a, b, b));
    assert(is_ordered_group(a, b, a));

    if (b > a)
        return sum_impl(b, a);
    else
        return sum_impl(a, b);
}

}  // namespace groups
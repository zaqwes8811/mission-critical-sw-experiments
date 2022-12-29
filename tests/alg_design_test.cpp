// https://www.youtube.com/watch?v=InMh3JxbiTs&ab_channel=CppCon

#include <gtest/gtest.h>

#include <algorithm>
#include <chrono>
#include <iterator>
#include <vector>

using namespace std::chrono_literals;

// 0. Understand task

// 1. Contraints

auto find_missing_element(const std::vector<unsigned>& v) {
    const auto it = std::adjacent_find(std::cbegin(v), std::cend(v),
                                       //(auto x, auto y)
                                       [](auto&& x, auto&& y) { return y - x > 1; });
    return *it + 1;
}

auto has_repeated(const auto& container) {
    //    return std::adjacent_find()
}

TEST(AlgDesignTest, Sorted) {
    find_missing_element({1, 8, 3});
    // Too simple
}

// 2. Back to past!
// Div. and con.
// Smallest number gap - bottom half

// not sorted?
// partition
// sortening contraint
unsigned find_missing_element(unsigned* first, unsigned* last) {
    // base case
    if (last - first == 1) {
        return *first + 1;
    }

    // recursive step
    int m = (last - first) / 2;
    if (*(first + m) == *first + m) {  // ???
        return find_missing_element(first + m, last);
    } else {
        return find_missing_element(first, first + m);
    }
}

unsigned find_missing_element(unsigned* first, unsigned* last, unsigned value) {
    // base case
    if (last == first) {
        return value;
    }

    // recursive step
    unsigned half = (last - first + 1) / 2;
    unsigned m = value + half;
    auto p = std::partition(first, last, [&](auto x) { return x < m; });
    if (*(first + m) == *first + m) {  // ???
        return find_missing_element(p, last, m);
    } else {
        return find_missing_element(first, p, value);
    }
}

// 3. Testings

// From recursion to iteration
// Tail rec.

// 1. Add accum
// 2. Base cond to loop
// 3. rep recursive with update
unsigned find_missing_element_iter(unsigned* first, unsigned* last, unsigned value) {
    while (last != first) {  // base to loop
        unsigned half = (last - first + 1) / 2;
        unsigned m = value + half;
        auto p = std::partition(first, last, [&](auto x) { return x < m; });

        if (p == first + half) {
            first = p;
            value = m;
        } else {
            last = p;
        }
    }
    return value;
}

// 4. Generalize and templatize
template <std::forward_iterator It, typename T = std::iter_value_t<It>>
T find_missing_element_iter(It first, It last, T value) {
    while (last != first) {  // base to loop
                             //        T half = (last - first + 1) / 2;
        T half = (std::distance(first, last) + 1) / 2;
        T m = value + half;
        auto p = std::partition(first, last, [&](auto x) { return x < m; });

        //        if (p == first + half) {
        if (p == std::next(first, half)) {
            first = p;
            value = m;
        } else {
            last = p;
        }
    }
    return value;
}

// Iterators? Concepts
// Any standart alg? partition
// wear <-> strong

// no ops
// next/distance/advice/prev
// Iter canteroy mistake!
// stable_part -> bidirect., same inplace_merge

// [!] Strength reduction

// Ops to consider carefully
// decr/additon
// replace it++ with ++it
// eaualti/orderinf req.
// halving/doubling

// Req on types!!!

template <std::forward_iterator It, typename T = std::iter_value_t<It>>
constexpr T find_missing_element_iter1(It first, It last, T value = {}) {
    while (last != first) {
        //        T half = (std::distance(first, last) + 1) / 2;
        const auto half = static_cast<T>(std::distance(first, last) + 1) / 2;
        const T m = value + half;
        const auto p = std::partition(first, last, [&](auto x) { return x < m; });

        if (p == std::next(first, half)) {
            first = p;
            value = m;
        } else {
            last = p;
        }
    }
    return value;
}

// 5. precond/postcond

// better name

// min_absent

template <std::forward_iterator It, typename T = std::iter_value_t<It>>
constexpr T min_absent(It first, It last, T value = {}) {
    while (last != first) {
        const auto half = static_cast<T>(std::distance(first, last) + 1) / 2;
        const T m = value + half;
        const auto p = std::partition(first, last, [&](auto x) { return x < m; });

        if (p == std::next(first, half)) {
            first = p;
            value = m;
        } else {
            last = p;
        }
    }
    return value;
}

// 6. TYPE REQUIR.

// chrone duration

template <std::forward_iterator It, typename T = std::iter_value_t<It>>
constexpr T min_absent_chrono(It first, It last, T value = {}) {
    using diff_t = decltype(value - value);
    while (last != first) {
        const auto half = (std::distance(first, last) + 1) / 2;
        const T m = value + static_cast<diff_t>(half);
        const auto p = std::partition(first, last, [&](auto x) { return x < m; });

        if (p == std::next(first, half)) {
            first = p;
            value = m;
        } else {
            last = p;
        }
    }
    return value;
}

// signed <-> T
// std::chrono time_point

// [!] Affine Space Types
//   timepoint + duration - affine space
//   ptrs + std::ptrDiff_t
TEST(AlgDesignTest, Chrono) {
    using T = std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds>;
    std::vector v = {T{0s}, T{1s}};
    min_absent_chrono(std::begin(v), std::end(v));
}

// 7. Documentation

// + Ranges
//   - laziness
//   - algoriths
//  Views[!]

// 8. Overloads
//   Range version, predicate

// 9. Another alg - nth_element

// Exptra
// Arguments ordering
// 1. ExecutionPolicy
// 2. first, last , n,
// ...

// Option 2

// Option 3

// Algorithmic principles
// 1. Usefull return - return usefull info - std::rotate
// 2. Separation types
// 3. Law on completeness - bad in some algs of stl
// 4. HARD. Low of interface refinement - hard to guess, need statistic

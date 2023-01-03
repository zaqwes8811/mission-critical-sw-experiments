// Based on:
//   C++20 Concepts: A Day in the Life - Saar Raz - CppCon 2019

#include <gtest/gtest.h>

#include <concepts>
#include <cstdio>
#include <memory>
#include <tuple>

template <typename T>
concept Node = std::is_object_v<T>;

template <typename T>
concept Tree = requires(T t) {
    // https://www.sandordargo.com/blog/2021/03/17/write-your-own-cpp-concepts-part-ii
    {t.root};  // -> Node;

    Node<decltype(t.root)>;
};

template <Node Root_, Tree... Children_>
struct tree {
    tree() = default;

    tree(std::convertible_to<Root_> auto&& root) : root{std::forward<decltype(root)>(root)} {
        //        static_assert(Node<decltype(root)>);
    }

    Root_ root;
    std::tuple<Children_...> children;

    template <std::size_t index_>
    requires(index_ < sizeof...(Children_)) Tree auto& child() {
        return std::get<index_>(children);
    }
};

static_assert(Tree<tree<int>>);
// static_assert(Tree<tree<int, tree<int>>>);

// using component_tree = tree<std::unique_ptr<compontne

//// https://stackoverflow.com/questions/31762958/check-if-class-is-a-template-specialization
// template <class T, template <class...> class Template>
// struct is_specialization : std::false_type {};
//
//// template <template <typename...> typename Template, typename... Args>
//// constexpr bool  is_specialization_of_v<Template, Template<Args...>>;
//
// template <typename T, template <typename...> class Z>
// struct is_specialization_of : std::false_type {};
//
// template <typename... Args, template <typename...> class Z>
// struct is_specialization_of<Z<Args...>, Z> : std::true_type {};
//
// template <typename T, template <typename...> class Z>
// inline constexpr bool is_specialization_of_v = is_specialization_of<T, Z>::value;

// struct start {};
// struct tick {};
// struct tock {};

// template <Tree Tree_, typename TreeLocation_>
// struct context {
//     Tree_& tree;
//     TreeLocation_ location;
// };
//
// template <typename T>
// concept Context = is_specialization_of_v<T, context>;
//
// struct tick_tock {
//     void handle(const start& message, Context auto context) {
//         puts("tick: ");
//         context.sendDown(tick{});
//     }
//
//     void handle(const tock& message, Context auto context) { puts("tock: "); }
// };
// struct responder {
//     void handle(const tick& message, Context auto context) {
//         puts("tock: ");
//         context.sendUp(tock{});
//     }
// };

void test() {
    //    tree<int, tree<int>> t;
    //    //    tree<int, tree<int>> t2(t);
    //    return t.child<0>().root;
}

// https://www.youtube.com/watch?v=tF-Nz4aRWAM&t=967s&ab_channel=CppCon

#include <future>
#include <thread>

std::future<int> async_algo() {
    std::promise<int> p;
    auto f = p.get_future();

    std::thread t{[p = std::move(p)]() mutable {
        int answer = 0;
        p.set_value(answer);
    }};
    t.detach();
    return f;
}

template <typename Cont>
std::future<int> async_algo1(Cont c) {
    std::promise<int> p;
    auto f = p.get_future();

    std::thread t{[p = std::move(p), c]() mutable {
        int answer = 0;
        p.set_value(c(answer));
    }};
    t.detach();
    return f;
}

auto async_algo2() {
    return [](auto p) {  // Promise like interface
        std::thread t{[p = std::move(p)]() mutable {
            int answer = 0;
            p.set_value(answer);
        }};
        t.detach();
    };
}

TEST(CompileTimeGraphTest, Best) {
    //    auto f = async_algo();
    //    f.than()
    //    f.wait();

    //    auto f = async_algo1([](int i) {
    //        return i;
    //    });
    //
    //    f.get();

    // separate compose and launch

    auto f = async_algo2();
    //    auto f2 = than(f, [](int i) {
    //
    //    });
}

// V2
// https://www.youtube.com/watch?v=xiaqNvqRB2E&ab_channel=CppEurope

struct immovable {
    immovable() = default;
    immovable(immovable&&) = delete;
};

template <class R, class T>
struct just_operation : immovable {
    R rec;
    T value;

    friend void start(just_operation& self) { set_value(self.rec, self.value); }
};

template <class T>
struct just_sender {
    T value;

    // every sender have to impl it
    template <class R>
    friend just_operation<R, T> connect(just_sender self, R rec) {
        return {{}, rec, self.value};
    }
};

template <class T>
just_sender<T> just(T t) {
    return {t};
}

struct cout_receiver {
    friend void set_value(cout_receiver& self, auto val) { std::cout << "result:" << val << "\n"; }
};

TEST(CompileTimeGraphTest, Main) {
    auto s = just(42);
    auto op = connect(s, cout_receiver{});
    start(op);
}
// Based on:
//   C++20 Concepts: A Day in the Life - Saar Raz - CppCon 2019

#include <concepts>
#include <cstdio>
#include <memory>
#include <tuple>

// using component_tree = tree<std::unique_ptr<compontne

//// https://stackoverflow.com/questions/31762958/check-if-class-is-a-template-specialization
//template <class T, template <class...> class Template>
//struct is_specialization : std::false_type {};
//
//// template <template <typename...> typename Template, typename... Args>
//// constexpr bool  is_specialization_of_v<Template, Template<Args...>>;
//
//template <typename T, template <typename...> class Z>
//struct is_specialization_of : std::false_type {};
//
//template <typename... Args, template <typename...> class Z>
//struct is_specialization_of<Z<Args...>, Z> : std::true_type {};
//
//template <typename T, template <typename...> class Z>
//inline constexpr bool is_specialization_of_v = is_specialization_of<T, Z>::value;

template <typename T>
concept Node = std::is_object_v<T>;

template <typename T>
concept Tree = requires(T t) {
    { t.root } -> Node;
};

template <Node Root_, Tree... Children_>
struct tree {
    tree() = default;

    tree(std::convertible_to<Root_> auto&& root) : root{std::forward<decltype(root)>(root)} {}

    Root_ root;
    std::tuple<Children_...> children;

    template <std::size_t index_>
    requires(index_ < sizeof...(Children_)) Tree auto& child() {
        return std::get<index_>(children);
    }
};

static_assert(Tree<tree<int>>);
//static_assert(Tree<tree<int, tree<int>>>);


//struct start {};
//struct tick {};
//struct tock {};

//template <Tree Tree_, typename TreeLocation_>
//struct context {
//    Tree_& tree;
//    TreeLocation_ location;
//};
//
//template <typename T>
//concept Context = is_specialization_of_v<T, context>;
//
//struct tick_tock {
//    void handle(const start& message, Context auto context) {
//        puts("tick: ");
//        context.sendDown(tick{});
//    }
//
//    void handle(const tock& message, Context auto context) { puts("tock: "); }
//};
//struct responder {
//    void handle(const tick& message, Context auto context) {
//        puts("tock: ");
//        context.sendUp(tock{});
//    }
//};

int test() {
//    tree<int, tree<int>> t;
//    //    tree<int, tree<int>> t2(t);
//    return t.child<0>().root;
}
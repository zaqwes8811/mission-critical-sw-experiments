//
// Created by zaqwes on 20.09.2022.
//

#include <gtest/gtest.h>

#include <functional>
#include <memory>

// Optional/Function/Any/Expected

// C++ Weekly - Ep 333 - A Simplified std::function Implementation

template <typename T>
class function;

template <typename Ret, typename... Param>
class function<Ret(Param...)> {
public:
    //    function(Ret (*f)(Param...)): callable{std::make_unique<callable_impl<Ret(*)(Param...)>>(f)} {
    //
    //    }

    // TODO() Heap elision
    // Big home on compiler
    template <typename FunctionObject>
    function(FunctionObject fo)
        : callable{std::make_unique<callable_impl<FunctionObject>>(std::move(fo))} {  // Forward here?
    }

    Ret operator()(Param... param) {
        //        return callable->call(std::forward<Param...>(param...));  // Unpack
        return callable->call(param...);
    }

    // copy ctory

    // add a plathoer of other ctors?

private:
    // Storage any size
    struct callable_interface {
        virtual Ret call(Param...) = 0;
        virtual ~callable_interface() = default;

        // virtual clone()
    };

    template <typename Callable>
    struct callable_impl : callable_interface {
        explicit callable_impl(Callable callable_) : callable(std::move(callable_)) {}

        // std::invoke
        Ret call(Param... param) override { return callable(param...); }
        Callable callable;
        // finish rule of 5
    };

    std::unique_ptr<callable_interface> callable;
};

int f(int x, int y) { return x + y; }

TEST(FunctionTest, Original) {
    function<int(int, int)> func{[](int x, int y) { return 0; }};
    int ret = func(1, 2);
    (void)ret;
}
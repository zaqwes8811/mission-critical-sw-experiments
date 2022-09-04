//
// Created by zaqwes on 06.02.2022.
//

#include <iceoryx_hoofs/cxx/expected.hpp>
#include <iceoryx_hoofs/design_pattern/creation.hpp>

iox::cxx::expected<int, int> v = iox::cxx::expected<int, int>::create_error<int>(1);

enum class MyResourceAbstractionError { ResourceNotAvailable, BlaBlubError };

class MyResourceAbstraction : public DesignPattern::Creation<MyResourceAbstraction, MyResourceAbstractionError> {
public:
    MyResourceAbstraction(int, int) {}
};

void f() {
    auto r = MyResourceAbstraction::create(0, 9);
    //    r.value().
}
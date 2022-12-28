
#include <absl/status/statusor.h>
#include <gtest/gtest.h>

#include <gsl/gsl>

class Quantity {
public:
    explicit Quantity(unsigned int quantity) : quantity_(quantity) {}
    template <typename T>
    static absl::StatusOr<Quantity> from(T quantity) {
        try {
            // It'll throw if something bad happened
            return Quantity(gsl::narrow<decltype(quantity_)>(quantity));
        } catch (std::exception& e) {
            return absl::InvalidArgumentError(e.what());
        }
    }

    [[nodiscard]] auto get() && { return quantity_; }

private:
    unsigned int quantity_{};
};

TEST(GslExploreTest, Narrow) { auto q = Quantity::from(123L); }

TEST(GslExploreTest, MoveFrom) {
    auto q = Quantity::from(123L);
    auto v = std::move(*q).get();

    //    auto v1 = [] {
    //        auto q = Quantity(123L);
    //        return q.get();  // Doesn't work really
    //    } ();
    //    auto v =  q->get();
}
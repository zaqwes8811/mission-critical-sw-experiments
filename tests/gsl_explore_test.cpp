
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

private:
    unsigned int quantity_{};
};

TEST(GslExploreTest, Narrow) { auto q = Quantity::from(123L); }
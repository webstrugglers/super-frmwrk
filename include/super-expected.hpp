#ifndef SUPER_EXPECTED
#define SUPER_EXPECTED

#include <variant>

namespace super {

template <typename T, typename E>
class Expected {
private:
    std::variant<T, E> storage;

public:
    constexpr Expected() = default;

    constexpr Expected(T val) : storage(val) {}
    constexpr Expected(E err) : storage(err) {}

    [[nodiscard]] constexpr bool has_value() const noexcept {
        return std::holds_alternative<T>(storage);
    }

    [[nodiscard]] constexpr bool has_error() const noexcept {
        return std::holds_alternative<E>(storage);
    }

    [[nodiscard]] constexpr T take_value() noexcept {
        return std::move(std::get<T>(storage));
    }

    [[nodiscard]] constexpr E take_err() noexcept {
        return std::move(std::get<E>(storage));
    }
};

}  // namespace super

#endif  // !SUPER_EXPECTED

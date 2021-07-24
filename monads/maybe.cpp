#include <optional>
#include <string>
#include <type_traits>

namespace {

template <typename T, typename = void>
struct transform_complex {
    using Type = T;
};

template <typename T>
struct transform_complex<T, std::enable_if_t<std::is_integral_v<std::decay_t<T>>>> {
    using Type = std::decay_t<T>;
};

template <typename T>
using transform_complex_t = typename transform_complex<T>::Type;

static_assert(std::is_same_v<int, transform_complex_t<int>>);
static_assert(std::is_same_v<int, transform_complex_t<const int>>);
static_assert(std::is_same_v<int, transform_complex_t<int&&>>);
static_assert(std::is_same_v<int, transform_complex_t<int&>>);
static_assert(std::is_same_v<int, transform_complex_t<const int&>>);

static_assert(std::is_same_v<std::string, transform_complex_t<std::string>>);
static_assert(std::is_same_v<const std::string, transform_complex_t<const std::string>>);
static_assert(std::is_same_v<std::string&&, transform_complex_t<std::string&&>>);
static_assert(std::is_same_v<std::string&, transform_complex_t<std::string&>>);
static_assert(std::is_same_v<const std::string&, transform_complex_t<const std::string&>>);

template <typename T>
using Maybe = std::optional<T>;

template <typename T>
constexpr Maybe<T> Just(T&& t) {
    return { std::move(t) };
}

template <typename T>
constexpr Maybe<T> Nothing() {
    return std::nullopt;
}

template <typename T>
static constexpr auto is_maybe_v = false;

template <typename T>
static constexpr auto is_maybe_v<Maybe<T>> = true;

static_assert(!is_maybe_v<int>);
static_assert(is_maybe_v<std::optional<int>>);
static_assert(is_maybe_v<Maybe<int>>);

template <typename T, typename Func, typename Ret = std::invoke_result_t<Func, T>>
constexpr Ret MBind(Maybe<T> ma, Func f) {
    static_assert(is_maybe_v<Ret>);
    if(!ma) {
        return std::nullopt;
    }
    return f(std::move(*ma));
}

template <typename T, typename Func>
constexpr auto operator|(Maybe<T> ma, Func f) {
    return MBind(std::move(ma), std::move(f));
}

}

static constexpr Maybe<int> LessZero(const int val) {
    if(val < 0) {
        return std::nullopt;
    }   
    return 100;
}
static constexpr Maybe<std::string_view> AboveZeroHelloWorld(const int val) {
    if(val <= 0) {
        return std::nullopt;
    }   
    return "hello world";
}
static constexpr Maybe<int> StringViewLength(const std::string_view val) {
    return val.length();
}

static_assert(LessZero(-1) == Nothing<int>());
static_assert(LessZero(1) == Just(100));
static_assert(MBind(Just(-1), LessZero) == Nothing<int>());
static_assert((Just(1) | LessZero) == Just(100));
static_assert((Just(-1) | LessZero) == Nothing<int>());

static_assert((Just(1) | LessZero | AboveZeroHelloWorld) == Just<std::string_view>("hello world"));
static_assert((Just(-1) | LessZero | AboveZeroHelloWorld) == Nothing<std::string_view>());

static_assert((Just(1) | LessZero | AboveZeroHelloWorld | StringViewLength) == Just(std::string_view{"hello world"}.length()));
static_assert((Just(-1) | LessZero | AboveZeroHelloWorld | StringViewLength) == Nothing<std::size_t>());


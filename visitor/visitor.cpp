#include <variant>

namespace {

template <typename... T>
static constexpr auto is_variant_v = false;

template <typename... T>
static constexpr auto is_variant_v<std::variant<T...>> = true;

static_assert(!is_variant_v<bool, int>);
static_assert(is_variant_v<std::variant<bool, int>>);

template <typename... Callables>
struct overload : Callables... {
	using Callables::operator()...;
};

template <typename... Callables>
overload(Callables...) -> overload<Callables...>;

}

template <typename Variant, typename... Visitors>
static constexpr auto VisitHelper(Variant&& variant, Visitors&&... visitors) {
	return std::visit(overload{ std::forward<Visitors>(visitors)... }, std::forward<Variant>(variant));
}

static constexpr auto HandleBool = [](const bool val) noexcept {
	return val ? 111 : 222;
};

static constexpr auto HandleInt = [](const int val) noexcept {
	return val < 100 ? 1 : 999;
};

using TestVariant = std::variant<int, bool>;
static_assert(VisitHelper(TestVariant{1}, HandleBool, HandleInt) == 1);
static_assert(VisitHelper(TestVariant{101}, HandleBool, HandleInt) == 999);
static_assert(VisitHelper(TestVariant{false}, HandleBool, HandleInt) == 222);
static_assert(VisitHelper(TestVariant{true}, HandleBool, HandleInt) == 111);

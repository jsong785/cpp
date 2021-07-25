#define CATCH_CONFIG_ENABLE_BENCHMARKING
#include "catch.hpp"
#include <string>
#include <type_traits>

namespace {

template <typename F, typename = void> class FuncWrap {
public:
  constexpr explicit FuncWrap(F &&f) : m_wrap{std::forward<F>(f)} {}

  template <typename Val> constexpr auto operator()(Val &&val) {
    return m_wrap(std::forward<Val>(val));
  }

private:
  F &&m_wrap{};
};

template <typename F>
class FuncWrap<F, std::enable_if_t<std::is_pointer_v<std::decay_t<F>>>> {
public:
  using FuncPtr = std::decay_t<F>;
  constexpr explicit FuncWrap(const FuncPtr f) : m_wrap{f} {}

  template <typename Val> constexpr auto operator()(Val &&val) {
    return m_wrap(std::forward<Val>(val));
  }

private:
  const FuncPtr m_wrap{};
};

template <typename Val, typename Func>
constexpr auto operator|(Val &&val, FuncWrap<Func> &&b) {
  return b(std::forward<Val>(val));
}

template <typename Val, typename... Func>
constexpr auto Compose(Val &&val, Func &&...func) {
  return (std::forward<Val>(val) | ... |
          FuncWrap<Func>{std::forward<Func>(func)});
}

} // namespace

static constexpr auto ReturnInt(const bool val) noexcept {
  return val ? 100 : 1;
}

static constexpr auto DoubleInt(const int val) noexcept { return val * 2; }

static_assert(Compose(true, ReturnInt, DoubleInt) == 200);
static_assert(Compose(true, ReturnInt, DoubleInt, DoubleInt) == 400);
static_assert(Compose(
                  true, [](const auto val) { return val ? 1000 : 0; },
                  DoubleInt) == 2000);

TEST_CASE("Benchmark compose") {

  const auto IntToString = [](const int val) { return std::to_string(val); };

  const auto DoubleString = [](std::string val) { return val + val; };

  const auto StringToInt = [](std::string val) {
    return std::stoi(std::move(val));
  };
  REQUIRE(StringToInt(DoubleString(IntToString(20))) == 2020);
  REQUIRE(Compose(20, IntToString, DoubleString, StringToInt) == 2020);
  BENCHMARK("Regular") { return StringToInt(DoubleString(IntToString(20))); };

  BENCHMARK("Compose") {
    return Compose(20, IntToString, DoubleString, StringToInt);
  };
}

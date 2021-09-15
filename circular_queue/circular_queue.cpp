#include <array>
#include <optional>

namespace {

template <typename T, std::size_t N> class CircularQueue {
public:
  constexpr bool push(const T item) {
    if (m_len == N) {
      return false;
    }
    m_array[(m_start + m_len++) % N] = item;
    return true;
  }

  constexpr std::optional<T> pop() {
    if (m_len == 0) {
      return {};
    }
    const auto index = (m_start++ % N);
    m_start %= N;
    --m_len;
    return m_array[index];
  }

  constexpr auto size() const noexcept { return m_len; }

private:
  std::size_t m_start{0};
  std::size_t m_len{0};
  std::array<T, N> m_array;
};

} // namespace

#define EXPECT_SIZE(X)                                                         \
  if (!size(X)) {                                                              \
    return false;                                                              \
  }
#define EXPECT_PUSH(I, E)                                                      \
  if (!push(I, E)) {                                                           \
    return false;                                                              \
  }
#define EXPECT_POP_NULL()                                                      \
  if (!pop({})) {                                                              \
    return false;                                                              \
  }
#define EXPECT_POP(E)                                                          \
  if (!pop({E})) {                                                             \
    return false;                                                              \
  }

static constexpr bool test() {

  CircularQueue<int, 3> queue{};
  const auto size = [&queue](const std::size_t expected) constexpr {
    return queue.size() == expected;
  };
  const auto push = [&queue](const int item, const bool expected) constexpr {
    return queue.push(item) == expected;
  };
  const auto pop = [&queue](const std::optional<int> expected) constexpr {
    return queue.pop() == expected;
  };
  EXPECT_SIZE(0);
  EXPECT_PUSH(0, true);
  EXPECT_SIZE(1);
  EXPECT_PUSH(1, true);
  EXPECT_SIZE(2);
  EXPECT_PUSH(2, true);
  EXPECT_SIZE(3);

  EXPECT_POP(0);
  EXPECT_SIZE(2);
  EXPECT_POP(1);
  EXPECT_SIZE(1);

  EXPECT_PUSH(0, true);
  EXPECT_SIZE(2);
  EXPECT_PUSH(0, true);
  EXPECT_SIZE(3);
  EXPECT_PUSH(0, false);
  EXPECT_SIZE(3);

  EXPECT_POP(2);
  EXPECT_SIZE(2);
  EXPECT_POP(0);
  EXPECT_SIZE(1);
  EXPECT_POP(0);
  EXPECT_SIZE(0);
  EXPECT_POP_NULL();
  EXPECT_SIZE(0);

  return true;
}
static_assert(test());

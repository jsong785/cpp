#include "catch.hpp"
#include <stack>
#include <string>

namespace internal {

static std::pair<double, std::size_t> parseDigit(const std::string_view equation) {
    double res = 0;
    double dec = 0;
    std::size_t i = 0;

    std::size_t decimalPlace = std::string_view::npos;
    for(; i < equation.size(); ++i) {
        const auto d = equation[i];
        if(d == '.') {
          decimalPlace = i;
          continue;
        }
        else if(!std::isdigit(d)) {
          break;
        }

        if(decimalPlace == std::string_view::npos) {
          res *= 10;
          res += (static_cast<double>(d - '0') * std::pow(10.0, static_cast<double>(i)));
        }
        else {
          dec += (static_cast<double>(d - '0') * std::pow(10.0, static_cast<double>(decimalPlace) - i));
        }
    }
    return { res + dec, i };
}

enum class Operation {
  none, add, substract, divide, multiply
};

static std::pair<Operation, std::size_t> parseOperation(const std::string_view equation) {
    const auto eq = equation[0];
    switch(eq) {
      case '+': return { Operation::add, 1 };
      case '-': return { Operation::substract, 1 };
      case '*': return { Operation::multiply, 1 };
    }
    return { Operation::divide, 1 };
}

}

static std::pair<double, std::size_t> calculateParen(const std::string_view equation);

double calculate(const std::string_view equation) {

  std::stack<double> values;

  auto lastOperation = internal::Operation::none;
  for(auto i = std::size_t{0}; i < equation.size();) {
      const auto c = equation[i];
      if(std::isspace(c)) {
        ++i;
        continue;
      }

      if(std::isdigit(c)) {
          const auto [num, increment] = internal::parseDigit({equation.cbegin() + i});
          i += increment;

          if(lastOperation == internal::Operation::add) {
              values.emplace(num);
          }
          else if(lastOperation == internal::Operation::substract) {
              values.emplace(-num);
          }
          else if(lastOperation == internal::Operation::multiply) {
              const auto n = values.top();
              values.pop();
              values.emplace(n * num);
          }
          else if(lastOperation == internal::Operation::divide) {
              const auto n = values.top();
              values.pop();
              values.emplace(n / num);
          }
          else {
              values.emplace(num);
          }
      }
      else if (c == '('){
          const auto [num, increment] = calculateParen({equation.cbegin() + i});
          i += increment;
          if(lastOperation == internal::Operation::add) {
              values.emplace(num);
          }
          else if(lastOperation == internal::Operation::substract) {
              values.emplace(-num);
          }
          else if(lastOperation == internal::Operation::multiply) {
              const auto n = values.top();
              values.pop();
              values.emplace(n * num);
          }
          else if(lastOperation == internal::Operation::divide) {
              const auto n = values.top();
              values.pop();
              values.emplace(n / num);
          }
          else {
              values.emplace(num);
          }
      }
      else {
          const auto [op, increment] = internal::parseOperation({equation.cbegin() + i});
          lastOperation = op;
          i += increment;
      }
  }

  double res = 0;
  while(!values.empty()) {
    res += values.top();
    values.pop();
  }
  return res;
}

static std::pair<double, std::size_t> calculateParen(const std::string_view equation) {
  std::size_t start = 0;
  std::size_t end = 0;

  std::size_t openCount = 0;
  for(std::size_t i = 0; i < equation.size(); ++i) {
      if(equation[i] == '(') {
          ++openCount;
      }
      else if(equation[i] == ')') {
        --openCount;
        if(openCount == 0) {
          end = i;
        }
      }
  }

  const auto eqStart = equation.cbegin() + start + 1;
  const auto len =  static_cast<std::size_t>(std::distance(eqStart, equation.cbegin() + end));
  return {  calculate({eqStart, len}),end-start+1 };
}

TEST_CASE("calculator", "[calculator]") {

  CHECK(calculate("1+1") == 2);
  CHECK(calculate("1 + 1") == 2);
  CHECK(calculate("1 + 1  ") == 2);
  CHECK(calculate("1.0 + 1  ") == 2);
  CHECK(calculate("1.1 + 1  ") == 2.1);

  CHECK(calculate("1.2 + 1 * 9.9 ") == 11.1);
  CHECK(calculate("10 * 1.2 + 1 * 9.9 ") == 21.9);
  CHECK(calculate("10 * 1.2 + 1 * 9.9 /3.3") == 15);

  CHECK(calculate("(10 * 1.2 + 1 * 9.9)") == 21.9);
  CHECK(calculate("1 + (10 * 1.2 + 1 * 9.9)") == 22.9);
  CHECK(calculate("1 + (10 * 1.2 + 1 * 9.9) + 1") == 23.9);
  CHECK(calculate("1 + (10 * 1.2 + 1 * 9.9) + (4 * 10)") == 62.9);
  CHECK(calculate("1 + (10 * 1.2 + 1 * 9.9) + (4 * (10/2))") == 42.9);

  CHECK(calculate("50 + ((10 * 1.2 + 1 * 9.9) + (4 * (10/2)))") == 91.9);
  CHECK(calculate("1.1 - 2.2 * (1+2+3+ 4)") == -20.9);
}


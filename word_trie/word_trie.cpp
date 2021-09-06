#include "catch.hpp"
#include <stack>
#include <string_view>
#include <array>
#include <vector>
#include  <unordered_map>

class Dictionary {
    public:
        void add(const std::string_view word) {
            if(word.empty()) {
                return;
            }

            Letter* letter = &m_root;
            for(const auto c : word) {
                letter = &letter->letters[c];
            }
            letter->end = true;
            ++m_count;
        }

        void remove(const std::string_view word) {
            if(word.empty()) {
                return;
            }
            auto last = GetLast(word);
            if(last == nullptr) {
                return;
            }
            if(last->end) {
                last->end = false; // remove tree later if only one in use
                --m_count;
            }
        }

        std::vector<std::string> lookup(const std::string_view prefix) const {
            if(prefix.empty()) {
                return {};
            }

            auto last = GetLast(prefix);
            if(last == nullptr) {
                return {};
            }

            std::vector<std::string> matches;

            std::stack<std::pair<const Letter*, std::string>> stack;
            stack.emplace(last, prefix);
            while(!stack.empty()) {
                const auto entry = stack.top();
                stack.pop();

                if(entry.first->end) {
                    matches.push_back(entry.second);
                }

                for(const auto& l : entry.first->letters) {
                    stack.emplace(&l.second, entry.second + l.first);
                }
            }

            return matches;
        }
    
        std::size_t count() const {
            return m_count;
        }

    private:
        struct Letter {
            bool end = false;
            std::unordered_map<char, Letter> letters;
        };
        const Letter* GetLast(const std::string_view words) const {
            auto letter = &m_root;
            for(const auto c : words) {
                const auto found = letter->letters.find(c);
                if(found == letter->letters.end()) {
                    return {};
                }
                letter = &found->second;
            }
            return letter;
        }

        Letter* GetLast(const std::string_view words) {
            return const_cast<Letter*>(const_cast<const Dictionary&>(*this).GetLast(words));
        }

        std::size_t m_count = 0;
        Letter m_root;
};

TEST_CASE("add") {
    Dictionary dic;
    dic.add("hello");
    dic.add("ab");
    dic.add("abc");
    REQUIRE(dic.count() == 3);

    CHECK(dic.lookup("ab") == std::vector<std::string>{ "ab", "abc" });
    CHECK(dic.lookup("abc") == std::vector<std::string>{ "abc" });
    dic.remove("ab");
    REQUIRE(dic.count() == 2);
    CHECK(dic.lookup("ab") == std::vector<std::string>{ "abc" });
    CHECK(dic.lookup("h") == std::vector<std::string>{ "hello" });

    dic.remove("zebra");
    REQUIRE(dic.count() == 2);

    dic.remove("ab");
    REQUIRE(dic.count() == 2);

    dic.remove("abc");
    REQUIRE(dic.count() == 1);

    dic.remove("hello");
    REQUIRE(dic.count() == 0);

    dic.add("日本語");
    REQUIRE(dic.count() == 1);

    dic.add("日本人");
    REQUIRE(dic.count() == 2);

    dic.remove("日本");
    REQUIRE(dic.count() == 2);

    CHECK(dic.lookup("日本") == std::vector<std::string>{ "日本語", "日本人" });

    dic.remove("日本人");
    CHECK(dic.lookup("日本") == std::vector<std::string>{ "日本語" });
    CHECK(dic.count() == 1);
}


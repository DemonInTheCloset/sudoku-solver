#include <array>
#include <cstdint>
#include <string>
#include <variant>

namespace jsm {
  namespace detail {
    template <typename T>
    constexpr auto count_set_bits(T val) noexcept -> int {
      int count = 0;

      while (val) {
        val &= (val - 1);
        ++count;
      }

      return count;
    }

    constexpr auto pow(int b, int e) noexcept -> int {
      if (b < 0 || e < 0) return -1;
      if (b == 0) return 0;
      if (e == 0 || b == 1) return 1;

      int y = 1;

      while (e > 1) {
        if (e & 1) {
          y *= b;
        }
        b *= b;
        e >>= 1;
      }

      return b * y;
    }
  } // namespace detail
} // namespace jsm

namespace jsm {
  template <typename T, int N>
  requires(sizeof(T) >= N) class BitSet {
    T set = 0;

    [[nodiscard]] static constexpr auto mask_from(int val) noexcept -> T { return 1 << val; }

    public:
    [[nodiscard]] static constexpr auto full_set() noexcept -> BitSet {
      BitSet set{};
      set.set = ~(1 << N);

      return set;
    }

    // Capacity
    [[nodiscard]] constexpr auto empty() const noexcept -> bool { return set; }

    [[nodiscard]] constexpr auto size() const noexcept -> int {
      return detail::count_set_bits(set);
    }

    [[nodiscard]] constexpr auto max_size() const noexcept -> int { return N; }

    // Modifiers
    constexpr void clear() noexcept { set = 0; }

    constexpr auto insert(int val) noexcept -> bool {
      T mask = mask_from(val);
      bool ret_val = set & mask;

      set |= mask;

      return ret_val;
    }

    constexpr void erase(int val) noexcept { set &= ~mask_from(val); }

    // Lookup
    [[nodiscard]] constexpr auto contains(int val) const noexcept -> bool {
      return set & mask_from(val);
    }
  };

  template <int N>
  using BitSet32 = BitSet<std::uint32_t, N>;

  template <int N>
  using BitSet64 = BitSet<std::uint64_t, N>;

  template <int N>
  requires(N > 1 && N <= 8) class Sudoku {
    constexpr static int NUM_ELEMENTS = N * N;
    constexpr static int SIDE_LENGTH = NUM_ELEMENTS;
    constexpr static std::size_t SIZE = SIDE_LENGTH * SIDE_LENGTH;

    using set_t = BitSet64<NUM_ELEMENTS>;

    using element_t = std::variant<int, set_t>;

    std::array<element_t, SIZE> m_elements;

    [[nodiscard]] constexpr auto full_set() const noexcept -> set_t { return set_t::full_set(); }

    public:
    constexpr Sudoku() noexcept {}

    constexpr Sudoku(const std::array<int, SIZE> &arr) {
      for (int i = 0; const auto v : arr) {
        if (v) {
          m_elements[i++] = v;
        } else {
          m_elements[i++] = full_set();
        }
      }
    }

    [[nodiscard]] constexpr auto to_str() const noexcept -> std::string {
      if constexpr (NUM_ELEMENTS < 10) {
        std::string str((3 * SIDE_LENGTH + N) * (SIDE_LENGTH + N - 1), ' ');

        for (int i = 0; i < SIDE_LENGTH; ++i) {
          for (int j = 0; j < SIDE_LENGTH; ++j) {
            int ix = i * SIDE_LENGTH + j;
            std::size_t strix = (i + i / N) * SIDE_LENGTH + (3 * j + 1 + j / N);

            if (m_elements[ix]) {
              str[strix] = '0' + m_elements[ix];
            } else {
              str[strix] = '.';
            }
          }

          for (int j = 0; j < N - 1; ++j) {
            std::size_t strix = (i + i / N) * SIDE_LENGTH + (3 * N * (j + 1)) + j;
            str[strix] = '|';
          }

          std::size_t strix = (i + i / N) * SIDE_LENGTH + (3 * SIDE_LENGTH + N - 1);
          str[strix] = '\n';
        }

        for (int i = 0; i < N - 1; ++i) {
          for (int j = 0; j < 3 * SIDE_LENGTH + N - 1; ++j) {
            std::size_t strix = SIDE_LENGTH * N * (i + 1) + SIDE_LENGTH * i + j;
            if ((j + 1) % (3 * N + 1)) {
              str[strix] = '-';
            } else {
              str[strix] = '+';
            }
          }

          std::size_t strix = SIDE_LENGTH * N * (i + 1) + SIDE_LENGTH * i + 3 * SIDE_LENGTH + N - 1;
          str[strix] = '\n';
        }

        return str;
      } else {
        std::string str((4 * SIDE_LENGTH + N) * (SIDE_LENGTH + N - 1), ' ');
        return str;
      }
    }
  };
} // namespace jsm

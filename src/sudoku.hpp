namespace jsm {
  template <int N>
  requires (N > 1) Sudoku {
    using SIDE_LENGTH = N *N;
    using set_t = std::set<int>;
  };
} // namespace jsm

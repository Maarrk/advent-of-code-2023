# advent-of-code-2023

## Roadmap

- Pin all submodules to specific tag
- Copy data files to the final output directory on multi-config generator (like Debug/Release for Visual Studio)

## Log

_Implicit `std::` before some identifiers here_

- Day 1:
  - Setup CMake subfolders with copying data files
  - Complex initialization with lambda [ES.28](http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Res-lambda-init)
  - Try using algorithm with iterators (`find_if`)
- Day 2:
  - Introduce unit testing with [doctest](https://github.com/doctest/doctest)
  - Default equality operator
  - Implemented partial comparison with `<=>`, but didn't play nicely with test macros
- Day 3:
  - Start writing tests before implementation
  - Use `mdspan` and `mdarray` from [reference implementation](https://github.com/kokkos/mdspan)
    - Many convenience functions missing, and unexpected errors on `subspan`
    - In the end, just used `mdarray` as container with manual iteration
  - Use `format` and `map`
- Day 4:
  - Parse data with streams and `>>`, practice stream state flags
  - Write templated function and switch between `set` and `unordered_set`
  - Use `ranges` and `accumulate` (unfortunately not usable together yet)
- Day 5:
  - Create a wrapper class for an operation that validates some logic invariants
  - Define or default convenience operators on own types: `-`, `<=>`, `>>`
  - Basic template on underlying numeric type
- Day 10:
  - Enum of flags
  - `constexpr` functions
  - `variant` with `monostate` (to get sth like `optional<variant>`)
- Day 11:
  - `emplace_back` into a `vector`


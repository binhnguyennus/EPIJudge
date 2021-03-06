#include <algorithm>
#include <memory>
#include <tuple>
#include <unordered_set>
#include <vector>
#include "test_framework/generic_test.h"

using std::get;
using std::hash;
using std::make_tuple;
using std::make_unique;
using std::tuple;
using std::unordered_set;
using std::vector;

struct HashTuple;

bool IsPatternSuffixContainedStartingAtXY(
    const vector<vector<int>>&, int, int, const vector<int>&, int,
    unordered_set<tuple<int, int, int>, HashTuple>*);

struct HashTuple {
  size_t operator()(const tuple<int, int, int>& t) const {
    return hash<int>()(get<0>(t) ^ get<1>(t) * 1021 ^ get<2>(t) * 1048573);
  }
};

bool IsPatternContainedInGrid(const vector<vector<int>>& grid,
                              const vector<int>& pattern) {
  for (int i = 0; i < size(grid); ++i) {
    for (int j = 0; j < size(grid[i]); ++j) {
      if (IsPatternSuffixContainedStartingAtXY(
              grid, i, j, pattern, 0,
              make_unique<unordered_set<tuple<int, int, int>, HashTuple>>()
                  .get())) {
        return true;
      }
    }
  }
  return false;
}

// Each entry in previous_attempts is a point in the grid and suffix of
// pattern (identified by its offset). Presence in previous_attempts indicates
// the suffix is not contained in the grid starting from that point.
bool IsPatternSuffixContainedStartingAtXY(
    const vector<vector<int>>& grid, int x, int y, const vector<int>& pattern,
    int offset,
    unordered_set<tuple<int, int, int>, HashTuple>* previous_attempts) {
  if (size(pattern) == offset) {
    // Nothing left to complete.
    return true;
  }
  // Check if (x, y) lies outside the grid.
  if (x < 0 || x >= size(grid) || y < 0 || y >= size(grid[x]) ||
      previous_attempts->find(make_tuple(x, y, offset)) !=
          cend(*previous_attempts) ||
      grid[x][y] != pattern[offset]) {
    return false;
  }

  if (IsPatternSuffixContainedStartingAtXY(grid, x - 1, y, pattern, offset + 1,
                                           previous_attempts) ||
      IsPatternSuffixContainedStartingAtXY(grid, x + 1, y, pattern, offset + 1,
                                           previous_attempts) ||
      IsPatternSuffixContainedStartingAtXY(grid, x, y - 1, pattern, offset + 1,
                                           previous_attempts) ||
      IsPatternSuffixContainedStartingAtXY(grid, x, y + 1, pattern, offset + 1,
                                           previous_attempts)) {
    return true;
  }
  previous_attempts->emplace(x, y, offset);
  return false;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"grid", "pattern"};
  return GenericTestMain(args, "is_string_in_matrix.tsv",
                         &IsPatternContainedInGrid, DefaultComparator{},
                         param_names);
}

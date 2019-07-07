#include <vector>
#include "test_framework/generic_test.h"
using std::vector;

int MinimumPathWeight(const vector<vector<int>>& triangle) {
  if (triangle.size() == 0) {
    return 0;
  }
  std::vector<int> table(triangle.size());
  table[0] = triangle[0][0];
  int min_weight = table[0];
  for (int i=1; i<triangle.size(); i++) {
    table[i] = table[i-1] + triangle[i][i];
    min_weight = table[i];
    for (int j=i-1; j>0; j--) {
      table[j] = std::min(table[j-1], table[j]) + triangle[i][j];
      min_weight = std::min(min_weight, table[j]);
    }
    table[0] = table[0] + triangle[i][0];
    min_weight = std::min(min_weight, table[0]);
  }
  return min_weight;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"triangle"};
  return GenericTestMain(args, "minimum_weight_path_in_a_triangle.cc",
                         "minimum_weight_path_in_a_triangle.tsv",
                         &MinimumPathWeight, DefaultComparator{}, param_names);
}

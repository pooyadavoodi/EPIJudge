#include "test_framework/generic_test.h"

int ComputeBinomialCoefficient(int n, int k) {
//  std::vector<int> bin_coef(n-k+1);
//  return ComputeBinomialCoefficientHelper(n, k, bin_coef);
  if (n == 0) {
    return 0;
  }
  if (k == 0) {
    return 1;
  }
  if (n == k) {
    return 1;
  }
  int64_t bin_coef = 1;
  for (int i = k+1; i<=n; i++) {
    bin_coef = bin_coef * i / (i-k);
  }
  return bin_coef;
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"n", "k"};
  return GenericTestMain(
      args, "binomial_coefficients.cc", "binomial_coefficients.tsv",
      &ComputeBinomialCoefficient, DefaultComparator{}, param_names);
}

#include <string>
#include <unordered_set>
#include <vector>
#include "test_framework/generic_test.h"
#include "test_framework/test_failure.h"
#include "test_framework/timed_executor.h"
using std::string;
using std::unordered_set;
using std::vector;

vector<string> DecomposeIntoDictionaryWords(
    const string& domain, const unordered_set<string>& dictionary) {

  std::map<string, bool> table;
  std::map<string, std::vector<string> > table_strings;

  for (int i=0; i<domain.size(); i++) {
    string prefix = domain.substr(0, i+1);
    if (dictionary.count(prefix)) {
      table[prefix] = true;
      table_strings[prefix] = {prefix};
    }
    else {
      int j;
      for (j=i; j>=0; j--) {
        auto suffix_of_prefix = domain.substr(j, i-j+1);
        auto prefix_of_prefix = domain.substr(0, j);
        if (dictionary.count(suffix_of_prefix)) {
          if (table[prefix_of_prefix] == true) {
            table[prefix] = true;
            table_strings[prefix] = table_strings[prefix_of_prefix];
            table_strings[prefix].push_back(suffix_of_prefix);
            break;
          }
        }
      }
      if (j < 0) {
        table[prefix] = false;
      }
    }
  }
  return table_strings[domain];
}
void DecomposeIntoDictionaryWordsWrapper(
    TimedExecutor& executor, const string& domain,
    const unordered_set<string>& dictionary, bool decomposable) {
  vector<string> result = executor.Run(
      [&] { return DecomposeIntoDictionaryWords(domain, dictionary); });
  if (!decomposable) {
    if (!result.empty()) {
      throw TestFailure("domain is not decomposable");
    }
    return;
  }

  if (std::any_of(std::begin(result), std::end(result),
                  [&](const std::string& s) { return !dictionary.count(s); })) {
    throw TestFailure("Result uses words not in dictionary");
  }

  if (std::accumulate(std::begin(result), std::end(result), string()) !=
      domain) {
    throw TestFailure("Result is not composed into domain");
  }
}

int main(int argc, char* argv[]) {
  std::vector<std::string> args{argv + 1, argv + argc};
  std::vector<std::string> param_names{"executor", "domain", "dictionary",
                                       "decomposable"};
  return GenericTestMain(args, "is_string_decomposable_into_words.cc",
                         "is_string_decomposable_into_words.tsv",
                         &DecomposeIntoDictionaryWordsWrapper,
                         DefaultComparator{}, param_names);
}

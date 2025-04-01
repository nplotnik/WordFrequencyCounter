// WordFrequencyCounter.cpp : Defines the entry point for the application.
//

#include "WordFrequencyCounter.h"
#include <algorithm>
#include <filesystem>
#include <format>
#include <fstream>
#include <queue>
#include <sstream>
#include <unordered_map>

class TextProcessor {
public:
  using Frequencies = std::unordered_map<std::string, int>;
  using Frequency = std::pair<std::string, int>;

  TextProcessor(const std::string &i_inputData) : d_inputData(i_inputData) {}

  void calculateWords() {
    int startIndex = 0;
    int length = 0;
    for (int i = 0; i < d_inputData.size(); ++i) {
      if (std::isalpha(d_inputData[i])) {
        length++;
      } else if (length == 0) {
        startIndex++;
      } else {
        std::string currentWord(d_inputData.begin() + startIndex,
                                d_inputData.begin() + startIndex + length);
        std::transform(currentWord.begin(), currentWord.end(),
                       currentWord.begin(),
                       [](unsigned char c) { return std::tolower(c); });
        d_frequencies[currentWord]++;
        startIndex += length + 1;
        length = 0;
      }
    }
  }

  std::vector<Frequency> getSortedFrequencies() {
    calculateWords();
    for (const auto &frequency : d_frequencies) {
      std::cout << frequency.first << " " << frequency.second << std::endl;
      d_sortedFrequencies.emplace(frequency.first, frequency.second);
    }
    std::vector<Frequency> result;
    while (d_sortedFrequencies.size() > 0) {
      result.push_back(d_sortedFrequencies.top());
      d_sortedFrequencies.pop();
    }
    return result;
  }

private:
  struct CustomComparator {
    bool operator()(const Frequency &firstFrequency,
                    const Frequency &secondFrequency) {
      if (firstFrequency.second == secondFrequency.second)
        return firstFrequency.first > secondFrequency.first;
      return firstFrequency.second < secondFrequency.second;
    }
  };

  std::string d_inputData;
  Frequencies d_frequencies;

  std::priority_queue<Frequency, std::vector<Frequency>, CustomComparator>
      d_sortedFrequencies;
};

std::ostream &
operator<<(std::ostream &out,
           const std::vector<TextProcessor::Frequency> frequencies) {
  for (const auto &frequency : frequencies) {
    out << frequency.second << " " << frequency.first << std::endl;
  }
  return out;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "Usage: ./WordFrequencyCounter [input_file] [output_file]\n";
    return 0;
  }

  std::filesystem::path inputFileName = argv[1];
  std::filesystem::path outputFileName = argv[2];

  if (!std::filesystem::exists(inputFileName)) {
    std::cerr << std::format("Input file {} doesn't exist\n",
                             inputFileName.string());
  }

  std::ifstream input(inputFileName);
  std::string content(std::istreambuf_iterator<char>(input), {});

  TextProcessor textProcessor(content);
  auto result = textProcessor.getSortedFrequencies();

  std::ofstream output(outputFileName);
  output << result;

  return 0;
}

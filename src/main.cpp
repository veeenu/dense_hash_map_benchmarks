#include <unordered_map>

#include <sparsehash/dense_hash_map>
#include <timer.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

int main() {

  OneShotTimer t;

  std::cout << "Reading tickers list... " << std::flush;

  t.start();

  std::ifstream i("tickers.csv", std::ifstream::in);
  std::vector<std::string> tickers;
  std::string s;

  while (!i.eof()) {
    std::getline(i, s);
    s += " EQUITY";
    tickers.push_back(s);
  }

  t.stop();
  std::cout << t.repr() << std::endl;

  std::unordered_map<std::string, double> um;
  google::dense_hash_map<std::string, double> dhm;
  dhm.set_empty_key(std::string(""));


  std::cout << "Inserting:" << std::endl;

  srand(31337);
  t.start();
  for (auto s : tickers) {
    double d = rand() / 1000.;
    um.emplace(s, d);
  }
  t.stop();

  std::cout << "  unordered_map:  " << t.repr() << std::endl;

  srand(31337);
  t.start();
  for (auto s : tickers) {
    double d = rand() / 1000.;
    dhm.emplace(s, d);
  }
  t.stop();
  
  std::cout << "  dense_hash_map: " << t.repr() << std::endl;

  std::cout << "Random access:" << std::endl;

  t.start();
  for (int i = 0; i < 10000; i++) {
    for (auto s : tickers) {
      auto& it = um.find(s);
    }
  }
  t.stop();

  std::cout << "  unordered_map:  " << t.repr() << std::endl;

  t.start();
  for (int i = 0; i < 10000; i++) {
    for (auto s : tickers) {
      auto& it = dhm.find(s);
    }
  }
  t.stop();

  std::cout << "  dense_hash_map: " << t.repr() << std::endl;

  std::cout << "Single key:" << std::endl;

  s = tickers[tickers.size() / 2];

  t.start();
  for (int i = 10000 * tickers.size(); i > 0; i--) {
    auto& it = um.find(s);
  }
  t.stop();

  std::cout << "  unordered_map:  " << t.repr() << std::endl;

  t.start();
  for (int i = 10000 * tickers.size(); i > 0; i--) {
    auto& it = dhm.find(s);
  }
  t.stop();

  std::cout << "  dense_hash_map: " << t.repr() << std::endl;

}
#ifndef __TIMER_H
#define __TIMER_H
#pragma once

#include <chrono>
#include <sstream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <memory>

std::string sec_human_repr (double seconds) {
  auto sec = std::chrono::duration<double>(seconds);
  auto ms = std::chrono::duration<double, std::milli>(sec).count();
  auto us = std::chrono::duration<double, std::micro>(sec).count();
  auto ns = std::chrono::duration<double, std::nano>(sec).count();
  typedef std::pair<double, std::string> p;
  std::vector<p> m({
    { sec.count(), "s" },
    { ms, "ms" },
    { us, "us" },
    { ns, "ns" },
  }), m_gt1;

  std::copy_if(m.begin(), m.end(), std::back_inserter(m_gt1), [](p a) { return a.first > 1.; });
  auto target = *std::min_element(m_gt1.begin(), m_gt1.end(), [](p a, p b) -> bool { return a.first < b.first; });

  std::ostringstream oss;
  oss << std::fixed << std::setprecision(2) << target.first << target.second;
  return oss.str();
}

class OneShotTimer {
  private:
    std::chrono::high_resolution_clock::time_point first_measurement;
    std::chrono::high_resolution_clock::time_point last_measurement;
  public:
    OneShotTimer () {};
    OneShotTimer (const OneShotTimer&) = delete;
    OneShotTimer (OneShotTimer&&) = default;
    OneShotTimer& operator= (const OneShotTimer&) = delete;
    OneShotTimer& operator= (OneShotTimer&&) = default;

    void start () {
      first_measurement = std::chrono::high_resolution_clock::now();
    }
    void stop () {
      last_measurement = std::chrono::high_resolution_clock::now();
    }

    const double total () const {
      return std::chrono::duration<double>(last_measurement - first_measurement).count();
    }
    std::string repr () const {
      return sec_human_repr(total());
    }
};

class LoopTimer {
  private:
    std::vector<double> measurements;
    std::chrono::high_resolution_clock::time_point first_measurement;
    std::chrono::high_resolution_clock::time_point last_measurement;
    std::size_t index;

    double computed_mean;
    double computed_std;
    double computed_total;
  public:
    LoopTimer (std::size_t size) 
    : measurements(size), index(0), 
      computed_mean(0.), computed_std(0.), computed_total(0.) {}

    LoopTimer (const LoopTimer&) = delete;
    LoopTimer (LoopTimer&&) = default;
    LoopTimer& operator= (const LoopTimer&) = delete;
    LoopTimer& operator= (LoopTimer&&) = default;

    void start () {
      first_measurement = last_measurement = std::chrono::high_resolution_clock::now();
      index = 0;
    }
    void split () {
      auto t = std::chrono::high_resolution_clock::now();
      measurements[index] = std::chrono::duration<double>(t - last_measurement).count();
      ++index;
      last_measurement = t;
    }
    void stop () {
      computed_total = std::chrono::duration<double>(last_measurement - first_measurement).count();
      computed_mean = computed_total / measurements.size();
      computed_std = 0;
      for (auto it = measurements.begin(); it != measurements.end(); ++it) {
        auto d = (*it) - computed_mean;
        computed_std += d*d;
      }
      computed_std = std::sqrt(computed_std / (measurements.size() - 1));
    }

    const double mean () const { return computed_mean; };
    const double std () const { return computed_std; };
    const double total () const { return computed_total; };
    const std::vector<double>& get_measurements () const { return measurements; }
    std::string repr () const {
      std::ostringstream oss;
      oss << sec_human_repr(total()) << " / " << measurements.size()
          << " iterations (mean " << sec_human_repr(mean())
          << " std " << sec_human_repr(std()) << ")";
      return oss.str();
    }
};

#endif // TIMER_H
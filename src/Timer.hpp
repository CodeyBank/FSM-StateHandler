/**
 * @file Timer.hpp
 * @author Stanley Ezeh (ezeh.stanley.es@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-07-09
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once

#include <atomic>
#include <chrono>
#include <iostream>
#include <mutex>
#include <thread>

#define TimerI Timer::GetInstance()

class Timer {
private:
  static Timer *mInstance;
  int mTbase;
  int m_ss;
  std::atomic<bool> running{false}; // not necessary if this is a singleton
  Timer(const Timer &) = delete;    // delete the copy constructor
  Timer &operator=(const Timer &) = delete; // delete the assignment constructor
  std::mutex mtx;

public:
  /**
   * @brief Construct a new Timer object
   *
   * @param rTBase
   */
  Timer(int rTBase);

  /**
   * @brief
   *
   */
  void run();

  void interrupt();

  /**
   * @brief Get the Instance object
   *
   * @return Timer*
   */
  static Timer *GetInstance();

  /**
   * @brief stop the running timer
   *
   */
  void stop();

  /**
   * @brief Destroy the Timer object
   *
   */
  ~Timer();
};

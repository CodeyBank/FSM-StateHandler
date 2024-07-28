/**
 * @file main.cpp
 * @author Stanley Ezeh (ezeh.stanley.es@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-07-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#include <ncurses.h>

#include "NcursesManager.hpp"
#include "Timer.hpp"
#include "protimer_fsm.hpp"
#include <iostream>

using namespace std;
// single instance
ProTimer *ProTimer::mInstance = nullptr;
int main() {
  ProTimer pt;
  Timer tim(100);
  Ui.run();
  return 0;
}
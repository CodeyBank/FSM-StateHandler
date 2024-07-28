/**
 * @file AppTask.hpp
 * @author Stanley Ezeh (ezeh.stanley.es@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-07-02
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "protimer_fsm.hpp"
#include <mutex>

// #define NO_COPY_MOVE(obj) (obj)

/**
 * @brief Manages the entire application
 *
 */
class AppTask final {
  AppTask(const AppTask &obj) = delete;
  AppTask(AppTask &&source) = delete;
  std::mutex mtx;

public:
  AppTask() {}
  ~AppTask();

  /**
   * @brief calls the state handler for given event
   *
   * @param rFsmObj
   * @param rEv
   * @return event_status_t
   */
  event_status_t EventDispatcher(ProTimer *rFsmObj, Event *rEv);

  void ApplicationHandler(ProTimer *rFsmObj, Event *rEv);

  void Test(const char *message);
};

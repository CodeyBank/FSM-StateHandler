/**
 * @file main.h
 * @author Stanley Ezeh (ezeh.stanley.es@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-07-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#pragma once
#include "ProtimerAssert.hpp"
#include <iostream>
#include <stdint.h>
#include <functional>
#include "ProtimerDefs.h"

#define ProtimerI ProTimer::GetInstance()


 event_status_t StateHandler_OFF(void *rObj, Event const *const rEV);
 event_status_t StateHandler_IDLE(void *rObj, Event const *const rEV);
 event_status_t StateHandler_TIMESET(void *rObj, Event const *const rEV);
 event_status_t StateHandler_PAUSE(void *rObj, Event const *const rEV);
 event_status_t StateHandler_STAT(void *rObj, Event const *const rEV);
 event_status_t StateHandler_COUNTDOWN(void *rObj, Event const *const rEV);

/**
 * @brief Protimer Object to hold the application state
 *
 */
class ProTimer {
public:
  uint32_t e_time; // elapsed time
  uint32_t c_time; // current time
  uint32_t p_time; // productive time
  protimer_state_t active_state;

  ProTimer() : p_time(0), active_state(IDLE_STATE), mPreviousState(OFF_STATE) {

    PROTIMER_ASSERT(mInstance == nullptr,
                    "Cannot have two instances of Protimer");
    mInstance = this;

    // initialize the handlers
    f_active_state = StateHandler_OFF;
    
  }


  protimer_state_t GetPreviousState();
  void SetNextState(protimer_state_t &&rNewState){mNextState = rNewState;}
  void SetPrevState(protimer_state_t &&rState){mPreviousState=rState;}



  inline static ProTimer *GetInstance() { return mInstance; }

  void test();
  inline protimer_state_t GetNextState() { return mNextState; }


  // Implementing the state handler approach
  using func_state_handler_t =  std::function<event_status_t(void *rObj, Event const *const rEV)>;

  func_state_handler_t f_next_state ;
  func_state_handler_t f_previous_state;
  func_state_handler_t f_active_state;

  void SetNextStateFunc(func_state_handler_t rNewState){f_next_state = rNewState;}
  void SetPrevStateFunc(func_state_handler_t rState){f_previous_state = rState;}


private:
  protimer_state_t mPreviousState;
  protimer_state_t mNextState;

  static ProTimer *mInstance;

  ProTimer(const ProTimer &) = delete;
  ProTimer &operator=(const ProTimer &) = delete;
};


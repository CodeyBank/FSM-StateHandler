/**
 * @file AppTask.cpp
 * @author Stanley Ezeh (ezeh.stanley.es@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-07-02
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "AppTask.hpp"
#include "NcursesManager.hpp"
#include <iostream>
#include <vector>

extern std::string aStateNames[6];
extern std::vector<std::string> aSignalNames;

AppTask::~AppTask() {}


void AppTask::ApplicationHandler(ProTimer *rFsmObj, Event *rEv) {
  std::lock_guard<decltype(mtx)> lock(mtx);
  event_status_t tStatus{};

  auto tSourceState = rFsmObj->GetPreviousState();
  std::string msg{__func__};
  msg += ": Source state->" + aStateNames[tSourceState] + " Received event:->" +
         aSignalNames[rEv->sig];
  Ui.disp_debug_msg(msg.c_str());
  tStatus = rFsmObj->f_active_state(rFsmObj, rEv);
   

  if (tStatus == EVENT_TRANSITION) {
    auto tTargetState = (rFsmObj)->GetNextState();
    std::string msg{"[" + std::string(__func__) + "]"};
    msg += ": " + aStateNames[tSourceState] + "->" + aStateNames[tTargetState];
    Ui.disp_debug_msg(msg.c_str());

    Event tEv;

    // 1. run exit event for the source state
    tEv.sig = INT_EXIT_SIG;
    rFsmObj->f_active_state(rFsmObj, &tEv);


    // 2. run the entry state of the new state

    tEv.sig = INT_ENTRY_SIG;
    auto target_stateHandler = rFsmObj->f_next_state;
    rFsmObj->active_state = tTargetState;
    rFsmObj->f_next_state(rFsmObj, &tEv);

  }
}

void AppTask::Test(const char *message) {
  NcurseUi::GetInstance().display_message(10, 2, message);
  // std::cout<<"Hi world \n";
}

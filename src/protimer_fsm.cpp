/**
 * @file protimer_fsm.cpp
 * @author Stanley Ezeh (ezeh.stanley.es@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-07-01
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "protimer_fsm.hpp"
#include "NcursesManager.hpp"
#include "Timer.hpp"

#include <numeric>
#include <sstream>
#include <string>
#include <vector>



std::string aStateNames[6] = {"OFF",   "IDLE",      "STAT",
                              "PAUSE", "COUNTDOWN", "TIMESET"};
std::vector<std::string> aSignalNames = {
    /* User signals */
    "USER_INC_TIME_SIG", "USER_DEC_TIME_SIG", "USER_TIME_TICK_SIG",
    "USER_START_PAUSE_SIG", "USER_ABORT_SIG",
    /* Internal Signals */
    "INT_ENTRY_SIG", "INT_EXIT_SIG"};

event_status_t StateHandler_OFF(void *rObj,Event const *const rEV) {
  event_status_t RetEvtStatus = EVENT_IGNORED;
  ProTimer* obj = static_cast<ProTimer*>(rObj);
  switch (rEV->sig) {
  case INT_ENTRY_SIG: {

    // Ui.display_clear();
    obj->SetNextState(IDLE_STATE);
    obj->SetNextStateFunc(StateHandler_IDLE);
    RetEvtStatus = event_status_t::EVENT_TRANSITION;
    // TimerI->run();
    break;
  }

  case INT_EXIT_SIG: {
    RetEvtStatus = event_status_t::EVENT_TRANSITION;
    // TimerI->stop();
    break;
  }

  default:
    break;
  }
  return RetEvtStatus;
}

event_status_t StateHandler_IDLE(void *rObj, Event const *const rEV) {
  event_status_t RetEvtStatus = EVENT_IGNORED;
  ProTimer* obj = static_cast<ProTimer*>(rObj);
  switch (rEV->sig) {
  case INT_ENTRY_SIG: {
    /* code */
    obj->c_time = 0;
    obj->e_time = 0;
    Ui.delete_line(1);
    Ui.display_message(1, 1, "State: IDLE", true);
    Ui.display_message(2, 3, "Set Time");
    Ui.display_time(obj->c_time);
    obj->SetPrevState(IDLE_STATE);
    obj->f_active_state = StateHandler_IDLE;
    TimerI->run();
    RetEvtStatus = event_status_t::EVENT_HANDLED;
    break;
  }

  case INT_EXIT_SIG: {
    /* code */
    Ui.disp_debug_msg("Exiting IDLE state\n");
    TimerI->stop();
    break;
  }

  case USER_INC_TIME_SIG: {
    /* code */
    obj->c_time += 60;
    obj->SetPrevState(IDLE_STATE);
    obj->SetNextState(TIME_SET_STATE);
    obj->SetNextStateFunc(StateHandler_TIMESET);
    RetEvtStatus = EVENT_TRANSITION;
    break;
  }

  case USER_START_PAUSE_SIG: {
    obj->SetNextState(STAT_STATE);
    obj->SetPrevState(IDLE_STATE);
    obj->SetNextStateFunc(StateHandler_STAT);
    RetEvtStatus = EVENT_TRANSITION;
    break;
  }

  case USER_TIME_TICK_SIG: {
    // internal transition
    // guard is if e->ss == 5
    // action is do_beep();
    if (reinterpret_cast<const TickEvent *>(rEV)->m_ss == 5) {
      Ui.do_beep();
      RetEvtStatus = EVENT_HANDLED;
      break;
    }
    RetEvtStatus = EVENT_IGNORED;
    break;
  }

  default:
    break;
  }
  return RetEvtStatus;
}

event_status_t StateHandler_TIMESET(void *rObj,Event const *const rEV) {
  event_status_t RetEvtStatus = EVENT_IGNORED;
  ProTimer* obj = static_cast<ProTimer*>(rObj);
  switch (rEV->sig) {
  case INT_ENTRY_SIG: {
    Ui.delete_line(1);
    Ui.display_message(1, 1, "State: TIMESET", true);
    obj->SetPrevState(TIME_SET_STATE);
    Ui.display_time(obj->c_time);
    obj->active_state = TIME_SET_STATE;
    obj->f_active_state = StateHandler_TIMESET;
    RetEvtStatus = EVENT_HANDLED;
    break;
  }
  case INT_EXIT_SIG: {
    obj->p_time = 0;
    RetEvtStatus = EVENT_HANDLED;
    break;
  }

  case USER_INC_TIME_SIG: {
    obj->c_time += 60;
    Ui.display_time(obj->c_time);
    RetEvtStatus = EVENT_HANDLED;
    break;
  }

  case USER_DEC_TIME_SIG: {
    if (obj->c_time >= 60) {
      obj->c_time -= 60;
      Ui.display_time(obj->c_time);
      RetEvtStatus = EVENT_HANDLED;
      break;
    }
    RetEvtStatus = EVENT_HANDLED;
    break;
  }

  case USER_START_PAUSE_SIG: {
    if (obj->c_time >= 60) {
      obj->SetNextState(COUNTDOWN_STATE);
      obj->SetPrevState(TIME_SET_STATE);
      obj->SetNextStateFunc(StateHandler_COUNTDOWN);
      RetEvtStatus = EVENT_TRANSITION;
      break;
    }
    RetEvtStatus = EVENT_IGNORED;
    break;
  }

  default:
    break;
  }
  return RetEvtStatus;
}

event_status_t StateHandler_PAUSE(void *rObj, Event const *const rEV) {
  event_status_t RetEvtStatus = EVENT_IGNORED;
  ProTimer* obj = static_cast<ProTimer*>(rObj);
  switch (rEV->sig) {
  case INT_ENTRY_SIG: {
    Ui.delete_line(1);
    Ui.display_message(1, 0, "State: PAUSED", true);
    obj->f_active_state = StateHandler_PAUSE;
    RetEvtStatus = EVENT_HANDLED;
    break;
  }
  case INT_EXIT_SIG: {
    RetEvtStatus = EVENT_HANDLED;
    break;
  }
  case USER_INC_TIME_SIG: {
    obj->c_time += 60;
    obj->SetPrevState(PAUSE_STATE);
    obj->SetNextState(TIME_SET_STATE);
    obj->SetNextStateFunc(StateHandler_TIMESET);
    RetEvtStatus = EVENT_TRANSITION;
    break;
  }

  case USER_DEC_TIME_SIG: {
    if (obj->c_time >= 60) {
      obj->c_time -= 60;
      obj->SetPrevState(PAUSE_STATE);
      obj->SetNextState(TIME_SET_STATE);
      obj->SetNextStateFunc(StateHandler_TIMESET);
      RetEvtStatus = EVENT_TRANSITION;
      break;
    }
    RetEvtStatus = EVENT_IGNORED;
    break;
  }

  case USER_ABORT_SIG: {
    obj->SetPrevState(PAUSE_STATE);
    obj->active_state = IDLE_STATE;
    obj->SetNextStateFunc(StateHandler_IDLE);
    RetEvtStatus = EVENT_TRANSITION;
    break;
  }

  case USER_START_PAUSE_SIG: {
    if (obj->c_time >= 60) {
      obj->SetPrevState(PAUSE_STATE);
      obj->SetNextState(COUNTDOWN_STATE);
      obj->SetNextStateFunc(StateHandler_COUNTDOWN);
      RetEvtStatus = EVENT_TRANSITION;
      break;
    }
  }

  case USER_STATUS_SIG: {
    obj->SetPrevState(PAUSE_STATE);
    obj->SetNextState(STAT_STATE);
    obj->SetNextStateFunc(StateHandler_STAT);
    RetEvtStatus = EVENT_TRANSITION;
    break;
  }
  default:
    break;
  }
  return RetEvtStatus;
}

event_status_t StateHandler_STAT(void *rObj, Event const *const rEV) {
  event_status_t RetEvtStatus = EVENT_IGNORED;
  ProTimer* obj = static_cast<ProTimer*>(rObj);
  static int time = 0;

  switch (rEV->sig) {
  case INT_ENTRY_SIG: {
    Ui.delete_line(1);
    Ui.display_message(1, 1, "State: STATUS", true);
    Ui.display_time(obj->p_time);
    obj->f_active_state = StateHandler_STAT;
    obj->SetPrevState(STAT_STATE);
    RetEvtStatus = EVENT_HANDLED;
    TimerI->run();
    break;
  }
  case INT_EXIT_SIG: {
    RetEvtStatus = EVENT_HANDLED;
    time=0;
    break;
  }

  case USER_TIME_TICK_SIG: {
    
    if (reinterpret_cast<const TickEvent *>(rEV)->m_ss == 10) 
    {
      time++;
    }

    if (time == 5) {
      obj->SetPrevState(STAT_STATE);
      obj->SetNextState(COUNTDOWN_STATE);
      obj->SetNextStateFunc(StateHandler_COUNTDOWN);
      RetEvtStatus = EVENT_TRANSITION;
      break;
    }
    else {
      RetEvtStatus = EVENT_IGNORED;
      break;
    }
  }

  default:
    break;
  }

  return RetEvtStatus;
}

event_status_t StateHandler_COUNTDOWN(void *rObj, Event const *const rEV) {
  event_status_t RetEvtStatus = EVENT_IGNORED;
  ProTimer* obj = static_cast<ProTimer*>(rObj);

  switch (rEV->sig) {

  case INT_ENTRY_SIG: {
    Ui.delete_line(1);
    Ui.display_message(1, 1, "State: COUNTDOWN", true);
    Ui.disp_debug_msg("Starting timer");
    obj->f_active_state = StateHandler_COUNTDOWN;
    TimerI->run();
    RetEvtStatus = EVENT_HANDLED;
    break;
  }
  case INT_EXIT_SIG: {
    obj->p_time += obj->e_time;
    obj->e_time = 0;
    TimerI->stop();
    RetEvtStatus = EVENT_HANDLED;
    break;
  }

  case USER_TIME_TICK_SIG: {
    // choice pseudostate
    // char msg[39];
    // sprintf(msg, "m->ss = %d", reinterpret_cast<const TickEvent
    // *>(rEV)->m_ss); Ui.display_message(5, 0, msg);
    if (reinterpret_cast<const TickEvent *>(rEV)->m_ss == 10 && obj->c_time > 0) {
      --obj->c_time;
      ++obj->e_time;
      Ui.display_time(obj->c_time);
      if (obj->c_time == 0) { // fire an external transition to go back to idle state
        obj->SetPrevState(COUNTDOWN_STATE);
        obj->SetNextState(IDLE_STATE);
        obj->SetNextStateFunc(StateHandler_IDLE);
        RetEvtStatus = EVENT_TRANSITION;
        break;
      }
    }
    RetEvtStatus = EVENT_HANDLED;
    break;
  }

  case USER_START_PAUSE_SIG: {
    obj->SetNextState(PAUSE_STATE);
    obj->SetNextStateFunc(StateHandler_PAUSE);
    obj->SetPrevState(COUNTDOWN_STATE);
    RetEvtStatus = EVENT_TRANSITION;
    break;
  }
  }
  return RetEvtStatus;
}

protimer_state_t ProTimer::GetPreviousState() { return active_state; }

void ProTimer::test() {
  NcurseUi::GetInstance().display_message(0, 0, "Hello world", true);
  napms(1000);
  NcurseUi::GetInstance().do_beep();
  napms(1000);
}

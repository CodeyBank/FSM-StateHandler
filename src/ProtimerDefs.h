/**
 * @file ProtimerDefs.h
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-07-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <stdint.h>
// Forward declaration

struct Event;

/**
 * @brief
 *
 */
typedef enum { EVENT_HANDLED, EVENT_IGNORED, EVENT_TRANSITION } event_status_t;

/**
 * @brief States of the application
 *
 */
typedef enum {
  OFF_STATE,
  IDLE_STATE,
  STAT_STATE,
  PAUSE_STATE,
  COUNTDOWN_STATE,
  TIME_SET_STATE,
  STATE_LAST
} protimer_state_t;

/**
 * @brief Signals
 *
 */
typedef enum {

  /* User signals */
  USER_INC_TIME_SIG,
  USER_DEC_TIME_SIG,
  USER_TIME_TICK_SIG,
  USER_START_PAUSE_SIG,
  USER_STATUS_SIG,
  USER_ABORT_SIG,

  /* Internal Signals */
  INT_ENTRY_SIG,
  INT_EXIT_SIG,

  /*LAST*/
  SIGNAL_LAST

} protimer_signal_t;


/* Base user event class */
struct Event {
  uint8_t sig;
};

/* Tick event class */
struct TickEvent : public Event {
  uint32_t m_ss; // sub-seconds
};

/* User generated events*/
struct UserEvent : public Event {
  // other definitions can go here
};

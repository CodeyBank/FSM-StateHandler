/**
 * @file Fsm.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2024-07-28
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#include <queue>
#include <mutex>
#include <atomic>
#include <thread>

#include "ProtimerDefs.h"

class Fsm
{
public:
    Fsm(void *Obj);
    ~Fsm();

    bool PostEvent(const Event &ev);
    protimer_state_t GetCurrentState();
    protimer_state_t GetNextState();
    void stop_fsm();
    void start_fsm();

private:
    protimer_state_t active_state;
    void ProcessEvent(const Event &ev);
    std::atomic<bool> running;
};
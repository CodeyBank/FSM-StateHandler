#include "Fsm.hpp"

Fsm::Fsm(void *Obj)
{
}

Fsm::~Fsm()
{
}

bool Fsm::PostEvent(const Event &ev)
{
    return false;
}

protimer_state_t Fsm::GetCurrentState()
{
    return protimer_state_t();
}

protimer_state_t Fsm::GetNextState()
{
    return protimer_state_t();
}

void Fsm::stop_fsm()
{
}

void Fsm::start_fsm()
{
}

void Fsm::ProcessEvent(const Event &ev)
{
}

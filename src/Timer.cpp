#include "Timer.hpp"
#include "AppTask.hpp"
#include "NcursesManager.hpp"
#include "ProtimerAssert.hpp"

Timer *Timer::mInstance = nullptr;

class MultipleInstacesException : public std::exception {
public:
  const char *what() const noexcept override {
    return "Cannot instantiate Timer Singleton twice";
  }
};

Timer::Timer(int rTBase) : mTbase(rTBase), m_ss(0) {
  // PROTIMER_ASSERT(mInstance !=nullptr, "Cannot instantiate Timer Singleton
  // twice" );

  // if timer has been instantiated throw error
  if (mInstance != nullptr) {
    throw MultipleInstacesException();
  }

  mInstance = this;
  // std::cout << "Timer instantiated" << std::endl;
}

void Timer::run() {
  // std::cout << "Timer started with base " << mTbase << std::endl;
  running = true;
  std::thread timerThread([this]() { interrupt(); });
  timerThread.detach();
}

void Timer::interrupt() {
  AppTask app;
  TickEvent e;
  while (running) {
    std::this_thread::sleep_for(std::chrono::milliseconds(mTbase));
    m_ss++;

    e.sig = USER_TIME_TICK_SIG;
    e.m_ss = m_ss % 10 + 1; // m_ss will always be between 1 and 10

    app.ApplicationHandler(ProtimerI, &e);
  }
}

Timer *Timer::GetInstance() { return mInstance; }

void Timer::stop() { running = false; }

Timer::~Timer() {
  std::cout << "Stopping timer" << std::endl;
  mInstance = nullptr;
  running = false;
}
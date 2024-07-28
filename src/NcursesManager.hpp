#pragma once
#include "AppTask.hpp"
#include "protimer_fsm.hpp"
#include <atomic>
#include <cstring>
#include <iostream>
#include <memory>
#include <mutex>
#include <ncurses.h>
#include <optional>
#include <thread>

#define Ui NcurseUi::GetInstance()
// Singleton NcurseUi class
class NcurseUi {
public:
  static NcurseUi &GetInstance() {
    static NcurseUi instance;
    return instance;
  }

  ~NcurseUi() { endwin(); }

  void run() {
    std::thread inputThread(&NcurseUi::handleInput, this);
    inputThread.join();
  }

  /**
   * @brief
   * @param col column. This argument is optional if the justify arg is set to
   * true
   * @param row
   * @param message
   * @param justify if true, text is centered on the window.
   */
  void display_message(int row, std::optional<int> col, const char *message,
                       bool justify = false) {
    std::lock_guard<decltype(mtx)> lock(mtx);
    if (justify) {
      int len = strlen(message);
      col = mWindowWidth / 2 - len / 2;
    }
    wmove(win, row, col.value());
    mvwprintw(win, row, col.value(), "%s", message);
    wrefresh(win);
  }

  /**
   * @brief clear the window
   *
   */
  void display_clear() {
    std::lock_guard<decltype(mtx)> lock(mtx);
    for (int idx{1}; idx < mWindowHeight; idx++) {
      delete_line(idx);
    }
    wrefresh(win);
  }

  void do_beep() { beep(); }

  /**
   * @brief Display the current time at row
   *
   * @param current_time
   * @param row row to display time
   */
  void display_time(uint32_t current_time, int row = 3) {
    // convert the time in int to chrono time
    std::lock_guard<decltype(mtx)> lock(mtx);
    mvwprintw(win, 3, 3, "Current time: %03d:%02d", current_time / 60,
              current_time % 60);

    wrefresh(win);
  }

  void adjustWindowSize(int width, int height) {}

  /**
   * @brief delay for specified ms
   *
   * @param ms
   */
  void delay(int ms) { napms(ms); }

  /**
   * @brief
   *
   * @param start line start row
   * @param stop line stop row
   */
  void del_multiple_lines(int start, int stop) {
    for (int idx = start; idx <= stop; idx++) {
      delete_line(idx);
    }
  }

  void delete_line(int line) {

    int width = mWindowWidth - 2;
    char cl[width];
    memset(cl, ' ', width);
    cl[width-1] = '\0';
    display_message(line, 1, cl);
  }

  /**
   * @brief Displays a debug message using two last lines
   *
   * @param message
   */
  void disp_debug_msg(const char *message) {
    del_multiple_lines(5, 6);
    int len = strlen(message);
    int width = mWindowWidth - 3;
    // if the debug message is longer than mwindow *2
    if (len > width * 2) {
      len = width * 2;
    }

    if (len > width) {
      // truncate the message to fit one line and print the rest on the next
      // line
      char first_half[width] = {'\0'};
      char second_half[width] = {'\0'};

      int idx, jdx;
      for (idx = 0; idx < width; idx++) {
        first_half[idx] = message[idx];
      }

      for (jdx = 0; jdx < (len - width); jdx++) {
        second_half[jdx] = message[jdx + idx];
      }
      first_half[++idx] = '\0';
      second_half[++jdx] = '\0';
      display_message(5, 1, first_half);
      display_message(6, 1, second_half);
      return;
    }
    display_message(5, 1, message);
  }

private:
  NcurseUi() : mWindowHeight(8), mWindowWidth(50), mYstart(10), mXstart(10) {
    initscr();
    cbreak();
    noecho();
    keypad(win, TRUE);
    keypad(stdscr, TRUE);
    // set up the window
    win = newwin(mWindowHeight, mWindowWidth, mYstart, mXstart);
    box(win, 0, 0);
    curs_set(0); // Hide the cursor
    wrefresh(win);
  }

  NcurseUi(const NcurseUi &) = delete;
  NcurseUi &operator=(const NcurseUi &) = delete;

  void handleInput() {
    AppTask app;
    Event e;

    Ui.display_message(0, 5, "Productivity Timer", true);

    char _line[mWindowWidth];
    memset(_line, '-', sizeof(_line) - 3);

    Ui.display_message(4, 1, _line);
    // start the system by entering IDLE
    e.sig = INT_ENTRY_SIG;
    app.ApplicationHandler(ProtimerI, &e);

    while (running) {
      int ch = wgetch(win);
      switch (ch) {
      case 'w': { // Handle UP key
        e.sig = USER_INC_TIME_SIG;
        app.ApplicationHandler(ProtimerI, &e);
        break;
      }
      case 'z': { // Handle DOWN key
        e.sig = USER_DEC_TIME_SIG;
        app.ApplicationHandler(ProtimerI, &e);
        break;
      }
      case 'p':
      case 'P': { // Handle Pause
        // Handle UP key
        e.sig = USER_START_PAUSE_SIG;
        app.ApplicationHandler(ProtimerI, &e);

        break;
      }
      case 's':
      case 'S': {
        e.sig = USER_STATUS_SIG;
        app.ApplicationHandler(ProtimerI, &e);
        break;
      }
      case 'q':
      case 'Q': { // Handle Quit
        running = false;
        break;
      }
      default:

        break;
      }
    }
  }

  WINDOW *win;
  int mWindowHeight;
  int mWindowWidth;
  int mXstart;
  int mYstart;

  std::atomic<bool> running{true};
  // std::mutex mtx; // Mutex to protect displayMessage
  std::recursive_mutex mtx;
};

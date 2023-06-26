// The MIT License (MIT)
//
// Copyright (c) 2019 Luigi Pertoldi
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.
//
// ============================================================================
//  ___   ___   ___   __    ___   ____  __   __   ___    __    ___
// | |_) | |_) / / \ / /`_ | |_) | |_  ( (` ( (` | |_)  / /\  | |_)
// |_|   |_| \ \_\_/ \_\_/ |_| \ |_|__ _)_) _)_) |_|_) /_/--\ |_| \_
//
// Very simple progress bar for c++ loops with internal running variable
//
// Author: Luigi Pertoldi
// Created: 3 dic 2016
//
// Notes: The bar must be used when there's no other possible source of output
//        inside the for loop
//

#ifndef PROGRESS_BAR_H_
#define PROGRESS_BAR_H_

#include <iostream>
#include <ostream>
#include <string>
#include <stdexcept>

class ProgressBar {

 public:
  // default destructor
  ~ProgressBar() = default;

  // delete everything else
  ProgressBar(ProgressBar const &) = delete;
  ProgressBar &operator=(ProgressBar const &) = delete;
  ProgressBar(ProgressBar &&) = delete;
  ProgressBar &operator=(ProgressBar &&) = delete;

  // default constructor, must call set_niter later
  inline ProgressBar();
  inline ProgressBar(int n, bool showbar = true, std::ostream &out = std::cerr);

  // reset bar to use it again
  inline void Reset();
  // set number of loop iterations
  inline void SetNiter(int iter);
  // chose your style
  inline void SetDoneChar(const std::string &sym) { done_str_ = sym; }
  inline void SetTodoChar(const std::string &sym) { todo_str_ = sym; }
  inline void SetOpeningBracketStr(const std::string &sym) { opening_bracket_str_ = sym; }
  inline void SetClosingBracketStr(const std::string &sym) { closing_bracket_str_ = sym; }
  // to show only the percentage
  inline void ShowBar(bool flag = true) { do_show_bar_ = flag; }
  // set the output stream
  inline void SetOutputStream(const std::ostream &stream) { output_.rdbuf(stream.rdbuf()); }
  // main function
  inline void Update();

 private:
  int progress_;
  int n_cycles_;
  int last_perc_;
  bool do_show_bar_;
  bool update_is_called_;

  std::string done_str_;
  std::string todo_str_;
  std::string opening_bracket_str_;
  std::string closing_bracket_str_;

  std::ostream &output_;
};

inline ProgressBar::ProgressBar() :
    progress_(0),
    n_cycles_(0),
    last_perc_(0),
    do_show_bar_(true),
    update_is_called_(false),
    done_str_("#"),
    todo_str_(" "),
    opening_bracket_str_("["),
    closing_bracket_str_("]"),
    output_(std::cerr) {}

inline ProgressBar::ProgressBar(int n, bool showbar, std::ostream &out) :
    progress_(0),
    n_cycles_(n),
    last_perc_(0),
    do_show_bar_(showbar),
    update_is_called_(false),
    done_str_("#"),
    todo_str_(" "),
    opening_bracket_str_("["),
    closing_bracket_str_("]"),
    output_(out) {}

inline void ProgressBar::Reset() {
  progress_ = 0,
      update_is_called_ = false;
  last_perc_ = 0;
}

inline void ProgressBar::SetNiter(int niter) {
  if (niter <= 0)
    throw std::invalid_argument(
        "ProgressBar::SetNiter: number of iterations null or negative.");
  n_cycles_ = niter;
}

inline void ProgressBar::Update() {

  if (0 == n_cycles_)
    throw std::runtime_error(
        "ProgressBar::Update: number of cycles not set.");

  if (!update_is_called_) {
    if (true == do_show_bar_) {
      output_ << opening_bracket_str_;
      for (int _ = 0; _ < 50; _++) output_ << todo_str_;
      output_ << closing_bracket_str_ << " 0%";
    } else output_ << "0%";
  }
  update_is_called_ = true;

  int perc = 0;

  // compute percentage, if did not change, do nothing and return
  perc = progress_ * 100. / n_cycles_;
  if (perc < last_perc_) return;

  // update percentage each unit
  if (perc == last_perc_ + 1) {
    // erase the correct  number of characters
    if (perc <= 10) output_ << "\b\b" << perc << "%";
    else if (perc > 10 and perc < 100) output_ << "\b\b\b" << perc << "%";
    else if (perc == 100) output_ << "\b\b\b" << perc << "%";
  }
  if (true == do_show_bar_) {
    // update bar every ten units
    if (0 == perc % 2) {
      // erase closing bracket
      output_ << std::string(closing_bracket_str_.size(), '\b');
      // erase trailing percentage characters
      if (perc < 10) output_ << "\b\b\b";
      else if (perc >= 10 && perc < 100) output_ << "\b\b\b\b";
      else if (perc == 100) output_ << "\b\b\b\b\b";

      // erase 'todo_char_'
      for (int j = 0; j < 50 - (perc - 1) / 2; ++j) {
        output_ << std::string(todo_str_.size(), '\b');
      }

      // add one additional 'done_char_'
      if (perc == 0) output_ << todo_str_;
      else output_ << done_str_;

      // refill with 'todo_char_'
      for (int j = 0; j < 50 - (perc - 1) / 2 - 1; ++j) output_ << todo_str_;

      // readd trailing percentage characters
      output_ << closing_bracket_str_ << ' ' << perc << "%";
    }
  }
  last_perc_ = perc;
  ++progress_;
  output_ << std::flush;
}

#endif //PROGRESS_BAR_H_

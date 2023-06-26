#include "include/progress_bar.h"
#include <cstdlib>
#include <unistd.h>

int main() {
//  ProgressBar bar(10 * 5);
  ProgressBar bar(10);
  bar.SetOpeningBracketStr("Process: [");
  bar.SetDoneChar("#");
  for (int i = 0; i < 10; ++i) {
//    for (int j = 0; j < 5; ++j) {
//      bar.Update();
//    }
    bar.Update();
    usleep(50000);
    // ... the program
  }
  bar.Update();
  return 0;
}
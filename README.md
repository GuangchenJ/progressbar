# progressbar

A very simple, header-only, fully customizable, progress bar (with percentage)
for `C++` loops.

Very simple to set up:
```c++
#include "include/progress_bar.h"

int main() {
  ProgressBar bar(100);
  for (int i = 0; i < 100; ++i) {
    bar.Update();
    // ... the program
  }
  return 0;
}
```
![animated gif](.github/example-simple.gif)

Allows customization:
```c++
#include "include/progress_bar.h"

int main() {
  ProgressBar bar(100);
  bar.SetTodoChar(" ");
  bar.SetDoneChar("█");
  bar.SetOpeningBracketStr("{");
  bar.SetClosingBracketStr("}");
  for (int i = 0; i < 100; ++i) {
    bar.Update();
    // ... the program
  }
  return 0;
}
```
![animated gif](.github/example-custom.gif)

## Notes

To use the bar in parallelized loops call `ProgressBar::Update` in a critical
section. With [OpenMP](http://www.openmp.org) this can be achieved with the
following structure:
```cpp
#pragma omp parallel for
for ( ... ) {
    #pragma omp critical
        bar.Update();
}
```
When the number of loops in your program is less than 50, the following bugs may occur:

```shell
Process: [   #    #    #    #    #    #    #    #    #   #] 100%
```

Just use the `Update` function a few times, for example

```c++
#include "include/progress_bar.h"

int main() {
  int repeat_num = 5;
  ProgressBar bar(10 * repeat_num);
  bar.SetOpeningBracketStr("Process: [");
  bar.SetDoneChar("#");
  for (int i = 0; i < 10; ++i) {
    for (int j = 0; j < repeat_num; ++j) {
      bar.Update();
    }
    // ... the program
  }
  bar.Update();
  return 0;
}
```

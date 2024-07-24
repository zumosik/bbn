#include "../src/scanner.h"
#include <criterion/criterion.h>
#include <stdio.h>

Test(add, basic) {
  Scanner scanner;
  initScanner(&scanner, "var x = 4;");
  printf("Hello\n");
}

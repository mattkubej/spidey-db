#include <string.h>

// Based on the implementation in https://stackoverflow.com/a/29544416/14610143

void reverse(char s[]) {
  char c;
  int i, j;

  for (i = 0, j = strlen(s) - 1; i < j; i++, j--) {
    c = s[i];
    s[i] = s[j];
    s[j] = c;
  }
}

void itoa(int n, char s[]) {
  int sign = n;

  if (sign < 0) {
    n = -n;
  }

  int i = 0;

  do {
    s[i++] = n % 10 + '0';
  } while ((n / 10) > 0);

  if (sign < 0) {
    s[i++] = '-';
  }

  s[i] = '\0';

  reverse(s);
}


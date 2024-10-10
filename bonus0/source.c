#include <stdio.h>
#include <string.h>
#include <unistd.h>

void p(char *buffer, const char *prompt) {
  char localBuffer[4104];
  char *newlinePosition;

  puts(prompt);
  read(0, localBuffer, 0x1000);
  newlinePosition = strchr(localBuffer, '\n');
  if (newlinePosition) {
    *newlinePosition = '\0';  // Remove the newline character
  }
  strncpy(buffer, localBuffer, 20);  // Copy only the first 20 characters
}

void pp(char *result) {
  char str1[20];
  char str2[20];

  p(str1, "-");
  p(str2, "-");

  strcpy(result, str1);  // Copy first part to result
  int len = strlen(result);
  result[len] = ' ';  // Append a space between parts
  result[len + 1] = '\0';  // Null-terminate the string after the space
  strcat(result, str2);  // Concatenate the second part
}

int main() {
  char combinedResult[54];

  pp(combinedResult);
  puts(combinedResult);  // Display the combined result
  return 0;
}

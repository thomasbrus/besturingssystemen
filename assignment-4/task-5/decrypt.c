#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static int shift[132] = { 2, 15, 14, 18, 15, 9, 25, 11, 19, 7, 12, 25, 24, 2, 25, 19, 9, 21, 3, 17, 22, 9, 8, 12, 9, 3, 19, 5, 13, 1, 6, 19, 18, 22, 19, 13, 3, 15, 23, 11, 16, 3, 2, 6, 3, 23, 13, 25, 7, 21, 0, 13, 12, 16, 13, 7, 23, 9, 17, 5, 10, 23, 22, 0, 23, 17, 7, 19, 1, 15, 20, 7, 6, 10, 7, 1, 17, 3, 11, 25, 4, 17, 16, 20, 17, 11, 1, 13, 21, 9, 14, 1, 0, 4, 1, 21, 11, 23, 5, 19, 24, 11, 10, 14, 11, 5, 21, 7, 15, 3, 8, 21, 20, 24, 21, 15, 5, 17, 25, 13, 18, 5, 4, 8, 5, 25, 15, 1, 9, 23 };

int prompt(char *message, char **output) {
  size_t size;
  printf("%s", message);
  return getline(output, &size, stdin);
}

char *decrypt(char *input) {
  int i;
  char *output = malloc(strlen(input) + 1);

  for (i = 0; input[i] != '\0'; i++) {
    int ord = (int)tolower(input[i]);

    if (ord < 97 || ord > 97 + 26) {
      output[i] = input[i];
    } else {
      if ((output[i] = ord - shift[i % 130]) < 97) output[i] += 26;
    }
  }

  return output;
}

int main(int argc, char *argv[]) {
  char *encrypted_text;
  char *decrypted_text;

  while (1) {  
    if (prompt("Enter text to decrypt: ", &encrypted_text) == -1) break;
    decrypted_text = decrypt(encrypted_text);
    printf("%s", decrypted_text);
  }

  free(encrypted_text);
  free(decrypted_text);
  
  return 0;
}
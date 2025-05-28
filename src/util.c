#include <string.h>
#include <math.h>


int str_to_int(char *input) {
    int output = 0;

    int length = strlen(input);

    for (int i = 0; i < length; i++) {
        char c = input[i];

        if ( c < '0' || c > '9') {
            return -1;
        }

        int digit = ( (int)c ) - 48;
        int multiplier = pow(10, length - i - 1);

        output += digit * multiplier;
    }

    return output;
}


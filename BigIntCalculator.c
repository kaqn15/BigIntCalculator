#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define Max_Length 100000
#define Sign 1

typedef struct bigint {
    int sign, length;
    char num [Max_Length];
}bigint;

int compare_bigint(bigint *a, bigint *b);

void reverse(bigint *a);

void emptygarbage(bigint *a); 

void inputbigint(bigint *a);

int isoperator(int ch); 

void printbigint(bigint *a);

bigint addition(bigint *a, bigint *b, int sign); 

bigint subtraction(bigint *a, bigint *b, int sign); 

bigint multibyonedigit(bigint *a, char ch); 

bigint multiplication(bigint *a, bigint *b, int sign);

bigint division(bigint *a, bigint *b, int sign);

int main() {
    bigint a, b, result;
    char op;
    printf("Enter first number: ");
    inputbigint(&a);
    printf("Enter second number: ");
    inputbigint(&b);
    printf("Enter operator: ");
    scanf("%c", &op);
    if (op == '+') result = addition(&a, &b, 1);
    if (op == '-') result = subtraction(&a, &b, 1);
    if (op == '*') result = multiplication(&a, &b, a.sign * b.sign);
    if (op == '/') result = division(&a, &b, a.sign * b.sign);
    printbigint(&result);
    return 0;
}

int compare_bigint(bigint *a, bigint *b) {
    if (a->length > b->length) return 1;
    else if (a->length < b->length) return -1;
    else {
        for (int i = 0; i < a->length; i++) {
            if (a->num[i] > b->num[i]) return 1;
            else if (a->num[i] < b->num[i]) return -1;
        }
        return 0;
	}
}

void reverse(bigint *a) {
    for (int start = 0, end = a->length - 1; start < end; start++, end--) { 
        int temp = a->num[start]; 
        a->num[start] = a->num[end]; 
        a->num[end] = temp;
    }
}

void emptygarbage(bigint *a){
    for (int i = a->length; i < Max_Length; i++) {
        a->num[i] = '0';
    }
}

int isoperator(int ch) {
    if (ch == '+') return 1;
    else if (ch == '-') return 2;
    else if (ch == '*') return 3;
    else if (ch == '/') return 4;
    else if (ch == '\n') return -1;
}

void inputbigint(bigint *a) {
    int ch, n = 0;
    a->sign = 1;

    // Check for the sign
    ch = getchar();
    if (ch == '-') {
        a->sign = -1;
        ch = getchar(); // Read the next character after the '-'
    }

    // Skip leading zeros
    while (ch == '0') {
        ch = getchar();
    }

    while (ch != '\n' && n < Max_Length) {
       
        a->num[n] = ch;
        n++;
        ch = getchar();
    }

    a->length = n;
    emptygarbage(a);
}

void printbigint(bigint *a) {
    if (a->sign == -1 && !(a->length == 1 && a->num[0] == '0')) {
        printf("-");
    }

    int leadingZeros = 1;
    for (int i = 0; i < a->length; i++) {
        if (a->num[i] != '0') {
            leadingZeros = 0;
        }

        if (!leadingZeros) {
            putchar(a->num[i]);
        }
    }

    if (leadingZeros) {
        putchar('0');
    }
}

bigint addition(bigint *a, bigint *b, int sign) {
    if (a->sign == -1 && b->sign == -1) {
        // Both inputs are negative
        a->sign = b->sign = 1;
        bigint result = addition(a, b, sign);
        result.sign = -sign;  // Add the negative sign to the result
        return result;
    }

    if (a->sign == -1) {
        // If the first number is negative, make it positive and perform subtraction
        a->sign = 1;
        return subtraction(b, a, sign);
    }

    if (b->sign == -1) {
        // If the second number is negative, make it positive and perform subtraction
        b->sign = 1;
        return subtraction(a, b, sign);
    }

    if (compare_bigint(a, b) == -1) {
        // Swap a and b to ensure a is greater than or equal to b
        bigint temp = *a;
        *a = *b;
        *b = temp;
    }

    reverse(a);
    reverse(b);
    char adder = '0';
    int i = 0;
    bigint result;

    // Perform addition
    while (i < a->length || adder != '0') {
        int temp = (a->num[i] - '0') + (i < b->length ? b->num[i] - '0' : 0) + (adder - '0');
        result.num[i] = temp % 10 + '0';
        adder = temp / 10 + '0';
        i++;
    }

    if (adder != '0') {
        result.num[i] = adder;
        i++;
    }

    result.length = i;
    result.sign = sign;
    reverse(&result);
    emptygarbage(&result);
    reverse(a);
    reverse(b);

    return result;
}

bigint subtraction(bigint *a, bigint *b, int sign) {
    bigint result;
    result.length = 0;
    
    if (a->sign == -1 && b->sign == -1) {
        // If both numbers are negative, make the second input positive and operate addition with the first negative number.
        b->sign = 1;
        return addition(b, a, sign);
    }

    if (a->sign == -1) {
        // If the first number is negative, make it positive and perform addition with the second number then add "-" in front of the output
        a->sign = 1;
        result = addition(a, b, sign);
        result.sign = -sign;  // Result should have the negative sign.
        return result;
    }

    if (b->sign == -1) {
        // If the second number is negative, make it positive and perform addition.
        b->sign = 1;
        result = addition(a, b, sign);
        result.sign = sign;  // Result should have the positive sign.
        return result;
    }

  	if (compare_bigint(a, b) == -1) {
        bigint result = subtraction(b, a, -sign);
        result.sign = -sign;
        return result;
    }  
    
    // Normal subtraction when both numbers are positive.
    reverse(a);
    reverse(b);
    char borrow = '0';
    int i = 0;

    while (i < a->length || borrow != '0') {
        int temp = (a->num[i] - '0') - (i < b->length ? b->num[i] - '0' : 0) - (borrow - '0');
        if (temp < 0) {
            temp += 10;
            borrow = '1';
        } else {
            borrow = '0';
        }
        result.num[i] = temp + '0';
        i++;
    }

    while (i > 1 && result.num[i - 1] == '0') {
        i--;
    }

    result.length = i;
    result.sign = sign;
    reverse(&result);
    emptygarbage(&result);
    reverse(a);
    reverse(b);

    return result;
}

bigint multibyonedigit(bigint *a, char ch) {
    bigint result;
    result.length = 0;
    emptygarbage(&result);

    reverse(a);

    char adder = '0';
    int i = 0;

    while (i < a->length || adder != '0') {
        result.num[i] = ((a->num[i] - '0') * (ch - '0') + (adder - '0')) % 10 + '0';
        adder = ((a->num[i] - '0') * (ch - '0') + (adder - '0')) / 10 + '0';
        i++;
    }
	
    result.length = i;
    result.sign = 1;
    reverse(a);
    reverse(&result);
    emptygarbage(&result);

    return result;
}

bigint multiplication(bigint *a, bigint *b, int sign) {
    bigint result;
    result.length = 0;

    a->sign = b->sign = 1;

    reverse(b);
    emptygarbage(&result);

    for (int i = 0; i < b->length; i++) {
        char ch = b->num[i];
        bigint carry = multibyonedigit(a, ch);
        carry.length += i;
        result = addition(&carry, &result, 1);  
    }

    reverse(b);
    emptygarbage(&result);
    result.sign = sign;

    return result;
}

bigint division(bigint *a, bigint *b, int sign) {
    bigint result;
    result.length = 0;

    // Determine the sign of the result based on the signs of both operands
    result.sign = 1;

    emptygarbage(&result);

    if ((a->sign == -1 && b->sign == -1) || (a->sign == 1 && b->sign == 1)) {
        // If both operands are negative or both are positive, make them positive
        a->sign = b->sign = 1;
    } else {
        // If only one operand is negative, make it positive
        a->sign = b->sign = 1;
        sign = -1;
    }

    if (b->num[0] == '0') {
        printf("Error: Division by zero is not allowed.\n");
        exit(EXIT_FAILURE);  // Terminate the program with an error indication
    }

    int counter = b->length;
    b->length = a->length;

	while (b->length >= counter) {
        while (compare_bigint(a, b) >= 0) {
            *a = subtraction(a, b, 1);
            result.num[result.length]++;
        }
        result.length++;
        b->length--;
    }

    result.sign = sign;  // Set the correct sign
    return result;
}

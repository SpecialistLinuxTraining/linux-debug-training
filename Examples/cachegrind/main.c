#include <stdio.h>

void function1(int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int x = i * j;
        }
    }
}

void function2(int n) {
    int vec[n];
    for (int i = 0; i < n; i++) {
        vec[i] = i * i;
    }
}

void function3(int n) {
    for (int i = 0; i < n; i++) {
        function1(i);
        function2(i);
    }
}

int main() {
    int n = 100;
    for (int i = 0; i < 10; i++) {
        function3(n);
    }
    return 0;
}

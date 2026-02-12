#include "Student.h"

// Implementation of Student methods
int Student::getTotal(int t) {
    int total = 0;
    for (int i = 0; i < 5; i++) total += termMarks[t][i];
    return total;
}

float Student::getPercentage(int t) {
    return (getTotal(t) / 500.0f) * 100.0f;
}

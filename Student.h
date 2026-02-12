#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <iostream>

using namespace std;

class Subject {
public:
    string name;
    int marks;
};

class Student {
public:
    int id;
    string fname;
    string lname;
    string semester;
    string year;
    string joinDate;
    int roll;
    string subNames[5]; 
    Subject subjects[5];
    int termMarks[3][5];
    int attendance;

    // Function Prototypes
    int getTotal(int t);
    float getPercentage(int t);
};

#endif

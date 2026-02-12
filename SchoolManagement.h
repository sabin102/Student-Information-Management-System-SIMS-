#ifndef SCHOOLMANAGEMENT_H
#define SCHOOLMANAGEMENT_H

#include "Student.h"
#include "Teacher.h"
#include <fstream>
#include <string>
#include <vector>

using namespace std;

struct User {
    char username[50];
    char password[50];
    char role[20];
    char assignedSubject[30];
};

class SchoolManagement {
public:
    // Validation Helpers
    bool isAlphabetic(string str);
    string getValidString(string prompt);
    int getValidInt(string prompt, int min = -2147483647, int max = 2147483647);
    
    // Auth Helpers
    string formatName(string str);
    string toLower(string str);
    string getSecurePassword();
    void changePassword(string username);
    void addUser(string user, string pass, string role, string subj = "All");
    void login();

    // ID Helpers
    int getNextTeacherID();
    int getNextStudentID();

    // Teacher Management
    void saveTeacher(Teacher t);
    void viewTeachers();
    void searchTeacher();
    void editTeacher();
    void deleteTeacher();
    void teacherMenu(string username);

    // Student Management
    void saveStudent(Student s);
    void viewStudents();
    void searchStudent();
    void editStudent();
    void deleteStudent();
    void updateMarks(string currentTeacherUser = "", string role = "admin");
    void updateAttendance();
    void studentMenu(string username);

    // Admin
    void adminMenu(string user);
};

#endif

#include "SchoolManagement.h"
#include <iostream>
#include <fstream>

using namespace std;

int main() {
    SchoolManagement sms;
    
    // Ensure admin exists
    ifstream check("users.dat"); 
    if (!check.is_open()) sms.addUser("admin", "admin", "admin");
    check.close();

    int choice;
    do {
        system("cls");
        cout << "=== STUDENT INFORMATION SYSTEM ===\n";
        cout << "1. Login\n2. Exit\nEnter Choice: ";
        if (!(cin >> choice)) { 
            cin.clear(); cin.ignore(1000, '\n'); continue;
        }
        if (choice == 1) sms.login();
    } while (choice != 2);
    
    return 0;
}

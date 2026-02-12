#include "SchoolManagement.h"
#include <iostream>
#include <iomanip>
#include <conio.h> // Windows only
#include <cstdio>
#include <cstdlib>
#include <cctype>

using namespace std;

// --- Helper & Validation Functions ---

bool SchoolManagement::isAlphabetic(string str) {
    for (char c : str) {
        if (!isalpha(c)) return false;
    }
    return true;
}

string SchoolManagement::getValidString(string prompt) {
    string value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (isAlphabetic(value)) return value;
        else cout << "\t\t\t\t[Error]: Text only (No Numbers/Symbols).\n";
    }
}

int SchoolManagement::getValidInt(string prompt, int min, int max) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            if (value >= min && value <= max) return value;
            else cout << "\t\t\t\tInvalid! Enter between " << min << " and " << max << ".\n";
        } else {
            cout << "\t\t\t\tInvalid Input! Enter a number.\n";
            cin.clear(); cin.ignore(1000, '\n');
        }
    }
}

string SchoolManagement::formatName(string str) {
    if (str.empty()) return "";
    str[0] = toupper(str[0]);
    for (size_t i = 1; i < str.length(); i++) str[i] = tolower(str[i]);
    return str;
}

string SchoolManagement::toLower(string str) {
    for (size_t i = 0; i < str.length(); i++) str[i] = tolower(str[i]);
    return str;
}

string SchoolManagement::getSecurePassword() {
    string pass = "";
    char ch;
    while ((ch = _getch()) != 13) { // 13 is Enter
        if (ch == 8) { // 8 is Backspace
            if (pass.length() > 0) {
                pass.erase(pass.length() - 1);
                cout << "\b \b";
            }
        } else {
            pass.push_back(ch);
            cout << '*';
        }
    }
    cout << endl;
    return pass;
}

// --- File & ID Functions ---

int SchoolManagement::getNextTeacherID() {
    ifstream fin("teachers.txt");
    Teacher t;
    int lastID = 0;
    while (fin >> t.id >> t.fname >> t.lname >> t.subject >> t.teachClass >> t.joinDate) {
        lastID = t.id;
    }
    fin.close();
    return lastID + 1;
}

int SchoolManagement::getNextStudentID() {
    ifstream fin("students.txt");
    if (!fin.is_open()) return 1;
    Student s;
    int lastID = 0;
    while (fin >> s.id >> s.fname >> s.lname >> s.semester >> s.year >> s.joinDate >> s.roll) {
        for (int i = 0; i < 5; i++) fin >> s.subNames[i];
        for (int t = 0; t < 3; t++) for (int i = 0; i < 5; i++) fin >> s.termMarks[t][i];
        fin >> s.attendance;
        lastID = s.id;
    }
    fin.close();
    return lastID + 1;
}

void SchoolManagement::addUser(string user, string pass, string role, string subj) {
    User u;
    snprintf(u.username, 50, "%s", user.c_str());
    snprintf(u.password, 50, "%s", pass.c_str());
    snprintf(u.role, 20, "%s", role.c_str());
    snprintf(u.assignedSubject, 30, "%s", subj.c_str());

    ofstream fout("users.dat", ios::binary | ios::app);
    fout.write((char*)&u, sizeof(User));
    fout.close();
}

void SchoolManagement::changePassword(string username) {
    system("cls");
    cout << "\n\t\t=== CHANGE PASSWORD ===\n";
    string oldPass, newPass;
    cout << "\t\tEnter Current Password: ";
    oldPass = getSecurePassword();

    fstream file("users.dat", ios::binary | ios::in | ios::out);
    User u;
    bool found = false;
    while (file.read((char*)&u, sizeof(User))) {
        if (string(u.username) == username && string(u.password) == oldPass) {
            cout << "\t\tEnter New Password: ";
            newPass = getSecurePassword();
            snprintf(u.password, 50, "%s", newPass.c_str());
            int pos = -1 * sizeof(User);
            file.seekp(pos, ios::cur);
            file.write((char*)&u, sizeof(User));
            found = true;
            cout << "\n\t\tPassword updated successfully!";
            break;
        }
    }
    if (!found) cout << "\n\t\tInvalid current password!";
    file.close();
    _getch();
}

// --- Teacher Implementation ---

void SchoolManagement::saveTeacher(Teacher t) {
    if (t.id == 0) t.id = getNextTeacherID();
    ofstream fout("teachers.txt", ios::app);
    fout << t.id << " " << t.fname << " " << t.lname << " " << t.subject << " " << t.teachClass << " " << t.joinDate << endl;
    fout.close();
    addUser(t.fname, "teacher", "teacher", t.subject);
    cout << "\n\t\t\t\tTeacher Added Successfully!\n";
}

void SchoolManagement::viewTeachers() {
    ifstream fin("teachers.txt");
    if (!fin.is_open()) { cout << "No teachers found.\n"; return; }
    Teacher t;
    system("cls");
    cout << "\nID   First Name      Last Name       Subject             Class\n";
    cout << "====================================================================\n";
    while (fin >> t.id >> t.fname >> t.lname >> t.subject >> t.teachClass >> t.joinDate) {
        cout << left << setw(5) << t.id << setw(16) << t.fname << setw(16) << t.lname 
             << setw(20) << t.subject << setw(10) << t.teachClass << endl;
    }
    fin.close();
    cout << "\nPress enter to back!!";
}

void SchoolManagement::searchTeacher() {
    system("cls");
    ifstream fin("teachers.txt");
    int key;
    cout << "Enter Teacher ID: "; cin >> key;
    Teacher t;
    bool found = false;
    while (fin >> t.id >> t.fname >> t.lname >> t.subject >> t.teachClass >> t.joinDate) {
        if (t.id == key) {
            cout << "\nFound: " << t.fname << " " << t.lname << " (" << t.subject << ")\n";
            found = true; break;
        }
    }
    if (!found) cout << "Not Found.\n";
    fin.close();
}

void SchoolManagement::editTeacher() {
    system("cls");
    int key;
    cout << "Enter ID to edit: "; cin >> key;
    ifstream fin("teachers.txt"); ofstream fout("temp.txt");
    Teacher t; bool found = false;
    while (fin >> t.id >> t.fname >> t.lname >> t.subject >> t.teachClass >> t.joinDate) {
        if (t.id == key) {
            found = true;
            cout << "Editing " << t.fname << "...\n";
            t.fname = getValidString("New First Name: ");
            t.lname = getValidString("New Last Name: ");
            t.subject = getValidString("New Subject: ");
            t.teachClass = getValidString("New Class: ");
        }
        fout << t.id << " " << t.fname << " " << t.lname << " " << t.subject << " " << t.teachClass << " " << t.joinDate << endl;
    }
    fin.close(); fout.close();
    remove("teachers.txt"); rename("temp.txt", "teachers.txt");
    if(found) cout << "Updated.\n"; else cout << "Not found.\n";
}

void SchoolManagement::deleteTeacher() {
    system("cls");
    int id;
    cout << "Enter ID to delete: "; cin >> id;
    ifstream fin("teachers.txt"); ofstream fout("temp.txt");
    Teacher t; bool found = false;
    while (fin >> t.id >> t.fname >> t.lname >> t.subject >> t.teachClass >> t.joinDate) {
        if (t.id == id) { found = true; continue; }
        fout << t.id << " " << t.fname << " " << t.lname << " " << t.subject << " " << t.teachClass << " " << t.joinDate << endl;
    }
    fin.close(); fout.close();
    remove("teachers.txt"); rename("temp.txt", "teachers.txt");
    if(found) cout << "Deleted.\n"; else cout << "Not found.\n";
}

// --- Student Implementation ---

void SchoolManagement::saveStudent(Student s) {
    if (s.id == 0) s.id = getNextStudentID();
    ofstream fout("students.txt", ios::app);
    fout << s.id << " " << s.fname << " " << s.lname << " " << s.semester << " " << s.year << " " << s.joinDate << " " << s.roll << endl;
    for (int i = 0; i < 5; i++) fout << s.subNames[i] << " ";
    fout << endl;
    for (int t = 0; t < 3; t++) {
        for (int i = 0; i < 5; i++) fout << s.termMarks[t][i] << " ";
        fout << endl;
    }
    fout << s.attendance << endl;
    fout.close();
    addUser(s.fname, s.fname, "student");
    cout << "\nStudent Added! Login: " << s.fname << " / " << s.fname << endl;
}

void SchoolManagement::viewStudents() {
    system("cls");
    ifstream fin("students.txt");
    if (!fin.is_open()) { cout << "No students.\n"; return; }
    Student s;
    cout << "ID   Name                 Class   Roll   Attendance\n";
    cout << "===================================================\n";
    while (fin >> s.id >> s.fname >> s.lname >> s.semester >> s.year >> s.joinDate >> s.roll) {
        for (int i = 0; i < 5; i++) fin >> s.subNames[i];
        for (int t = 0; t < 3; t++) for (int i = 0; i < 5; i++) fin >> s.termMarks[t][i];
        fin >> s.attendance;
        cout << left << setw(5) << s.id << setw(21) << (s.fname + " " + s.lname) 
             << setw(8) << s.semester << setw(7) << s.roll << s.attendance << "%" << endl;
    }
    fin.close();
    cout << "\nPress Enter...";
}

void SchoolManagement::searchStudent() {
    system("cls");
    ifstream fin("students.txt");
    int id; cout << "Enter Student ID: "; cin >> id;
    Student s; bool found = false;
    while (fin >> s.id >> s.fname >> s.lname >> s.semester >> s.year >> s.joinDate >> s.roll) {
        for (int i = 0; i < 5; i++) fin >> s.subNames[i];
        for (int t = 0; t < 3; t++) for (int i = 0; i < 5; i++) fin >> s.termMarks[t][i];
        fin >> s.attendance;
        if (s.id == id) {
            found = true;
            cout << "Found: " << s.fname << " " << s.lname << " (Class: " << s.semester << ")\n";
        }
    }
    if (!found) cout << "Not Found.\n";
    fin.close();
    cout << "Press Enter...";
}

void SchoolManagement::editStudent() {
    system("cls");
    int key; cout << "Enter ID to edit: "; cin >> key;
    ifstream fin("students.txt"); ofstream fout("temp.txt");
    Student s; bool found = false;
    while (fin >> s.id >> s.fname >> s.lname >> s.semester >> s.year >> s.joinDate >> s.roll) {
        for (int i = 0; i < 5; i++) fin >> s.subNames[i];
        for (int t = 0; t < 3; t++) for (int i = 0; i < 5; i++) fin >> s.termMarks[t][i];
        fin >> s.attendance;
        if (s.id == key) {
            found = true;
            s.fname = getValidString("New First Name: ");
            s.lname = getValidString("New Last Name: ");
            s.semester = getValidString("New Class: ");
            cout << "New Roll: "; cin >> s.roll;
        }
        fout << s.id << " " << s.fname << " " << s.lname << " " << s.semester << " " << s.year << " " << s.joinDate << " " << s.roll << endl;
        for (int i = 0; i < 5; i++) fout << s.subNames[i] << " ";
        fout << endl;
        for (int t = 0; t < 3; t++) {
            for (int i = 0; i < 5; i++) fout << s.termMarks[t][i] << " ";
            fout << endl;
        }
        fout << s.attendance << endl;
    }
    fin.close(); fout.close();
    remove("students.txt"); rename("temp.txt", "students.txt");
    if(found) cout << "Updated.\n"; else cout << "Not found.\n";
}

void SchoolManagement::deleteStudent() {
    system("cls");
    int id; cout << "Enter ID to delete: "; cin >> id;
    ifstream fin("students.txt"); ofstream fout("temp.txt");
    Student s; bool found = false;
    while (fin >> s.id >> s.fname >> s.lname >> s.semester >> s.year >> s.joinDate >> s.roll) {
        for (int i = 0; i < 5; i++) fin >> s.subNames[i];
        for (int t = 0; t < 3; t++) for (int i = 0; i < 5; i++) fin >> s.termMarks[t][i];
        fin >> s.attendance;
        if (s.id == id) { found = true; continue; }
        fout << s.id << " " << s.fname << " " << s.lname << " " << s.semester << " " << s.year << " " << s.joinDate << " " << s.roll << endl;
        for (int i = 0; i < 5; i++) fout << s.subNames[i] << " ";
        fout << endl;
        for (int t = 0; t < 3; t++) {
            for (int i = 0; i < 5; i++) fout << s.termMarks[t][i] << " ";
            fout << endl;
        }
        fout << s.attendance << endl;
    }
    fin.close(); fout.close();
    remove("students.txt"); rename("temp.txt", "students.txt");
    if(found) cout << "Deleted.\n"; else cout << "Not found.\n";
}

void SchoolManagement::updateMarks(string currentTeacherUser, string role) {
    system("cls");
    int id, term;
    string subjectToEdit = "";
    if (role == "teacher") {
        ifstream ufin("users.dat", ios::binary);
        User u;
        while (ufin.read((char*)&u, sizeof(User))) {
            if (toLower(u.username) == toLower(currentTeacherUser)) {
                subjectToEdit = u.assignedSubject; break;
            }
        }
        ufin.close();
        cout << "Logged in as Teacher. Subject: " << subjectToEdit << endl;
    }

    cout << "Enter Student ID: "; cin >> id;
    cout << "Enter Term (1-3): "; cin >> term; term--;
    if (term < 0 || term > 2) return;

    ifstream fin("students.txt"); ofstream fout("temp.txt");
    Student s; bool found = false;
    while (fin >> s.id >> s.fname >> s.lname >> s.semester >> s.year >> s.joinDate >> s.roll) {
        for (int i = 0; i < 5; i++) fin >> s.subNames[i];
        for (int t = 0; t < 3; t++) for (int i = 0; i < 5; i++) fin >> s.termMarks[t][i];
        fin >> s.attendance;
        if (s.id == id) {
            found = true;
            for (int i = 0; i < 5; i++) {
                if (role == "admin" || toLower(s.subNames[i]) == toLower(subjectToEdit)) {
                    cout << "Enter marks for " << s.subNames[i] << ": ";
                    cin >> s.termMarks[term][i];
                }
            }
        }
        fout << s.id << " " << s.fname << " " << s.lname << " " << s.semester << " " << s.year << " " << s.joinDate << " " << s.roll << endl;
        for (int i = 0; i < 5; i++) fout << s.subNames[i] << " ";
        fout << endl;
        for (int t = 0; t < 3; t++) {
            for (int i = 0; i < 5; i++) fout << s.termMarks[t][i] << " ";
            fout << endl;
        }
        fout << s.attendance << endl;
    }
    fin.close(); fout.close();
    remove("students.txt"); rename("temp.txt", "students.txt");
    if(found) cout << "Updated.\n"; else cout << "Not Found.\n";
    if(role=="teacher") { cin.ignore(); cin.get(); }
}

void SchoolManagement::updateAttendance() {
    system("cls");
    int id; cout << "Enter ID: "; cin >> id;
    ifstream fin("students.txt"); ofstream fout("temp.txt");
    Student s; bool found = false;
    while (fin >> s.id >> s.fname >> s.lname >> s.semester >> s.year >> s.joinDate >> s.roll) {
        for (int i = 0; i < 5; i++) fin >> s.subNames[i];
        for (int t = 0; t < 3; t++) for (int i = 0; i < 5; i++) fin >> s.termMarks[t][i];
        fin >> s.attendance;
        if (s.id == id) {
            found = true;
            cout << "Enter Attendance %: "; cin >> s.attendance;
        }
        fout << s.id << " " << s.fname << " " << s.lname << " " << s.semester << " " << s.year << " " << s.joinDate << " " << s.roll << endl;
        for (int i = 0; i < 5; i++) fout << s.subNames[i] << " ";
        fout << endl;
        for (int t = 0; t < 3; t++) {
            for (int i = 0; i < 5; i++) fout << s.termMarks[t][i] << " ";
            fout << endl;
        }
        fout << s.attendance << endl;
    }
    fin.close(); fout.close();
    remove("students.txt"); rename("temp.txt", "students.txt");
    if(found) cout << "Updated.\n";
}

// --- Menus ---

void SchoolManagement::adminMenu(string user) {
    int ch;
    do {
        system("cls");
        cout << "1. Add Teacher\n2. View Teachers\n3. Manage Teacher\n4. Add Student\n5. View Students\n6. Manage Student\n7. Update Records\n8. Change Password\n9. Logout\nChoice: ";
        cin >> ch;
        if (ch == 1) {
            Teacher t; t.id = 0;
            t.fname = getValidString("First Name: ");
            t.lname = getValidString("Last Name: ");
            cout << "Join Date: "; cin >> t.joinDate;
            t.subject = getValidString("Subject: ");
            t.teachClass = getValidString("Class: ");
            saveTeacher(t);
            cin.ignore(); cin.get();
        } else if (ch == 2) { viewTeachers(); cin.ignore(); cin.get(); }
        else if (ch == 3) {
            cout << "1. Search 2. Edit 3. Delete: "; int s; cin >> s;
            if (s == 1) searchTeacher(); else if (s == 2) editTeacher(); else if (s == 3) deleteTeacher();
            cin.ignore(); cin.get();
        } else if (ch == 4) {
            Student s; s.id = 0;
            s.fname = getValidString("First Name: ");
            s.lname = getValidString("Last Name: ");
            s.semester = getValidString("Class: ");
            s.year = getValidString("Year: ");
            cout << "Join Date: "; cin >> s.joinDate;
            cout << "Roll: "; cin >> s.roll;
            for(int i=0; i<5; i++) s.subNames[i] = getValidString("Subject " + to_string(i+1) + ": ");
            for(int t=0; t<3; t++) for(int i=0; i<5; i++) s.termMarks[t][i] = 0;
            s.attendance = 0;
            saveStudent(s);
            cin.ignore(); cin.get();
        } else if (ch == 5) { viewStudents(); cin.ignore(); cin.get(); }
        else if (ch == 6) {
            cout << "1. Search 2. Edit 3. Delete: "; int s; cin >> s;
            if (s == 1) searchStudent(); else if (s == 2) editStudent(); else if (s == 3) deleteStudent();
            cin.ignore(); cin.get();
        } else if (ch == 7) {
            cout << "1. Marks 2. Attendance: "; int s; cin >> s;
            if (s == 1) updateMarks(); else if (s == 2) updateAttendance();
            cin.ignore(); cin.get();
        } else if (ch == 8) { changePassword(user); }
    } while (ch != 9);
}

void SchoolManagement::teacherMenu(string username) {
    int ch;
    do {
        system("cls");
        cout << "1. View Students\n2. Update Marks\n3. Update Attendance\n4. Search Student\n5. Logout\nChoice: ";
        cin >> ch;
        if (ch == 1) viewStudents();
        else if (ch == 2) updateMarks(username, "teacher");
        else if (ch == 3) updateAttendance();
        else if (ch == 4) searchStudent();
        if (ch != 5) { cin.ignore(); cin.get(); }
    } while (ch != 5);
}

void SchoolManagement::studentMenu(string username) {
    int termChoice;
    do {
        system("cls");
        ifstream fin("students.txt");
        Student s; bool found = false;
        while (fin >> s.id >> s.fname >> s.lname >> s.semester >> s.year >> s.joinDate >> s.roll) {
            for (int i = 0; i < 5; i++) fin >> s.subNames[i];
            for (int t = 0; t < 3; t++) for (int i = 0; i < 5; i++) fin >> s.termMarks[t][i];
            fin >> s.attendance;
            if (s.fname == username) {
                found = true;
                cout << "Welcome " << s.fname << "\n1. Term 1\n2. Term 2\n3. Term 3\n4. Logout\nChoice: ";
                cin >> termChoice;
                if (termChoice >= 1 && termChoice <= 3) {
                    int t = termChoice - 1;
                    system("cls");
                    cout << "Results for Term " << termChoice << endl;
                    for (int i = 0; i < 5; i++) cout << s.subNames[i] << ": " << s.termMarks[t][i] << endl;
                    cout << "Total: " << s.getTotal(t) << " Percentage: " << s.getPercentage(t) << "%" << endl;
                    cout << "Attendance: " << s.attendance << "%\nPress Enter...";
                    cin.ignore(); cin.get();
                }
                break;
            }
        }
        fin.close();
        if (!found) break;
    } while (termChoice != 4);
}

void SchoolManagement::login() {
    system("cls");
    string user, pass;
    cout << "Username: "; cin >> user;
    cout << "Password: "; pass = getSecurePassword();

    ifstream fin("users.dat", ios::binary);
    if (!fin) {
         ofstream fout("users.dat", ios::binary);
         User u;
         snprintf(u.username, 50, "admin"); snprintf(u.password, 50, "admin");
         snprintf(u.role, 20, "admin"); snprintf(u.assignedSubject, 30, "All");
         fout.write((char*)&u, sizeof(User)); fout.close();
         fin.open("users.dat", ios::binary);
    }
    User u; bool matched = false;
    while (fin.read(reinterpret_cast<char*>(&u), sizeof(User))) {
        if (string(u.username) == user && string(u.password) == pass) {
            matched = true;
            string role = u.role;
            if (role == "admin") adminMenu(user);
            else if (role == "teacher") teacherMenu(user);
            else if (role == "student") studentMenu(user);
            break;
        }
    }
    if (!matched) { cout << "\nInvalid Login!"; cin.ignore(); cin.get(); }
    fin.close();
}

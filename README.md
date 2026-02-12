# 🎓 Student Information Management System (SIMS)

A robust, console-based application built in **C++** designed to streamline the management of academic records. This system facilitates the efficient handling of student data, teacher records, grades, and attendance through a secure, role-based interface.

---

## 🚀 Features

### 🔐 Security & Access Control
*   **Role-Based Login:** Distinct dashboards for **Admins**, **Teachers**, and **Students**.
*   **Secure Authentication:** Password masking (hidden input) during login.
*   **Data Persistence:** All records (users, students, teachers) are saved permanently in local files (`.txt` and `.dat`), ensuring no data loss upon exit.

### 🛠 Administrative Capabilities
*   **CRUD Operations:** Create, Read, Update, and Delete records for both Students and Teachers.
*   **Input Validation:** Ensures names and subjects contain only alphabetic characters, preventing data corruption.
*   **Auto-Incrementing IDs:** Automatically generates unique Primary Keys for students and teachers to maintain database integrity.

### 📚 Academic Management
*   **Grade Management:** Teachers can assign and update marks for specific terms.
*   **Attendance Tracking:** Record and update student attendance percentages.
*   **Report Generation:** Students can view their own term-wise results, totals, and percentages.

---

## 📂 Project Structure

The project follows a **modular architecture**, separating data objects from business logic for better maintainability.

```text
SIMS/
├── main.cpp                # Entry point of the application
├── SchoolManagement.h      # Header file for system logic and menus
├── SchoolManagement.cpp    # Implementation of system logic
├── Student.h               # Header file for Student data structure
├── Student.cpp             # Implementation of Student methods
├── Teacher.h               # Header file for Teacher data structure
├── Teacher.cpp             # Implementation of Teacher methods
└── README.md               # Project documentation

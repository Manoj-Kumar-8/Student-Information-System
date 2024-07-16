#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <mutex>
#include <thread>
using namespace std;
// Structs to represent Student, Course, and Enrollment
struct Student {
    int id;
    string name;
    int age;
    string department;
};
struct Course {
    int id;
    string name;
    int credits;
};
struct Enrollment {
    int student_id;
    int course_id;
};
// Global vectors to store data
vector<Student> students;
vector<Course> courses;
vector<Enrollment> enrollments;
// Mutexes to protect shared data
mutex mtx_students;
mutex mtx_courses;
mutex mtx_enrollments;
// Function prototypes
void display_welcome_message();
void add_student();
void add_course();
void enroll_student();
void print_students();
void print_courses();
void print_enrollments();
int get_int_input(const string& prompt);
string get_string_input(const string& prompt);
void press_enter_to_continue();
void display_options();
// Welcome message and problem statement
void display_welcome_message() {
    cout << "===================================================" << endl;
    cout << "Welcome to VNRVJIET Student Information System!" << endl;
    cout << "===================================================" << endl << endl;

    cout << "This system allows you to manage student records, course information, and enrollments." << endl;
}
// Function to add a new student
void add_student() {
    int id = get_int_input("Enter student ID: ");
    string name = get_string_input("Enter student name: ");
    int age = get_int_input("Enter student age: ");
    string department = get_string_input("Enter student department: ");
    lock_guard<mutex> lock(mtx_students);
    students.push_back({id, name, age, department});
    cout << "Student added successfully." << endl;
}
// Function to add a new course
void add_course() {
    int id = get_int_input("Enter course ID: ");
    string name = get_string_input("Enter course name: ");
    int credits = get_int_input("Enter course credits: ");
    lock_guard<mutex> lock(mtx_courses);
    courses.push_back({id, name, credits});
    cout << "Course added successfully." << endl;
}
// Function to enroll a student in a course
void enroll_student() {
    int student_id = get_int_input("Enter student ID: ");
    int course_id = get_int_input("Enter course ID: ");
    // Using std::lock to lock multiple mutexes in a deadlock-free manner
    unique_lock<mutex> lock1(mtx_students, defer_lock);
    unique_lock<mutex> lock2(mtx_courses, defer_lock);
    unique_lock<mutex> lock3(mtx_enrollments, defer_lock);
    std::lock(lock1, lock2, lock3);
    enrollments.push_back({student_id, course_id});
    cout << "Student with ID " << student_id << " enrolled in course with ID " << course_id << "." << endl;
}
// Function to print all students
void print_students() {
    lock_guard<mutex> lock(mtx_students);
    if (students.empty()) {
        cout << "No student data available." << endl;
        return;
    }
    cout << "Printing all students:" << endl;
    for (const auto& student : students) {
        cout << "ID: " << student.id << ", Name: " << student.name
             << ", Age: " << student.age << ", Department: " << student.department << endl;
    }
}
// Function to print all courses
void print_courses() {
    lock_guard<mutex> lock(mtx_courses);
    if (courses.empty()) {
        cout << "No course data available." << endl;
        return;
    }
    cout << "Printing all courses:" << endl;
    for (const auto& course : courses) {
        cout << "ID: " << course.id << ", Name: " << course.name
             << ", Credits: " << course.credits << endl;
    }
}
// Function to print all enrollments
void print_enrollments() {
    lock_guard<mutex> lock(mtx_enrollments);
    if (enrollments.empty()) {
        cout << "No enrollment data available." << endl;
        return;
    }
    cout << "Printing all enrollments:" << endl;
    for (const auto& enrollment : enrollments) {
        cout << "Student ID: " << enrollment.student_id << ", Course ID: " << enrollment.course_id << endl;
    }
}
// Function to get integer input from user
int get_int_input(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        string input;
        getline(cin, input);
        stringstream ss(input);
        if (ss >> value && ss.eof()) {
            break;
        } else {
            cout << "Invalid input. Please enter a valid integer." << endl;
        }
    }
    return value;
}
// Function to get string input from user
string get_string_input(const string& prompt) {
    string input;
    while (true) {
        cout << prompt;
        getline(cin, input);
        if (!input.empty()) {
            break;
        } else {
            cout << "Invalid input. Please enter a valid string." << endl;
        }
    }
    return input;
}
// Function to pause and wait for user to press enter
void press_enter_to_continue() {
    cout << "Press Enter to continue...";
    cin.ignore(); // Clear the newline from input buffer
    cin.get();    // Wait for Enter key
}
// Function to display options
void display_options() {
    cout << endl;
    cout << "Choose an operation (1-7):" << endl;
    cout << "1. Add Student" << endl;
    cout << "2. Add Course" << endl;
    cout << "3. Enroll Student" << endl;
    cout << "4. Print Students" << endl;
    cout << "5. Print Courses" << endl;
    cout << "6. Print Enrollments" << endl;
    cout << "7. Exit" << endl;
}
// Main function
int main() {
    display_welcome_message();
    while (true) {
        display_options();
        int choice = get_int_input("Enter your choice: ");
        switch (choice) {
            case 1:
                add_student();
                break;
            case 2:
                add_course();
                break;
            case 3:
                enroll_student();
                break;
            case 4:
                print_students();
                break;
            case 5:
                print_courses();
                break;
            case 6:
                print_enrollments();
                break;
            case 7:
                cout << "Exiting program. Goodbye!" << endl;
                return 0;
            default:
                cout << "Invalid choice. Please choose an operation from 1 to 7." << endl;
                break;
        }
    }
    return 0;
}

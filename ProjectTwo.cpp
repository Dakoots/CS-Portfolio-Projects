#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

class Course {
public:
    string id;
    string title;
    vector<string> prereqs;

    Course(const string &num, const string &name, const vector<string> &prerequisites)
        : id(num), title(name), prereqs(prerequisites) {}
};

string trimWhitespace(const string &str) {
    size_t start = str.find_first_not_of(" \t");
    if (start == string::npos) return "";
    size_t end = str.find_last_not_of(" \t");
    return str.substr(start, end - start + 1);
}

bool loadCoursesFromFile(const string &filename, vector<Course> &courses) {
    ifstream inputFile(filename);
    if (!inputFile) {
        cerr << "Error: Unable to open " << filename << "\n";
        return false;
    }

    string line;
    while (getline(inputFile, line)) {
        stringstream ss(line);
        string token;
        vector<string> parts;
        while (getline(ss, token, ',')) {
            parts.push_back(trimWhitespace(token));
        }
        
        if (parts.size() < 2) continue;
        
        string courseId = parts[0];
        string courseTitle = parts[1];
        vector<string> prerequisites(parts.begin() + 2, parts.end());
        
        courses.emplace_back(courseId, courseTitle, prerequisites);
    }
    inputFile.close();
    cout << "Course data successfully loaded.\n";
    return true;
}

void displayCourseList(const vector<Course> &courses) {
    if (courses.empty()) {
        cout << "No courses available. Load data first.\n";
        return;
    }
    
    vector<Course> sortedCourses = courses;
    sort(sortedCourses.begin(), sortedCourses.end(), [](const Course &a, const Course &b) {
        return a.id < b.id;
    });

    cout << "\nAvailable Courses:\n";
    for (const auto &course : sortedCourses) {
        cout << course.id << " - " << course.title << "\n";
    }
    cout << endl;
}

void displayCourseDetails(const vector<Course> &courses, const string &courseId) {
    for (const auto &course : courses) {
        if (course.id == courseId) {
            cout << "\nCourse ID: " << course.id << "\n";
            cout << "Course Title: " << course.title << "\n";
            cout << "Prerequisites: ";
            if (course.prereqs.empty()) {
                cout << "None\n";
            } else {
                for (size_t i = 0; i < course.prereqs.size(); i++) {
                    cout << course.prereqs[i] << (i < course.prereqs.size() - 1 ? ", " : "");
                }
                cout << "\n";
            }
            return;
        }
    }
    cout << "Course " << courseId << " not found.\n";
}

int main() {
    vector<Course> courses;
    bool dataLoaded = false;
    int choice;
    string filename;

    do {
        cout << "\n---- Menu ----\n";
        cout << "1: Load courses from file\n";
        cout << "2: Display all courses\n";
        cout << "3: View course details\n";
        cout << "9: Exit\n";
        cout << "Select an option: ";
        cin >> choice;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        switch (choice) {
            case 1:
                courses.clear();
                cout << "Enter file name: ";
                cin >> filename;
                dataLoaded = loadCoursesFromFile(filename);
                break;
            case 2:
                if (!dataLoaded) {
                    cout << "Load data first.\n";
                } else {
                    displayCourseList(courses);
                }
                break;
            case 3:
                if (!dataLoaded) {
                    cout << "Load data first.\n";
                } else {
                    string courseQuery;
                    cout << "Enter course ID: ";
                    cin >> courseQuery;
                    displayCourseDetails(courses, courseQuery);
                }
                break;
            case 9:
                cout << "Goodbye!\n";
                break;
            default:
                cout << "Invalid option. Try again.\n";
        }
    } while (choice != 9);

    return 0;
}
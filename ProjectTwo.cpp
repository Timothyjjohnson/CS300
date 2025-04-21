// Project Two by: Timothy Johnson
// Date: April 17th, 2025
// CS300
// Professor Rissover

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

// course structure
struct Course {
    string courseID;
    string courseTitle;
    vector<string> prerequisites;
};

// bst node
struct Node {
    Course course;
    Node* left;
    Node* right;

    Node(Course aCourse) : course(aCourse), left(nullptr), right(nullptr) {}
};

// coursebst class
class CourseBST {
private:
    Node* root;

    void addNode(Node*& node, Course course) {
        if (node == nullptr) {
            node = new Node(course);
        }
        else if (course.courseID < node->course.courseID) {
            addNode(node->left, course);
        }
        else {
            addNode(node->right, course);
        }
    }

    void inOrder(Node* node) {
        if (node == nullptr) {
            return;
        }
        inOrder(node->left);
        cout << node->course.courseID << ", " << node->course.courseTitle << endl;
        inOrder(node->right);
    }

    Node* search(Node* node, string courseID) {
        if (node == nullptr || node->course.courseID == courseID) {
            return node;
        }
        if (courseID < node->course.courseID) {
            return search(node->left, courseID);
        }
        else {
            return search(node->right, courseID);
        }
    }

public:
    CourseBST() : root(nullptr) {}

    void Insert(Course course) {
        addNode(root, course);
    }

    void PrintCourseList() {
        inOrder(root);
    }

    Course* Search(string courseID) {
        Node* foundNode = search(root, courseID);
        if (foundNode != nullptr) {
            return &foundNode->course;
        }
        return nullptr;
    }
};

// load course data function
void LoadCourses(const string& fileName, CourseBST& catalog) {
    ifstream file(fileName);

    if (!file.is_open()) {
        cout << "Error opening file: " << fileName << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string courseID, courseTitle, prereq;
        vector<string> prereqs;

        getline(ss, courseID, ',');
        getline(ss, courseTitle, ',');

        while (getline(ss, prereq, ',')) {
            prereqs.push_back(prereq);
        }

        Course course = { courseID, courseTitle, prereqs };
        catalog.Insert(course);
    }

    file.close();
    cout << "Course data loaded successfully" << endl;
}

// display course info
void DisplayCourse(Course course) {
    cout << course.courseID << ", " << course.courseTitle << endl;
    if (!course.prerequisites.empty()) {
        cout << "Prerequisites: ";
        for (size_t i = 0; i < course.prerequisites.size(); ++i) {
            cout << course.prerequisites[i];
            if (i < course.prerequisites.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }
}

// main program
int main() {
    CourseBST catalog;
    int choice = 0;
    string fileName = "CS 300 ABCU_Advising_Program_Input.csv";

    while (choice != 4) {
        cout << endl;
        cout << "Menu:" << endl;
        cout << "  1. Load Data Structure" << endl;
        cout << "  2. Print Course List" << endl;
        cout << "  3. Print Course" << endl;
        cout << "  4. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            LoadCourses(fileName, catalog);
            break;
        case 2:
            catalog.PrintCourseList();
            break;
        case 3: {
            cout << "Enter course ID (example: CS101): ";
            string courseID;
            cin >> courseID;
            transform(courseID.begin(), courseID.end(), courseID.begin(), ::toupper);

            Course* course = catalog.Search(courseID);
            if (course != nullptr) {
                DisplayCourse(*course);
            }
            else {
                cout << "Course not found" << endl;
            }
            break;
        }
        case 4:
            cout << "Goodbye" << endl;
            break;
        default:
            cout << "Invalid choice. Please select 1-4." << endl;
            break;
        }
    }

    return 0;
}

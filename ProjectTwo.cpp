// Project Two by: Timothy Johnson
// Date: April 16th, 2025
// CS300
// Professor Rissover

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <algorithm>
#include <cctype>

using namespace std;

// class to hold info about a course
class Course {
public:
    string courseID;
    string courseTitle;
    vector<string> prerequisites;

    Course(string id, string title, vector<string> prereqs) {
        courseID = id;
        courseTitle = title;
        prerequisites = prereqs;
    }
};

// function to make all letters uppercase to match courses
string toUpper(string str) {
    for (char& ch : str) {
        ch = toupper(ch);
    }
    return str;
}

// load the course catalog from the file
vector<Course> importCourseCatalog(string fileName) {
    vector<Course> courseLibrary;
    set<string> existingCourseIDs;
    vector<vector<string>> rawPrereqs;

    ifstream file(fileName);

    if (!file.is_open()) {
        cout << "Oops! Could not locate the course data file. Please check the file path." << endl;
        return courseLibrary;
    }

    if (file.peek() == ifstream::traits_type::eof()) {
        cout << "Warning: The course file is empty. No courses to load." << endl;
        return courseLibrary;
    }

    cout << "Loading course catalog... Please hold on." << endl;

    string line;
    // first pass: store courseID and courseTitle
    while (getline(file, line)) {
        vector<string> courseData;
        stringstream ss(line);
        string item;

        while (getline(ss, item, ',')) {
            courseData.push_back(item);
        }

        if (courseData.size() < 2) {
            cout << "Warning: This course entry is formatted incorrectly -> " << line << endl;
            continue;
        }

        string courseID = toUpper(courseData[0]);
        string courseTitle = courseData[1];

        existingCourseIDs.insert(courseID);
        rawPrereqs.push_back(courseData);

        Course newCourse(courseID, courseTitle, {});
        courseLibrary.push_back(newCourse);
    }

    // second pass: add valid prereqs for each course
    for (size_t i = 0; i < courseLibrary.size(); ++i) {
        for (size_t j = 2; j < rawPrereqs[i].size(); ++j) {
            string prereq = toUpper(rawPrereqs[i][j]);
            if (existingCourseIDs.find(prereq) != existingCourseIDs.end()) {
                courseLibrary[i].prerequisites.push_back(prereq);
            }
            else {
                cout << "Error: Prerequisite " << prereq << " not found. Double-check the course numbers!" << endl;
            }
        }
    }

    file.close();

    cout << "Course catalog loaded successfully! A total of " << courseLibrary.size() << " courses have been loaded." << endl;
    return courseLibrary;
}

// print all courses in order by course ID
void PrintAlphanumCourses(vector<Course> courseLibrary) {
    cout << "Sorting courses by course number..." << endl;

    sort(courseLibrary.begin(), courseLibrary.end(), [](Course a, Course b) {
        return a.courseID < b.courseID;
        });

    for (Course course : courseLibrary) {
        cout << "Course ID: " << course.courseID << endl;
        cout << "Course Title: " << course.courseTitle << endl;
    }
}

// show full course details when user searches
void findCourseDetails(vector<Course> courseLibrary, string searchID) {
    bool found = false;
    cout << "Looking for course: " << searchID << " ... Let me check!" << endl;

    for (Course course : courseLibrary) {
        if (course.courseID == searchID) {
            cout << "----- COURSE DETAILS -----" << endl;
            cout << "Course Number: " << course.courseID << endl;
            cout << "Course Title: " << course.courseTitle << endl;

            if (course.prerequisites.empty()) {
                cout << "Prerequisites: None. You can take this course as a standalone!" << endl;
            }
            else {
                cout << "Prerequisites:" << endl;
                for (string prereq : course.prerequisites) {
                    cout << " - " << prereq << endl;
                }
            }
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Hmm... That course isn’t in the system. Double-check the course number and try again!" << endl;
    }
}

// display the menu and handle user input
void showMenu(vector<Course>& courseLibrary) {
    int userChoice = 0;
    string searchID;

    while (userChoice != 9) {
        cout << "Welcome to the ABCU Course Catalog System!" << endl;
        cout << "Please choose one of the following options:" << endl;
        cout << "1: Load the course data into the system" << endl;
        cout << "2: Print all courses in alphanumeric order" << endl;
        cout << "3: Get details for a specific course" << endl;
        cout << "9: Exit the program" << endl;
        cout << "Enter your choice: ";
        cin >> userChoice;
        cin.ignore();

        switch (userChoice) {
        case 1:
            cout << "Loading course data into the system..." << endl;
            courseLibrary = importCourseCatalog("course_data.txt");
            break;

        case 2:
            cout << "Sorting and displaying courses in order..." << endl;
            PrintAlphanumCourses(courseLibrary);
            break;

        case 3:
            cout << "Enter the course ID to view details: ";
            getline(cin, searchID);
            findCourseDetails(courseLibrary, toUpper(searchID));
            break;

        case 9:
            cout << "Exiting... Goodbye!" << endl;
            break;

        default:
            cout << "Invalid choice. Please try again." << endl;
        }

        cout << endl;
    }
}

// start the program
int main() {
    vector<Course> courseLibrary;
    showMenu(courseLibrary);
    return 0;
}

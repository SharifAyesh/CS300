//============================================================================
// Name        : Project Two 7-1
// Author      : Sharif Ayesh
// Version     : 1.0
// Copyright   : Copyright © 2025 SNHU
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Windows.h>

using namespace std;

const int GLOBAL_SLEEP_TIME = 3000;

struct Course {
    string courseId;
    string courseName;
    vector<string> preList;
};

class BinarySearchTree {
private:
    struct Node {
        Course course;
        Node* right;
        Node* left;
        Node() {
            left = nullptr;
            right = nullptr;
        }
        Node(Course aCourse) {
            course = aCourse;
            left = nullptr;
            right = nullptr;
        }
    };

    Node* root;
    void inOrder(Node* node);
    void clear(Node* node);
    int size = 0;

public:
    BinarySearchTree();
    ~BinarySearchTree();
    void InOrder();
    void Insert(Course aCourse);
    void Remove(string courseId);
    Course Search(string courseId);
    int Size();
    void Clear();
};

BinarySearchTree::BinarySearchTree() {
    root = nullptr;
}

BinarySearchTree::~BinarySearchTree() {
    Clear();
}

void BinarySearchTree::Clear() {
    clear(root);
    root = nullptr;
    size = 0;
}

void BinarySearchTree::clear(Node* node) {
    if (node == nullptr) return;
    clear(node->left);
    clear(node->right);
    delete node;
}

void BinarySearchTree::InOrder() {
    inOrder(root);
}

void BinarySearchTree::Insert(Course aCourse) {
    Node* currentNode = root;

    if (root == NULL) {
        root = new Node(aCourse);
    }
    else {
        while (currentNode != NULL) {
            if (aCourse.courseId < currentNode->course.courseId) {
                if (currentNode->left == nullptr) {
                    currentNode->left = new Node(aCourse);
                    break;
                }
                else {
                    currentNode = currentNode->left;
                }
            }
            else {
                if (currentNode->right == nullptr) {
                    currentNode->right = new Node(aCourse);
                    break;
                }
                else {
                    currentNode = currentNode->right;
                }
            }
        }
    }
    size++;
}

void BinarySearchTree::Remove(string courseId) {
    
}

Course BinarySearchTree::Search(string courseId) {
    Course aCourse;
    Node* currentNode = root;

    while (currentNode != NULL) {
        if (currentNode->course.courseId == courseId) {
            return currentNode->course;
        }
        else if (courseId < currentNode->course.courseId) {
            currentNode = currentNode->left;
        }
        else {
            currentNode = currentNode->right;
        }
    }

    return aCourse;
}

void BinarySearchTree::inOrder(Node* node) {
    if (node == NULL) return;
    inOrder(node->left);
    cout << node->course.courseId << ", " << node->course.courseName << endl;
    inOrder(node->right);
}

int BinarySearchTree::Size() {
    return size;
}

vector<string> Split(string lineFeed) {
    char delim = ',';
    lineFeed += delim;
    vector<string> lineTokens;
    string temp = "";

    for (int i = 0; i < lineFeed.length(); i++) {
        if (lineFeed[i] == delim) {
            lineTokens.push_back(temp);
            temp = "";
            i++;
        }
        temp += lineFeed[i];
    }
    return lineTokens;
}

void loadCourses(string csvPath, BinarySearchTree* courseList) {
    ifstream inFS;
    string line;
    vector<string> stringTokens;

    cout << "\nAttempting to open file: " << csvPath << endl;

    inFS.open(csvPath);

    if (!inFS.is_open()) {
        cout << "Could not open file. Please check the filename or path." << endl;
        return;
    }

    while (getline(inFS, line)) {
        if (line.empty()) continue;

        Course aCourse;
        stringTokens = Split(line);

        if (stringTokens.size() < 2) {
            cout << "Skipping malformed line: " << line << endl;
        }
        else {
            aCourse.courseId = stringTokens.at(0);
            aCourse.courseName = stringTokens.at(1);

            for (unsigned int i = 2; i < stringTokens.size(); i++) {
                aCourse.preList.push_back(stringTokens.at(i));
            }

            courseList->Insert(aCourse);
        }
    }

    inFS.close();
    cout << "\nFile loaded successfully. " << courseList->Size() << " course(s) inserted.\n" << endl;
}

void displayCourse(Course aCourse) {
    cout << aCourse.courseId << ", " << aCourse.courseName << endl;
    cout << "Prerequisites: ";

    if (aCourse.preList.empty()) {
        cout << "none" << endl;
    }
    else {
        for (unsigned int i = 0; i < aCourse.preList.size(); i++) {
            cout << aCourse.preList.at(i);
            if (aCourse.preList.size() > 1 && i < aCourse.preList.size() - 1) {
                cout << ", ";
            }
        }
        cout << endl;
    }
}

void convertCase(string& toConvert) {
    for (unsigned int i = 0; i < toConvert.length(); i++) {
        if (isalpha(toConvert[i])) {
            toConvert[i] = toupper(toConvert[i]);
        }
    }
}

int main() {
    string csvPath;
    string aCourseKey;
    BinarySearchTree* courseList = new BinarySearchTree();
    Course course;
    int choice = 0;

    cout << "Welcome to the course planner.\n" << endl;
    cout << "Please enter the file name to load (courses.csv): ";
    getline(cin, csvPath);

    courseList->Clear(); // clear before loading
    loadCourses(csvPath, courseList);

    while (choice != 9) {
        cout << "\n1. Load Data Structure." << endl;
        cout << "2. Print Course List." << endl;
        cout << "3. Print Course." << endl;
        cout << "9. Exit\n" << endl;

        cout << "What would you like to do? ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
        case 1:
            cout << "\nEnter file name to load: ";
            getline(cin, csvPath);
            courseList->Clear(); // clear before loading again
            loadCourses(csvPath, courseList);
            Sleep(GLOBAL_SLEEP_TIME);
            break;

        case 2:
            cout << "\nHere is a sample schedule:\n" << endl;
            courseList->InOrder();
            break;

        case 3:
            cout << "\nWhat course do you want to know about? ";
            cin >> aCourseKey;
            convertCase(aCourseKey);
            course = courseList->Search(aCourseKey);

            if (!course.courseId.empty()) {
                cout << endl;
                displayCourse(course);
            }
            else {
                cout << "\nCourse ID " << aCourseKey << " not found." << endl;
            }
            Sleep(GLOBAL_SLEEP_TIME);
            break;

        case 9:
            break;

        default:
            cout << choice << " is not a valid option.\n" << endl;
            Sleep(GLOBAL_SLEEP_TIME);
        }
    }

    cout << "\nThank you for using the course planner!" << endl;
    Sleep(GLOBAL_SLEEP_TIME);
    return 0;
}

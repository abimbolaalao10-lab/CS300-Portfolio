#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

/*
    ============================================================================
                                COURSE STRUCT
    ============================================================================
*/

struct Course {
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;
};

/*
    ============================================================================
                        BINARY SEARCH TREE FOR COURSE STORAGE
    ============================================================================
*/

class BinarySearchTree {

private:

    // Node structure for BST
    struct Node {
        Course course;
        Node* left;
        Node* right;

        Node(Course c) {
            course = c;
            left = nullptr;
            right = nullptr;
        }
    };

    Node* root;

    /*
        Insert node into BST recursively
    */
    Node* insert(Node* node, Course course) {
        if (node == nullptr) {
            return new Node(course);
        }

        if (course.courseNumber < node->course.courseNumber) {
            node->left = insert(node->left, course);
        }
        else {
            node->right = insert(node->right, course);
        }

        return node;
    }

    /*
        Search recursively for a course
    */
    Course search(Node* node, string courseNumber) {
        Course emptyCourse;

        if (node == nullptr) {
            return emptyCourse;
        }

        if (node->course.courseNumber == courseNumber) {
            return node->course;
        }

        if (courseNumber < node->course.courseNumber) {
            return search(node->left, courseNumber);
        }
        else {
            return search(node->right, courseNumber);
        }
    }

    /*
        In-order traversal prints all courses alphanumerically
    */
    void inOrder(Node* node) {
        if (node == nullptr) return;
        inOrder(node->left);
        cout << node->course.courseNumber << ": " << node->course.courseTitle << endl;
        inOrder(node->right);
    }

    /*
        Free all nodes
    */
    void destroy(Node* node) {
        if (node != nullptr) {
            destroy(node->left);
            destroy(node->right);
            delete node;
        }
    }

public:

    BinarySearchTree() {
        root = nullptr;
    }

    ~BinarySearchTree() {
        destroy(root);
    }

    void Insert(Course course) {
        root = insert(root, course);
    }

    Course Search(string courseNumber) {
        return search(root, courseNumber);
    }

    void PrintInOrder() {
        inOrder(root);
    }
};

/*
    ============================================================================
                        FUNCTION TO PARSE CSV FILE AND LOAD DATA
    ============================================================================
*/

vector<string> split(const string& line, char delimiter) {
    vector<string> tokens;
    string token;
    stringstream ss(line);

    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

void loadCourses(string fileName, BinarySearchTree &bst) {
    ifstream file(fileName);

    if (!file.is_open()) {
        cout << "*** ERROR: Could not open file. Verify file name. ***" << endl;
        return;
    }

    string line;

    while (getline(file, line)) {
        if (line.length() == 0) continue;

        vector<string> tokens = split(line, ',');

        if (tokens.size() < 2) {
            cout << "*** ERROR: Invalid line format. Skipping line. ***" << endl;
            continue;
        }

        Course course;
        course.courseNumber = tokens[0];
        course.courseTitle = tokens[1];

        for (size_t i = 2; i < tokens.size(); i++) {
            course.prerequisites.push_back(tokens[i]);
        }

        bst.Insert(course);
    }

    cout << "Courses successfully loaded into the Binary Search Tree." << endl;

    file.close();
}

/*
    ============================================================================
                        PRINT A SINGLE COURSE'S DETAILS
    ============================================================================
*/

void printCourseDetails(BinarySearchTree &bst) {
    string courseNumber;
    cout << "Enter course number: ";
    cin >> courseNumber;

    Course c = bst.Search(courseNumber);

    if (c.courseNumber == "") {
        cout << "*** Course not found. ***" << endl;
        return;
    }

    cout << endl;
    cout << c.courseNumber << ": " << c.courseTitle << endl;

    if (c.prerequisites.size() == 0) {
        cout << "Prerequisites: None" << endl;
    }
    else {
        cout << "Prerequisites: ";
        for (size_t i = 0; i < c.prerequisites.size(); i++) {
            cout << c.prerequisites[i];
            if (i < c.prerequisites.size() - 1) cout << ", ";
        }
        cout << endl;
    }
}

/*
    ============================================================================
                        MAIN MENU AND PROGRAM LOOP
    ============================================================================
*/

int main() {

    BinarySearchTree bst;
    bool dataLoaded = false;
    int choice = 0;
    string fileName;

    cout << "===== ABCU COURSE ADVISING PROGRAM =====" << endl;

    while (choice != 9) {

        cout << endl;
        cout << "1. Load Data File" << endl;
        cout << "2. Print Course List (A-Z)" << endl;
        cout << "3. Print Course Details" << endl;
        cout << "9. Exit" << endl;
        cout << "Enter choice: ";
        cin >> choice;
        cout << endl;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "*** Invalid input. Try again. ***" << endl;
            continue;
        }

        switch (choice) {

        case 1:
            cout << "Enter file name (example: CS 300 ABCU_Advising_Program_Input.csv): ";
            cin >> fileName;
            loadCourses(fileName, bst);
            dataLoaded = true;
            break;

        case 2:
            if (!dataLoaded) {
                cout << "*** Please load the file first. ***" << endl;
            }
            else {
                bst.PrintInOrder();
            }
            break;

        case 3:
            if (!dataLoaded) {
                cout << "*** Please load the file first. ***" << endl;
            }
            else {
                printCourseDetails(bst);
            }
            break;

        case 9:
            cout << "Goodbye!" << endl;
            break;

        default:
            cout << "*** Invalid option. Try again. ***" << endl;
        }
    }

    return 0;
}

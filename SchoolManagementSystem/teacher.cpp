#include "users.h"

Teacher :: Teacher(const string &userId = "", const string &userName = "", Course* c = nullptr) : User(userId, userName), courses(c)
{
    if (courses == nullptr) 
    {
        courses = new Course[1];  // Initialize with default values
        courses[0].Name = -1;
    }
}

Teacher :: ~Teacher()
{
    delete[] courses;
}

void Teacher :: viewProfile(ostream &os) const 
{
    os << "\n========== Teacher Profile ==========\n";
    os << "ID: " << id << endl;
    os << "Name: " << name << endl;

    // Read teachers data
    ifstream teachersFile("teachers.json");
    if (!teachersFile.is_open()) {
        os << "Error: Unable to open teachers.json" << endl;
        return;
    }
    json teachersData;
    teachersFile >> teachersData;
    teachersFile.close();

    // Check if teacher has any assigned courses
    auto teacherIt = find_if(teachersData["teachers"].begin(), teachersData["teachers"].end(),
                             [this](const json& teacher) { return teacher["id"] == this->id; });

    if (teacherIt == teachersData["teachers"].end() || !teacherIt->contains("courses") || teacherIt->at("courses").empty()) {
        os << "Courses: Not assigned to any courses.\n";
    } else {
        os << "\nAssigned Courses:\n";
        os << "----------------------------------------------------\n";
        os << "Course Code\tCourse Name\n";
        os << "----------------------------------------------------\n";

        for (const auto& course : teacherIt->at("courses")) {
            string courseCode = course["Code"];
            
            // Read courses data to get course name
            ifstream coursesFile("courses.json");
            json coursesData;
            coursesFile >> coursesData;
            coursesFile.close();

            auto courseIt = find_if(coursesData["courses"].begin(), coursesData["courses"].end(),
                                    [&courseCode](const json& c) { return c["code"] == courseCode; });

            string courseName = (courseIt != coursesData["courses"].end()) ? courseIt->at("name") : "Unknown";

            os << courseCode << "\t\t" << courseName << "\n";
        }

        os << "----------------------------------------------------\n";
        os << "Total Courses: " << teacherIt->at("courses").size() << endl;
    }

    os << "\n======================================\n";
}

void Teacher :: updateAttendance(const string& studentId, const string& courseCode) {
    // Read teachers data
    ifstream teachersFile("teachers.json");
    if (!teachersFile.is_open()) {
        cout << "Error: Unable to open teachers.json" << endl;
        return;
    }
    json teachersData;
    teachersFile >> teachersData;
    teachersFile.close();

    // Check if the teacher is handling any courses
    auto teacherIt = find_if(teachersData["teachers"].begin(), teachersData["teachers"].end(),
                             [this](const json& teacher) { return teacher["id"] == this->id; });

    if (teacherIt == teachersData["teachers"].end() || !teacherIt->contains("courses") || teacherIt->at("courses").empty()) {
        cout << "Error: You are not assigned to any courses." << endl;
        return;
    }

    // Verify if the teacher is handling the entered course
    auto courseIt = find_if(teacherIt->at("courses").begin(), teacherIt->at("courses").end(),
                            [&courseCode](const json& course) { return course["Code"] == courseCode; });

    if (courseIt == teacherIt->at("courses").end()) {
        cout << "Error: You are not authorized to update attendance for this course." << endl;
        return;
    }

    // Read enrollments data
    ifstream enrollmentsFile("enrollments.json");
    if (!enrollmentsFile.is_open()) {
        cout << "Error: Unable to open enrollments.json" << endl;
        return;
    }
    json enrollmentsData;
    enrollmentsFile >> enrollmentsData;
    enrollmentsFile.close();

    // Check if the student is enrolled in the course
    if (!enrollmentsData["enrollments"].contains(studentId) ||
        find_if(enrollmentsData["enrollments"][studentId].begin(),
                enrollmentsData["enrollments"][studentId].end(),
                [&courseCode](const json& enrollment) {
                    return enrollment["courseCode"] == courseCode;
                }) == enrollmentsData["enrollments"][studentId].end()) {
        cout << "Error: Student is not enrolled in this course." << endl;
        return;
    }

    int totalClasses, absents;

    // Get total classes and absents from teacher
    cout << "Enter total number of classes: ";
    cin >> totalClasses;
    cout << "Enter number of absents: ";
    cin >> absents;

    // Validate input
    if (totalClasses <= 0) {
        cout << "Error: Total classes must be greater than 0." << endl;
        return;
    }
    if (absents < 0 || absents > totalClasses) {
        cout << "Error: Invalid number of absents." << endl;
        return;
    }

    // Calculate attendance percentage
    float attendancePercentage = ((float)(totalClasses - absents) / totalClasses) * 100;

    // Round to two decimal places
    attendancePercentage = round(attendancePercentage * 100.0) / 100.0;

    // Update the attendance
    for (auto& enrollment : enrollmentsData["enrollments"][studentId]) {
        if (enrollment["courseCode"] == courseCode) {
            enrollment["attendance"] = attendancePercentage;
            break;
        }
    }

    // Write updated data back to file
    ofstream outFile("enrollments.json");
    if (!outFile.is_open()) {
        cout << "Error: Unable to open enrollments.json for writing" << endl;
        return;
    }
    outFile << setw(4) << enrollmentsData << endl;
    outFile.close();

    cout << "Attendance updated successfully. New attendance: " << attendancePercentage << "%" << endl;
}

void Teacher :: updateGrade(const string& studentId, const string& courseCode) {
    json teachersData, enrollmentsData;

    // Read teachers data
    ifstream teachersFile("teachers.json");
    if (!teachersFile.is_open()) {
        cout << "Error: Unable to open teachers.json for reading!" << endl;
        return;
    }
    teachersFile >> teachersData;
    teachersFile.close();

    // Read enrollments data
    ifstream enrollmentsFile("enrollments.json");
    if (!enrollmentsFile.is_open()) {
        cout << "Error: Unable to open enrollments.json for reading!" << endl;
        return;
    }
    enrollmentsFile >> enrollmentsData;
    enrollmentsFile.close();

    // Verify teacher's authorization
    auto teacherIt = find_if(teachersData["teachers"].begin(), teachersData["teachers"].end(),
                             [this](const json& teacher) { return teacher["id"] == this->id; });

    if (teacherIt == teachersData["teachers"].end() || !teacherIt->contains("courses")) {
        cout << "Error: You are not authorized to update grades." << endl;
        return;
    }

    // Check if the teacher is assigned to the course
    auto courseIt = find_if(teacherIt->at("courses").begin(), teacherIt->at("courses").end(),
                            [&courseCode](const json& course) { return course["Code"] == courseCode; });

    if (courseIt == teacherIt->at("courses").end()) {
        cout << "Error: You are not authorized to update grades for this course." << endl;
        return;
    }

    // Check if the student is enrolled in the course
    if (!enrollmentsData["enrollments"].contains(studentId)) {
        cout << "Error: Student not found." << endl;
        return;
    }

    auto enrollmentIt = find_if(enrollmentsData["enrollments"][studentId].begin(),
                                enrollmentsData["enrollments"][studentId].end(),
                                [&courseCode](const json& enrollment) {
                                    return enrollment["courseCode"] == courseCode;
                                });

    if (enrollmentIt == enrollmentsData["enrollments"][studentId].end()) {
        cout << "Error: Student is not enrolled in this course." << endl;
        return;
    }

    // Get grades from teacher
    float grade;
    cout << "\nEnter the new grade for the student (0-100): ";
    cin >> grade;

    if (grade < 0 || grade > 100) {
        cout << "Error: Invalid grade. Must be between 0 and 100." << endl;
        return;
    }

    // Update the grade
    (*enrollmentIt)["grade"] = grade;

    // Write updated data back to file
    ofstream outFile("enrollments.json");
    if (!outFile.is_open()) {
        cout << "Error: Unable to open enrollments.json for writing!" << endl;
        return;
    }
    outFile << enrollmentsData.dump(4);
    outFile.close();

    cout << "\nGrade updated successfully!" << endl;
    cout << "New Grade: " << grade << endl;
}

void Teacher :: viewProfile() const {
    viewProfile(cout);  // Default to console output
}

ostream& operator<<(ostream& os, const Teacher &teacher) {
    teacher.viewProfile(os);
    return os;
}

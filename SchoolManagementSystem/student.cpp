#include "users.h"

void Student :: viewAttendances() const {
    // Read courses data
    ifstream coursesFile("courses.json");
    if (!coursesFile.is_open()) {
        cout << "Error: Unable to open courses.json" << endl;
        return;
    }
    json coursesData;
    coursesFile >> coursesData;
    coursesFile.close();

    // Read enrollments data
    ifstream enrollmentsFile("enrollments.json");
    if (!enrollmentsFile.is_open()) {
        cout << "Error: Unable to open enrollments.json" << endl;
        return;
    }
    json enrollmentsData;
    enrollmentsFile >> enrollmentsData;
    enrollmentsFile.close();

    if (!enrollmentsData["enrollments"].contains(id)) {
        cout << "You are not enrolled in any courses." << endl;
        return;
    }

    cout << "\nAttendance Report for " << name << " (ID: " << id << ")" << endl;
    cout << "----------------------------------------------------" << endl;
    cout << "Course Code\tCourse Name\t\tAttendance" << endl;
    cout << "----------------------------------------------------" << endl;

    float totalAttendance = 0.0f;
    int courseCount = 0;

    for (const auto& enrollment : enrollmentsData["enrollments"][id]) {
        string courseCode = enrollment["courseCode"];
        float attendance = enrollment["attendance"];

        // Find course name
        string courseName = "Unknown Course";
        for (const auto& course : coursesData["courses"]) {
            if (course["code"] == courseCode) {
                courseName = course["name"];
                break;
            }
        }

        // Format output
        cout << courseCode << "\t\t";
        if (courseName.length() < 8) {
            cout << courseName << "\t\t\t";
        } else if (courseName.length() < 16) {
            cout << courseName << "\t\t";
        } else {
            cout << courseName << "\t";
        }
        cout << attendance << "%" << endl;

        totalAttendance += attendance;
        courseCount++;
    }

    float overallAttendance = (courseCount > 0) ? (totalAttendance / courseCount) : 0.0f;
    cout << "----------------------------------------------------" << endl;
    cout << "Overall Attendance: " << overallAttendance << "%" << endl << endl;
}

void Student :: viewGrades() const {
    // Read courses data
    ifstream coursesFile("courses.json");
    if (!coursesFile.is_open()) {
        cout << "Error: Unable to open courses.json" << endl;
        return;
    }
    json coursesData;
    coursesFile >> coursesData;
    coursesFile.close();

    // Read enrollments data
    ifstream enrollmentsFile("enrollments.json");
    if (!enrollmentsFile.is_open()) {
        cout << "Error: Unable to open enrollments.json" << endl;
        return;
    }
    json enrollmentsData;
    enrollmentsFile >> enrollmentsData;
    enrollmentsFile.close();

    if (!enrollmentsData["enrollments"].contains(id)) {
        cout << "You are not enrolled in any courses." << endl;
        return;
    }

    cout << "\nGrade Report for " << name << " (ID: " << id << ")" << endl;
    cout << "----------------------------------------------------" << endl;
    cout << "Course Code\tCourse Name\t\tGrade" << endl;
    cout << "----------------------------------------------------" << endl;

    float totalGrade = 0.0f;
    int courseCount = 0;

    for (const auto& enrollment : enrollmentsData["enrollments"][id]) {
        string courseCode = enrollment["courseCode"];
        float grade = enrollment["grade"];

        // Find course name
        string courseName = "Unknown Course";
        for (const auto& course : coursesData["courses"]) {
            if (course["code"] == courseCode) {
                courseName = course["name"];
                break;
            }
        }

        // Format output
        cout << courseCode << "\t\t";
        if (courseName.length() < 8) {
            cout << courseName << "\t\t\t";
        } else if (courseName.length() < 16) {
            cout << courseName << "\t\t";
        } else {
            cout << courseName << "\t";
        }
        cout << grade << endl;

        totalGrade += grade;
        courseCount++;
    }

    float gpa = (courseCount > 0) ? (totalGrade / courseCount) : 0.0f;
    cout << "----------------------------------------------------" << endl;

    gpa = round(gpa) / 100.0;
    cout << "GPA: " << gpa << endl << endl;

}

void Student :: viewProfile() const {
    cout << "\n========== Student Profile ==========\n";
    cout << "ID: " << id << endl;
    cout << "Name: " << name << endl;

    // Read enrollments data
    ifstream enrollmentsFile("enrollments.json");
    if (!enrollmentsFile.is_open()) {
        cout << "Error: Unable to open enrollments.json" << endl;
        return;
    }
    json enrollmentsData;
    enrollmentsFile >> enrollmentsData;
    enrollmentsFile.close();

    // Check if student is enrolled in any courses
    if (!enrollmentsData["enrollments"].contains(id)) {
        cout << "Courses: Not enrolled in any courses.\n";
    } else {
        cout << "\nEnrolled Courses:\n";
        cout << "----------------------------------------------------\n";
        cout << "Course Code\tGrade\t\tAttendance\n";
        cout << "----------------------------------------------------\n";

        float totalGrade = 0.0f;
        float totalAttendance = 0.0f;
        int courseCount = 0;

        for (const auto& enrollment : enrollmentsData["enrollments"][id]) {
            string courseCode = enrollment["courseCode"];
            float grade = enrollment["grade"];
            float attendance = enrollment["attendance"];

            cout << courseCode << "\t\t" << grade << "\t\t" << attendance << "%\n";

            totalGrade += grade;
            totalAttendance += attendance;
            courseCount++;
        }

        cout << "----------------------------------------------------\n";
        if (courseCount > 0) {
            float avgGrade = totalGrade / courseCount;
            float avgAttendance = totalAttendance / courseCount;
            
            // Round to two decimal places
            avgGrade = round(avgGrade * 100.0) / 100.0;
            avgAttendance = round(avgAttendance * 100.0) / 100.0;

            cout << "Average Grade: " << avgGrade << endl;
            cout << "Average Attendance: " << avgAttendance << "%\n";
        }
    }

    cout << "\n======================================\n";
}

Student :: Student(const string &userId = "", const string& userName = "", StudentStats* stat = nullptr) : User(userId, userName), stats(stat) 
{
    if (stats == nullptr) 
    {
        stats = new StudentStats[1];  // Initialize with default values
        stats[0] = {"", 0, 0.0f};  // Default course code, grade, and attendance
    }
}
   
Student :: ~Student() 
{
    delete[] stats;
}



#include <iostream>
#include <string>
#include <fstream>
#include <cmath>
#include "json.hpp"
using json = nlohmann::json;
using namespace std;



struct Course
{
    string Code;
    string Name;
};

struct StudentStats
{
    string courseCode;
    int grade;
    float attendance;
};



class User
{
    protected:
        string id;
        string name;
    
    public:
        User(const string &userId, const string &userName) { id = userId; name = userName;};
        virtual void viewProfile() const = 0;
        string getId() const;
        string getName() const;
        ~User() {};

};

class Principal : public User
{
    public:
        void createStudent(const string &id, const string &name);
        void retrieveStudent(const string &id);
        void updateStudent(const string &id);
        void deleteStudent(const string &id);
        void viewAllStudents() const;

        void createTeacher(const string &id, const string &name);
        void retrieveTeacher(const string &id);
        void updateTeacher(const string &id);
        void deleteTeacher(const string &id);
        void viewAllTeachers() const;

        void createCourse(const string &id, const string &name);
        void retrieveCourse(const string &id);
        void updateCourse(const string &id);
        void deleteCourse(const string &id);
        void viewAllCourses() const;

        void viewProfile() const override;
        Principal(string &userId, const string &userName) : User(userId, userName) {};

};

class Student : public User
{
    private:
        StudentStats* stats;
    
    public:
        Student(const string &userId, const string &userName, StudentStats*);
        ~Student();
        void viewProfile() const override;
        void viewAttendances() const;
        void viewGrades() const;

};

class Teacher : public User
{
    private:
        Course* courses;

    public:
        Teacher(const string &userId, const string &userName, Course* c);
        ~Teacher();
        void viewProfile() const override;
        void updateAttendance(const string &studentId, const string &courseCode);
        void updateGrade(const string &studentId, const string &courseCode);

};






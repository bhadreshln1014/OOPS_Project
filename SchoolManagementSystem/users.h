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
        virtual void viewProfile(ostream &os) const = 0;
        virtual void viewProfile() const = 0;
        string getId() const;
        string getName() const;
        ~User() {};

};

class Student : public User
{
    private:
        StudentStats* stats;
    
    public:
        Student(const string &userId, const string &userName, StudentStats*);
        ~Student();
        void viewProfile() const override;
        void viewProfile(ostream &os) const override;
        friend ostream & operator <<(ostream &os, const Student &);
        void viewAttendances() const;
        void viewGrades() const;
        Student();

};

class Teacher : public User
{
    private:
        Course* courses;

    public:
        Teacher(const string &userId, const string &userName, Course* c);
        ~Teacher();
        Teacher();
        void viewProfile() const override;
        void viewProfile(ostream &os) const override;
        void updateAttendance(const string &studentId, const string &courseCode);
        void updateGrade(const string &studentId, const string &courseCode);
        friend ostream & operator <<(ostream &os, const Teacher &);

};

class Principal : public User
{
    public:
        void createStudent();
        void retrieveStudent();
        void updateStudent();
        void deleteStudent();
        void viewAllStudents() const;

        void createTeacher();
        void retrieveTeacher();
        void updateTeacher();
        void deleteTeacher();
        void viewAllTeachers() const;

        void createCourse(const string &id, const string &name);
        void retrieveCourse(const string &id);
        void updateCourse(const string &id);
        void deleteCourse(const string &id);
        void viewAllCourses() const;

        void viewProfile() const override;
        void viewProfile(ostream &os) const override;
        friend ostream & operator <<(ostream &os, const Principal &);
        Principal(string &userId, const string &userName) : User(userId, userName) {};

        template <typename T>
        void create(T& object)
        {
            if(is_same<decay_t<T>, Student>::value)
            {
                createStudent();
            }
            else if(is_same<decay_t<T>, Teacher>::value)
            {
                createTeacher();
            }
        }

        template <typename T>
        void retrieve(T& object)
        {
            if(is_same<decay_t<T>, Student>::value)
            {
                retrieveStudent();
            }
            else if(is_same<decay_t<T>, Teacher>::value)
            {
                retrieveTeacher();
            }
        }

        template <typename T>
        void update(T& object)
        {
            if(is_same<decay_t<T>, Student>::value)
            {
                updateStudent();
            }
            else if(is_same<decay_t<T>, Teacher>::value)
            {
                updateTeacher();
            }
        }

        template <typename T>
        void del(T& object)
        {
            if(is_same<decay_t<T>, Student>::value)
            {
                deleteStudent();
            }
            else if(is_same<decay_t<T>, Teacher>::value)
            {
                deleteTeacher();
            }
        }

        template <typename T>
        void viewAll(T& object)
        {
            if(is_same<decay_t<T>, Student>::value)
            {
                viewAllStudents();
            }
            else if(is_same<decay_t<T>, Teacher>::value)
            {
                viewAllTeachers();
            }
        }

};






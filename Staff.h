#ifndef STAFF_H
#define STAFF_H

#include <vector>
#include "Department.h"

struct Staff_PIndex
{
    int offset;
    char ID[13];
};

struct Staff_SIndex
{
    char Name[50];
    short pointer;

};

struct Staff_InvertedList
{
    char ID[13];
    int next;
};

class Staff
{
private:
    char ID[13];
    char Name[50];
    char Degree[50];
    char Dept_ID[13];
public:




    void load_Staff_PIndex();
    void load_Staff_SIndex();
    void reocnstruct_Staff_PIndex();
    void reocnstruct_Staff_SIndex();
    bool get_Staff_IndexState();

    void save_Staff_PIndex();
    void save_Staff_SIndex();

    void sort_Staff_PIndex();
    void sort_Staff_SIndex();
    void add_Staff();
    void change_Staff_IndexState(bool state);

    void write_Staff_SIndex( char Name[], char ID[]);
    void update_record_InvertedList(Staff_InvertedList invertupdate,int address);
    void appendTo_Staff_InvertedList(Staff_InvertedList invert);
    void update_Staff_SIndex(int next);

    int checkNameSIndex(char Name[]);
    void printAll_Staff();
    int StaffPIndexBinarySearch(char id[13]);
    void get_Dept_With_Max_Number_of_staff();
    void increament_Counter(char id[] );
    Staff get_Staff(int offset);
    void Delete_Staff(char id[]);
};

#endif // STAFF_H

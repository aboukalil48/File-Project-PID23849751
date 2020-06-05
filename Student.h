#ifndef STUDENT_H
#define STUDENT_H

struct Stud_PIndex
{
    int offset;
    char ID[13];
};

struct Stud_SIndex
{
    char Name[50];
    short pointer;

};

struct Stud_InvertedList
{
    char ID[13];
    int next;
};

class Student
{
private:
    char ID[13];
    char Name[50];
    char GPA[13];
    char Level[13];
    char Dept_ID[13];
public:

    void add_stud();
    void change_Stud_IndexState(bool state);
    void write_Stud_SIndex( char Name[], char ID[]);
    void sort_Stud_PIndex();
    void sort_Stud_SIndex();
    bool get_Stud_IndexState();
    void load_Stud_PIndex();
    void load_Stud_SIndex();
    void reocnstruct_Stud_PIndex();
    void reocnstruct_Stud_SIndex();
    void save_Stud_PIndex();
    void save_Stud_SIndex();
    void update_record_InvertedList(Stud_InvertedList invertupdate,int address);
    void appendTo_Stud_InvertedList(Stud_InvertedList invert);
    void update_Stud_SIndex(int next);
    int  checkNameSIndex(char Name[]);
    void printAll_stud();
    int  StudPIndexBinarySearch(char id[]);
    void get_stud_joined_Dept_Managed_By(char Name[] );
    void Delete_stud(char id[]);
    void print_GPA_in_rang(char low[] , char high[]);

};

#endif // STUDENT_H

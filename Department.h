#ifndef DEPARTMENT_H
#define DEPARTMENT_H
#include <vector>

struct Dept_PIndex
{
    int offset;
    char ID[13];
};

struct Dept_SIndex
{
    char Name[50];
    short pointer;

};

struct Dept_InvertedList
{
    char ID[13];
    int next;
};

struct Dept_Counter {
    char ID[13];
    int counter;
};

class Department
{
private:
    char ID[13];
    char Name[50];
    char HeadOfDept[50];
public:



    void save_Dept_PIndex();
    void save_Dept_SIndex();

    void add_Dept();
    void write_Dept_SIndex( char Name[], char ID[]);
    void change_Dept_IndexState(bool state);
    void sort_Dept_PIndex();
    void sort_Dept_SIndex();

    bool get_Dept_IndexState();
    void load_Dept_PIndex();
    void load_Dept_SIndex();
    void reocnstruct_Dept_PIndex();
    void reocnstruct_Dept_SIndex();




    void update_record_InvertedList(Dept_InvertedList invertupdate,int address);
    void appendTo_Dept_InvertedList(Dept_InvertedList invert);
    void update_Dept_SIndex(int next);

    int checkNameSIndex(char Name[]);
    void printAll_Dept();
    int DeptPIndexBinarySearch(char id[13]);
    std::vector<Dept_Counter> get_Dept_IDs();
    Department get_Dept(int offset);
    int getOffset(int index);
    int get_D_index();
    char* getName(Department p);
    char* getHeadOfDept(Department p);
    void getID(Department p ,char Name[]);
    Department get_Dept_Managed_By(char id[13]);
    void Delete_Dept(char id[]);
    int getIndex();

};
#endif // DEPARTMENT_H

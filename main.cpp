#include <iostream>
#include <fstream>
#include "Student.h"
#include "Staff.h"
#include "Department.h"
#include <vector>
#include <strstream>
#include <sstream>

using namespace std;

void menu();
void ExcecuteQuery(string );
vector<string> split (const string &s, char delim);

int main()
{
    //add staff first then department for that staff then add student to exist dept
    // exist of each from Primary Indexes
    Student stud;
    Staff staff;
    Department dept;

    stud.load_Stud_PIndex();
    stud.load_Stud_SIndex();
    staff.load_Staff_PIndex();
    staff.load_Staff_SIndex();
    dept.load_Dept_PIndex();
    dept.load_Dept_SIndex();

    menu();

    stud.save_Stud_PIndex();
    stud.save_Stud_SIndex();
    staff.save_Staff_PIndex();
    staff.save_Staff_SIndex();
    dept.save_Dept_PIndex();
    dept.save_Dept_SIndex();

    return 0;
}

void menu()
{
    int num;
    Student stud;
    Staff staff;
    Department dept;
    while(true)
    {
        cout<<"================ Menu ==================================\n"<<endl;
        cout<<"1)  Add New Student"<<endl;
        cout<<"2)  Delete Student"<<endl;
        cout<<"3)  Print All Students"<<endl;
        cout<<"4)  Add New Staff"<<endl;
        cout<<"5)  Delete Staff"<<endl;
        cout<<"6)  Print All Staffs"<<endl;
        cout<<"7)  Add New Dept"<<endl;
        cout<<"8)  Delete Dept"<<endl;
        cout<<"9)  Print All Dept"<<endl;
        cout<<"10) Write a Query"<<endl;
        cout<<"11) Exit"<<endl;
        cout<<"\nChoice : ";
        cin>>num;
        cin.ignore();

        if(num==1)
        {
            cout<<"================ Add New Student ======================\n"<<endl;
            stud.add_stud();
        }
        else if( num == 2 )
        {
            cout<<"================ Delete Student ======================\n"<<endl;
            char id[13];
            cout<<"\nEnter Student ID : ";
            cin.getline(id, 13);
            stud.Delete_stud(id);
        }
        else if( num == 3 )
        {
            cout<<"================ Print All Student ======================\n"<<endl;
            stud.printAll_stud();
        }
        else if( num == 4 )
        {
            cout<<"================ Add New Staff ======================\n"<<endl;
            staff.add_Staff();
        }
        else if( num == 5 )
        {
            cout<<"================ Delete Staff ======================\n"<<endl;
            char id[13];
            cout<<"\nEnter Staff ID : ";
            cin.getline(id, 13);
            staff.Delete_Staff(id);
        }
        else if( num == 6 )
        {
            cout<<"================ Print All Staffs ======================\n"<<endl;
            staff.printAll_Staff();
        }
        else if( num == 7 )
        {
             cout<<"================ Add New Department ======================\n"<<endl;
             dept.add_Dept();
        }
        else if( num == 8 )
        {
            cout<<"================ Delete Department ======================\n"<<endl;
            char id[13];
            cout<<"\nEnter Dept ID : ";
            cin.getline(id, 13);
            dept.Delete_Dept(id);
        }
        else if( num == 9 )
        {
            cout<<"================ Print All Departments ======================\n"<<endl;
            dept.printAll_Dept();
        }
        else if( num == 10 )
        {
            cout<<"================ Write a Query =========================="<<endl;
            string query;
            cout<<"\nEnter Query : ";
            getline(cin, query);
            cout<<endl;
            ExcecuteQuery(query);
        }
        else if( num == 11 )
        {
            break;
        }
        else
        {
            cout<<"Wrong choice"<<endl;
        }
    }
}

vector<string> split (const string &s, char delim)
{
    vector<string> result;
    stringstream ss (s);
    string item;

    while (getline (ss, item, delim))
    {
        result.push_back (item);
    }

    return result;
}


void ExcecuteQuery(string query)
{
    vector<string> v= split (query, ' ');
    Department dept;
    Student stud;
    Staff staff;
    if(v.at(0) == "find")
    {
        // find students with gpa between 2.1 and 3.3
        // find students joined department managed by name='Moussa Hamid'
        if(v.at(1)  == "students" || v.at(1)  == "student")
        {
            if(v.at(2)  == "with")
            {
                if( (v.at(3)  == "gpa"  || v.at(3)  == "GPA") && v.at(4)  == "between")
                {
                    string  l = v.at(5);
                    string  h = v.at(6);
                    char low[l.size() + 1] ;
                    char high[h.size() + 1];
                    l.copy(low, l.size() + 1);
                    h.copy(high, h.size() + 1);
                    low[l.size()] = '\0';
                    high[h.size()] = '\0';
                    stud.print_GPA_in_rang(low, high);
                }
            } // find student joined department managed by name = 'Ibrahim Mohamed' 'Moussa Hamid'
            else if(v.at(2)  == "joined")
            {
                if(v.at(3)  == "Department" || v.at(3)  == "department")
                {
                    if(v.at(4)  == "managed" && v.at(5)  == "by")
                    {
                        // name index 6
                        // = index 7
                        // name value start from index 8
                        string s="";
                        if(v.size()>9)
                        {
                            for(int i=8 ; i< v.size(); i++)
                            {
                                if(i == 8)
                                {
                                    s += v.at(i).substr(1,(v.at(i).length()))+" "; // remove " at start
                                }
                                else if(i != v.size()-1)
                                {
                                     s += v.at(i) + " ";
                                }
                                else
                                {
                                     s += v.at(i).substr(0,(v.at(i).length()-1));       // remove " at end
                                }
                            }
                        }
                        else
                        {
                            s = v.at(8).substr(1,(v.at(8).length())-2);
                        }
                        char Name[s.size() + 1];
                        s.copy(Name, s.size() + 1);
                        Name[s.size()] = '\0';
                        stud.get_stud_joined_Dept_Managed_By(Name);
                    }
                }
            }
        }//find department with max number of staff members
        else if(v.at(1)  == "Department" || v.at(1)  == "department")
        {
            if(v.size() == 8)
            {
                if(v.at(2)  == "with" && v.at(3)  == "max" && v.at(4)  == "number" && v.at(5)  == "of" && v.at(6)  == "staff" && v.at(7)  == "members")
                {
                    cout<<"================  Department with max number of staff members ========"<<endl;
                    staff.get_Dept_With_Max_Number_of_staff();
                }
            }
        }
    }
}

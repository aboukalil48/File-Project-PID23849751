#include "Department.h"
#include "Staff.h"
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <strstream>
#include <cstring>
#include <sstream>
#include <vector>
#include <string>
#include <stdio.h>
#include <ctime>
#include <vector>
using namespace std;


static int d_index =0 ;
static int D_S_index =0 ;
static int Department_size=0 ;

Dept_PIndex D_PIndex[20];
Dept_SIndex D_SIndex[20];

char* Department::getName(Department p)
{
    return p.Name;
}

void Department::getID(Department p ,char Name[])
{
    strcpy(Name , p.ID);
}
char* Department::getHeadOfDept(Department p)
{
    return p.HeadOfDept;
}


void Department::change_Dept_IndexState(bool state)
{
    ifstream in("status.txt" , ios::in);
    bool data;
    bool data_arr[3];
    in.seekg(2,ios::beg);
    for(int i = 0; i<3; i++)
    {
        in.read((char*)&data, sizeof(data));
        data_arr[i] = data;
    }
    in.close();

    data_arr[2] = state; // update state of staff

    ofstream out("status.txt" , ios::out | ios::trunc);
    out<<data_arr[0]<<data_arr[1]<<data_arr[2];
    out.close();
}

bool Department::get_Dept_IndexState()
{
    ifstream inf("status.txt",ios::in);
    bool state;
    inf.seekg(0,ios::beg);
    inf.read((char*)&state, sizeof(state));
    inf.close();
    return state;
}

void Department::reocnstruct_Dept_PIndex()
{
    ifstream in("Dept_data.txt",ios::in);
    Department Dept;
    short len;
    char ch;
    d_index=0;
    int address;
    in.seekg(0,ios::beg);
    char record[120];

    if(in.good())
    {
        while(!in.eof())
        {
            address=in.tellg();
            in.read ((char*) &ch, sizeof (char));
            if(in.eof())
                break;
            if(ch=='*')
            {
                in.read(record,120);
                continue;
            }
            in.read(record,len);
            istrstream buff(record);
            buff.getline(Dept.ID,13,'|');
            buff.getline(Dept.Name,50,'|');
            buff.getline(Dept.HeadOfDept,50,'|');
            Dept_PIndex temp1 ;
            strcpy(temp1.ID,Dept.ID);
            temp1.offset=address;
            D_PIndex[d_index++] = temp1;
        }
        sort_Dept_PIndex();
    }
    in.close();
}

void Department::reocnstruct_Dept_SIndex()
{
    ifstream in("Dept_data.txt",ios::in);
    Department Dept;
    short len;
    char ch;
    D_S_index=0;
    Department_size=0;
    in.seekg(0,ios::beg);
    char record[120];

    if(in.good())
    {
        while(!in.eof())
        {
            in.read ((char*) &ch, sizeof (char));
            if(in.eof())
                break;
            if(ch=='*')
            {
                in.read(record,120);
                continue;
            }
            in.read(record,120);
            istrstream buff(record);
            buff.getline(Dept.ID,13,'|');
            buff.getline(Dept.Name,50,'|');
            buff.getline(Dept.HeadOfDept,50,'|');

            write_Dept_SIndex(Dept.Name, Dept.ID);
            Department_size++;
            D_S_index++;
        }
        sort_Dept_SIndex();
    }
    in.close();
}

void Department::load_Dept_PIndex()
{
    if(!get_Dept_IndexState())
    {
        ifstream in("Dept_PIndex.txt",ios::in);
        Dept_PIndex temp;
        if(in.good())
        {
            in.seekg(0,ios::beg);
            d_index = 0;
            while(!in.eof())
            {
                in.read((char*) &temp,sizeof(temp));
                if(in.eof())
                    break;
                D_PIndex[d_index++] = temp;
            }
            in.close();
            sort_Dept_PIndex();
        }
    }
    else
        reocnstruct_Dept_PIndex();
}

void Department::sort_Dept_SIndex()
{
    int  j;
    char Name[50];
    Dept_SIndex temp;
    for(int i = 1; i<D_S_index; i++)
    {
        strcpy(Name,D_SIndex[i].Name);
        strcpy(temp.Name,D_SIndex[i].Name);
        temp.pointer = D_SIndex[i].pointer;
        j = i-1;
        while( (j >= 0 ) && strcmp(D_SIndex[j].Name, temp.Name ) > 0)
        {
            strcpy( D_SIndex[j+1].Name, D_SIndex[j].Name );
            D_SIndex[j+1].pointer = D_SIndex[j].pointer;
            j--;
        }
        D_SIndex[j+1] = temp;
    }
}


void Department::load_Dept_SIndex()
{
    if(!get_Dept_IndexState())
    {
        ifstream in("Dept_SIndex.txt",ios::in);
        Dept_SIndex temp;
        if(in.good())
        {
            in.seekg(0,ios::beg);
            D_S_index = 0;
            Department_size =0;
            while(!in.eof())
            {
                Department_size++;
                in.read((char*) &temp,sizeof(temp));
                if(in.eof())
                    break;
                D_SIndex[D_S_index++] = temp;
            }
            in.close();
            sort_Dept_SIndex();
        }

    }
    else
        reocnstruct_Dept_SIndex();
}

void Department::save_Dept_PIndex()
{
    ofstream out("Dept_PIndex.txt",ios::out | ios::trunc);
    if(out.good())
    {
        Dept_PIndex Dept;
        for(int i=0 ; i<d_index; i++)
        {
            Dept = D_PIndex[i];
            out.write((char*)& (Dept),sizeof(Dept));
        }
        out.close();
        change_Dept_IndexState(0);
    }
}

void Department::save_Dept_SIndex()
{
    ofstream out("Dept_SIndex.txt",ios::out | ios::trunc);
    if(out.good())
    {
        Dept_SIndex Dept;
        for(int i=0 ; i<D_S_index; i++)
        {
            Dept = D_SIndex[i];
            out.write((char*)& (Dept),sizeof(Dept));
        }
        out.close();
        change_Dept_IndexState(0);
    }
}

// sort Primary Index key (ID) ascending
void Department::sort_Dept_PIndex()
{
    int  j;
    char id[13];
    Dept_PIndex temp;
    for(int i = 1; i<d_index; i++)
    {
        strcpy(id,D_PIndex[i].ID);
        strcpy(temp.ID,D_PIndex[i].ID);
        temp.offset = D_PIndex[i].offset;
        j = i-1;
        while( (j >= 0 ) && strcmp(D_PIndex[j].ID, temp.ID ) > 0)
        {
            strcpy( D_PIndex[j+1].ID, D_PIndex[j].ID );
            D_PIndex[j+1].offset = D_PIndex[j].offset;
            j--;
        }
        D_PIndex[j+1] = temp;
    }
}

void Department::update_record_InvertedList(Dept_InvertedList invertupdate,int address)
{
    fstream Invert_file("Dept_InvertedList.txt",ios::out | ios::in);
    Invert_file.seekp((address * sizeof(invertupdate) ),ios::beg);
    Invert_file.write((char*)& invertupdate, sizeof (invertupdate));
    Invert_file.close();
}

void Department::appendTo_Dept_InvertedList(Dept_InvertedList invert)
{
    ofstream file("Dept_InvertedList.txt",ios::out | ios::app);
    if(file.good())
    {
        file.seekp( 0,ios::end);
        file.write((char*)&invert, sizeof(invert));
    }
    file.close();
}

void Department::update_Dept_SIndex(int next)
{
    ifstream file("Dept_InvertedList.txt",ios::in);
    Dept_InvertedList invertupdate;
    Dept_InvertedList invertTemp;
    if(file.good())
    {
        while(!file.eof())
        {
            file.seekg((sizeof(invertTemp) * next ),ios::beg);
            file.read((char*)& invertTemp, sizeof (invertTemp));
            if(file.eof())
                break;
            if(invertTemp.next == -1)
            {
                strcpy(invertupdate.ID, invertTemp.ID);
                invertupdate.next = d_index;
                file.close();
                update_record_InvertedList(invertupdate,next);
                break;
            }
            else
            {
                next = invertTemp.next;
            }
        }

    }
    if(file.is_open())
        file.close();
}

void Department::write_Dept_SIndex( char Name[], char ID[])
{
    Dept_SIndex temp;
    Dept_InvertedList invert;
    strcpy(temp.Name,Name);
    strcpy(invert.ID, ID);
    invert.next = -1;
    int next = checkNameSIndex(Name);
    if(next == -1)
    {
        temp.pointer = Department_size;
        D_SIndex[D_S_index] = temp;
        D_S_index++;
    }
    else
    {
        update_Dept_SIndex(next);
    }
    appendTo_Dept_InvertedList(invert);
}

int Department::checkNameSIndex(char Name[])
{
    int pointer = -1;
    for(int i=0 ; i<D_S_index; i++)
    {
        if( strcmp( D_SIndex[i].Name, Name) == 0)
        {
            pointer = D_SIndex[i].pointer;
            break;
        }
    }
    return pointer;
}

void Department::add_Dept()
{

    Department Dept ;
    cout<<"Enter Dept ID : ";
    cin.getline(Dept.ID,13);
    cout<<"Enter Dept Name : ";
    cin.getline(Dept.Name,50);
    cout<<"Enter Head Of Dept : ";
    cin.getline(Dept.HeadOfDept,50);

    Staff staff;
    int found = staff.checkNameSIndex(Dept.HeadOfDept);
    if(found != -1)
    {
        ofstream out("Dept_data.txt",ios::out | ios::app);
        out.seekp(0,ios::end);
        int address=out.tellp();
        change_Dept_IndexState(1);
        char* buffer = new char[120];
        strcpy(buffer,Dept.ID);
        strcat(buffer,"|");
        strcat(buffer,Dept.Name);
        strcat(buffer,"|");
        strcat(buffer,Dept.HeadOfDept);
        strcat(buffer,"|");
        strcat(buffer,"\n");

        short len = strlen(buffer);
        out.write((char*)&"$", sizeof(char));
        out.write(buffer, len);

        //save to Primary index array
        Dept_PIndex temp1 ;
        strcpy(temp1.ID,Dept.ID);
        temp1.offset=address;
        D_PIndex[d_index] = temp1;

        //save to Secondary index array
        write_Dept_SIndex(Dept.Name, Dept.ID);

        d_index++;
        Department_size++;
        sort_Dept_PIndex();
        sort_Dept_SIndex();
        delete [] buffer;

        out.close();
    }
    else
    {
        cout<<"There is no staff with Name :"<<Dept.HeadOfDept<<endl;
    }

}


void Department::printAll_Dept()
{
    ifstream in("Dept_data.txt",ios::in);
    Department Dept;
    char ch;
    in.seekg(0,ios::beg);
    char record[120];
    if(in.good())
    {
        while(!in.eof())
        {
            in.read ((char*) &ch, sizeof (char));
            if(in.eof())
                break;
            if(ch=='*')
            {
                in.getline(record, 120);
                continue;
            }
            in.getline(record, 120);  // delim defaults to '\n'
            istrstream buff(record);
            buff.getline(Dept.ID,13,'|');
            buff.getline(Dept.Name,50,'|');
            buff.getline(Dept.HeadOfDept,50,'|');
            cout<<"Dept ID      : "<<Dept.ID<<endl;
            cout<<"Dept Name    : "<<Dept.Name<<endl;
            cout<<"Head Of Dept : "<<Dept.HeadOfDept<<endl;
            cout<<endl;
        }
    }
    in.close();

}
/*
void Department::printInverted()
{
    ifstream in("Dept_InvertedList.txt",ios::in);
    in.seekg(0,ios::beg);
    Dept_InvertedList invert;
    if(in.good())
    {
        while(! in.eof())
        {
            in.read((char*) &(invert), sizeof(invert));

            if(in.eof())
                break;
            cout<<"Invert ID  : "<<invert.ID<<"  Invert next : "<<invert.next<<endl;
        }
        in.close();
    }
    else
        cout<<"Can not open file "<<endl;
}
*/
int Department::DeptPIndexBinarySearch(char id[])
{
    int low=0 ;
    int high=d_index;
    while(low<=high)
    {
        int mid=(low+high)/2;
        if( strcmp( D_PIndex[mid].ID, id) == 0)
            return mid;
        else if( strcmp(D_PIndex[mid].ID,id)<0)
            low=mid+1;
        else
            high=mid-1;
    }
    return -1 ;
}



Department Department::get_Dept_Managed_By(char Name[])
{
    ifstream in("Dept_data.txt",ios::in);
    Department Dept ;
    char ch;
    in.seekg(0,ios::beg);
    char record[120];
    if(in.good())
    {
        while(!in.eof())
        {
            in.read ((char*) &ch, sizeof (char));
            if(in.eof())
                break;
            if(ch=='*')
            {
                in.getline(record, 120);
                continue;
            }
            else
            {
                in.getline(record, 120);  // delim defaults to '\n'
                istrstream buff(record);
                buff.getline(Dept.ID,13,'|');
                buff.getline(Dept.Name,50,'|');
                buff.getline(Dept.HeadOfDept,50,'|');
                if(strcmp(Name,Dept.HeadOfDept)==0)
                {
                    break;
                }
            }
        }
    }
    in.close();
    return Dept;
}

void Department::Delete_Dept(char id[])
{
    change_Dept_IndexState(1);
    fstream file("Dept_data.txt",ios::out | ios::in);
    char ch;
    int curr_index= DeptPIndexBinarySearch(id);
    if(curr_index==-1)
    {
        cout<<"Not found !"<<endl;
        return;
    }
    int offset = D_PIndex[curr_index].offset;
    file.seekg(offset,ios::beg);
    file.read((char*)&ch, sizeof(ch));
    if(file.eof())
        return;
    file.seekp(offset,ios::beg);
    file.write((char*)&"*",sizeof(char));
    file.close();
    cout << "Dept Deleted" << endl;
    // shift PIndex
    for(int i=curr_index ; i<d_index-1; i++)
    {
        Dept_PIndex temp =D_PIndex[i+1];
        D_PIndex[i]=temp;
    }
    d_index--;
}


Department Department::get_Dept(int offset)
{
    ifstream in("Dept_data.txt",ios::in);
    Department Dept;
    char ch;

    char record[120];
    if(in.good())
    {
        in.seekg(offset,ios::beg);
        in.read ((char*) &ch, sizeof (char));
        if(ch != '*')
        {
            in.getline(record, 120);
            istrstream buff(record);
            buff.getline(Dept.ID,13,'|');
            buff.getline(Dept.Name,50,'|');
            buff.getline(Dept.HeadOfDept,50,'|');
        }
    }
    in.close();
    return Dept;
}

int Department::getOffset(int index)
{
    return D_PIndex[index].offset;
}
int Department::get_D_index()
{
    return d_index;
}
std::vector<Dept_Counter> Department::get_Dept_IDs()
{
    std::vector<Dept_Counter> dept_couter(d_index);
    for(int i=0 ; i<d_index; i++)
    {
        Dept_Counter d;
        strcpy(d.ID, D_PIndex[i].ID);
        d.counter = 0;
        dept_couter.push_back(d);
    }
    return dept_couter;
}
/*
void Department::printPIndex()
{
    Dept_PIndex Dept;
    for(int i=0 ; i<d_index; i++)
    {
        Dept = D_PIndex[i];
        cout<<"Dept ID  : "<<Dept.ID<<" - "<<"Dept offset : "<<Dept.offset<<endl;
    }
}

void Department::printSIndex()
{
   Dept_SIndex Dept;
    cout<<"================ Dept Secondary Index =========================="<<endl;
    for(int i=0 ; i<D_S_index; i++)
    {
        Dept = D_SIndex[i];
        cout<<"Dept Name  : "<<Dept.Name<<" - "<<" Pointer : "<<Dept.pointer<<endl;
    }
}

*/

#include "Student.h"
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
using namespace std;

static int s_index =0 ;
static int SS_index =0 ;
static int Student_size=0 ;

Stud_PIndex S_PIndex[20];
Stud_SIndex S_SIndex[20];


void Student::change_Stud_IndexState(bool state)
{
    ifstream in("status.txt" , ios::in);
    bool data;
    bool data_arr[3];
    in.seekg(0,ios::beg);
    for(int i = 0; i<3; i++)
    {
        in.read((char*)&data, sizeof(data));
        data_arr[i] = data;
    }
    in.close();

    data_arr[0] = state; // update state of staff

    ofstream out("status.txt" , ios::out | ios::trunc);
    out<<data_arr[0]<<data_arr[1]<<data_arr[2];
    out.close();
}

bool Student::get_Stud_IndexState()
{
    ifstream inf("status.txt",ios::in);
    bool state;
    inf.seekg(0,ios::beg);
    inf.read((char*)&state, sizeof(state));
    inf.close();
    return state;
}

void Student::reocnstruct_Stud_PIndex()
{
    ifstream in("stud_data.txt",ios::in);
    Student stud;
    char ch;
    s_index=0;
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
            in.read(record,120);
            istrstream buff(record);
            buff.getline(stud.ID,13,'|');
            buff.getline(stud.Name,50,'|');
            buff.getline(stud.GPA,13,'|');
            buff.getline(stud.Level,13,'|');
            buff.getline(stud.Dept_ID,13,'|');
            Stud_PIndex temp1 ;
            strcpy(temp1.ID,stud.ID);
            temp1.offset=address;
            S_PIndex[s_index++] = temp1;
        }
        sort_Stud_PIndex();
    }
    in.close();
}

void Student::reocnstruct_Stud_SIndex()
{
    ifstream in("stud_data.txt",ios::in);
    Student stud;
    char ch;
    SS_index=0;
    Student_size=0;
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
            buff.getline(stud.ID,13,'|');
            buff.getline(stud.Name,50,'|');
            buff.getline(stud.GPA,13,'|');
            buff.getline(stud.Level,13,'|');
            buff.getline(stud.Dept_ID,13,'|');

            write_Stud_SIndex(stud.Name, stud.ID);
            Student_size++;
            SS_index++;
        }
        sort_Stud_SIndex();
    }
    in.close();
}

void Student::load_Stud_PIndex()
{
    if(!get_Stud_IndexState())
    {
        ifstream in("Stud_PIndex.txt",ios::in);
        Stud_PIndex temp;
        if(in.good())
        {
            in.seekg(0,ios::beg);
            s_index = 0;
            while(!in.eof())
            {
                in.read((char*) &temp,sizeof(temp));
                if(in.eof())
                    break;
                S_PIndex[s_index++] = temp;
            }
            in.close();
            sort_Stud_PIndex();
        }
    }
    else
        reocnstruct_Stud_PIndex();
}

void Student::sort_Stud_SIndex()
{
    int  j;
    char Name[50];
    Stud_SIndex temp;
    for(int i = 1; i<SS_index; i++)
    {
        strcpy(Name,S_SIndex[i].Name);
        strcpy(temp.Name,S_SIndex[i].Name);
        temp.pointer = S_SIndex[i].pointer;
        j = i-1;
        while( (j >= 0 ) && strcmp(S_SIndex[j].Name, temp.Name ) > 0)
        {
            strcpy( S_SIndex[j+1].Name, S_SIndex[j].Name );
            S_SIndex[j+1].pointer = S_SIndex[j].pointer;
            j--;
        }
        S_SIndex[j+1] = temp;
    }
}


void Student::load_Stud_SIndex()
{
    if(!get_Stud_IndexState())
    {
        ifstream in("Stud_SIndex.txt",ios::in);
        Stud_SIndex temp;
        if(in.good())
        {
            in.seekg(0,ios::beg);
            SS_index = 0;
            Student_size =0;
            while(!in.eof())
            {
                Student_size++;
                in.read((char*) &temp,sizeof(temp));
                if(in.eof())
                    break;
                S_SIndex[SS_index++] = temp;
            }
            in.close();
            sort_Stud_SIndex();
        }

    }
    else
        reocnstruct_Stud_SIndex();
}

void Student::save_Stud_PIndex()
{
    ofstream out("Stud_PIndex.txt",ios::out | ios::trunc);
    if(out.good())
    {
        Stud_PIndex stud;
        for(int i=0 ; i<s_index; i++)
        {
            stud = S_PIndex[i];
            out.write((char*)& (stud),sizeof(stud));
        }
        out.close();
        change_Stud_IndexState(0);
    }
}

void Student::save_Stud_SIndex()
{
    ofstream out("Stud_SIndex.txt",ios::out | ios::trunc);
    if(out.good())
    {
        Stud_SIndex stud;
        for(int i=0 ; i<SS_index; i++)
        {
            stud = S_SIndex[i];
            out.write((char*)& (stud),sizeof(stud));
        }
        out.close();
        change_Stud_IndexState(0);
    }
}

// sort Primary Index key (ID) ascending
void Student::sort_Stud_PIndex()
{
    int  j;
    char id[13];
    Stud_PIndex temp;
    for(int i = 1; i<s_index; i++)
    {
        strcpy(id,S_PIndex[i].ID);
        strcpy(temp.ID,S_PIndex[i].ID);
        temp.offset = S_PIndex[i].offset;
        j = i-1;
        while( (j >= 0 ) && strcmp(S_PIndex[j].ID, temp.ID ) > 0)
        {
            strcpy( S_PIndex[j+1].ID, S_PIndex[j].ID );
            S_PIndex[j+1].offset = S_PIndex[j].offset;
            j--;
        }
        S_PIndex[j+1] = temp;
    }
}

void Student::update_record_InvertedList(Stud_InvertedList invertupdate,int address)
{
    fstream Invert_file("Stud_InvertedList.txt",ios::out | ios::in);
    Invert_file.seekp((address * sizeof(invertupdate) ),ios::beg);
    Invert_file.write((char*)& invertupdate, sizeof (invertupdate));
    Invert_file.close();
}

void Student::appendTo_Stud_InvertedList(Stud_InvertedList invert)
{
    ofstream file("Stud_InvertedList.txt",ios::out | ios::app);
    if(file.good())
    {
        file.seekp( 0,ios::end);
        file.write((char*)&invert, sizeof(invert));
    }
    file.close();
}

void Student::update_Stud_SIndex(int next)
{
    ifstream file("Stud_InvertedList.txt",ios::in);
    Stud_InvertedList invertupdate;
    Stud_InvertedList invertTemp;
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
                invertupdate.next = s_index;
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

void Student::write_Stud_SIndex( char Name[], char ID[])
{
    Stud_SIndex temp;
    Stud_InvertedList invert;
    strcpy(temp.Name,Name);
    strcpy(invert.ID, ID);
    invert.next = -1;
    int next = checkNameSIndex(Name);
    if(next == -1)
    {
        temp.pointer = Student_size;
        S_SIndex[SS_index] = temp;
        SS_index++;
    }
    else
    {
        update_Stud_SIndex(next);
    }
    appendTo_Stud_InvertedList(invert);
}

int Student::checkNameSIndex(char Name[])
{
    int pointer = -1;
    for(int i=0 ; i<SS_index; i++)
    {
        if( strcmp( S_SIndex[i].Name, Name) == 0)
        {
            pointer = S_SIndex[i].pointer;
            break;
        }
    }
    return pointer;
}

void Student::add_stud()
{
    Student stud ;
    cout<<"Enter Stud ID : ";
    cin.getline(stud.ID,13);
    cout<<"Enter Stud Name : ";
    cin.getline(stud.Name,50);
    cout<<"Enter Stud GPA : ";
    cin.getline(stud.GPA,13);
    cout<<"Enter Stud Level : ";
    cin.getline(stud.Level,13);
    cout<<"Enter Stud Dept_ID : ";
    cin.getline(stud.Dept_ID,13);

    Department d;
    int found = d.checkNameSIndex(stud.Dept_ID);
    if(found != -1)
    {
        change_Stud_IndexState(1);
        ofstream out("stud_data.txt",ios::out | ios::app);
        out.seekp(0,ios::end);
        int address=out.tellp();

        char* buffer = new char[120];
        strcpy(buffer,stud.ID);
        strcat(buffer,"|");
        strcat(buffer,stud.Name);
        strcat(buffer,"|");
        strcat(buffer,stud.GPA);
        strcat(buffer,"|");
        strcat(buffer,stud.Level);
        strcat(buffer,"|");
        strcat(buffer,stud.Dept_ID);
        strcat(buffer,"|");
        strcat(buffer,"\n");
        short len = strlen(buffer);
        out.write((char*)&"$", sizeof(char));
        out.write(buffer, len);

        //save to Primary index array
        Stud_PIndex temp1 ;
        strcpy(temp1.ID,stud.ID);
        temp1.offset=address;
        S_PIndex[s_index] = temp1;

        //save to Secondary index array
        write_Stud_SIndex(stud.Name, stud.ID);

        s_index++;
        Student_size++;
        sort_Stud_PIndex();
        sort_Stud_SIndex();
        delete [] buffer;

        out.close();
    }
    else
    {
        cout<<"There is no Department with ID :"<<stud.Dept_ID<<endl;
    }
}


void Student::printAll_stud()
{
    ifstream in("stud_data.txt",ios::in);
    Student stud;
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
            buff.getline(stud.ID,13,'|');
            buff.getline(stud.Name,50,'|');
            buff.getline(stud.GPA,13,'|');
            buff.getline(stud.Level,13,'|');
            buff.getline(stud.Dept_ID,13,'|');
            cout<<"Stud ID      : "<<stud.ID<<endl;
            cout<<"Stud Name    : "<<stud.Name<<endl;
            cout<<"Stud GPA     : "<<stud.GPA<<endl;
            cout<<"Stud Level   : "<<stud.Level<<endl;
            cout<<"Stud Dept_ID : "<<stud.Dept_ID<<endl;
            cout<<endl;
        }
    }
    in.close();

}
/*
void Student::printInverted()
{
    ifstream in("Stud_InvertedList.txt",ios::in);
    in.seekg(0,ios::beg);
    Stud_InvertedList invert;
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
int Student::StudPIndexBinarySearch(char id[])
{
    int low=0 ;
    int high=s_index;
    while(low<=high)
    {
        int mid=(low+high)/2;
        if( strcmp( S_PIndex[mid].ID, id) == 0)
            return mid;
        else if( strcmp(S_PIndex[mid].ID,id)<0)
            low=mid+1;
        else
            high=mid-1;
    }
    return -1 ;
}

void Student::print_GPA_in_rang(char low[] , char high[])
{
   ifstream in("stud_data.txt",ios::in);
    Student stud;
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
            buff.getline(stud.ID,13,'|');
            buff.getline(stud.Name,50,'|');
            buff.getline(stud.GPA,13,'|');
            buff.getline(stud.Level,13,'|');
            buff.getline(stud.Dept_ID,13,'|');

            if( (strcmp(stud.GPA ,low ) >= 0) && (strcmp(stud.GPA ,high )<= 0 ))
            {
                cout<<"Stud ID      : "<<stud.ID<<endl;
                cout<<"Stud Name    : "<<stud.Name<<endl;
                cout<<"Stud GPA     : "<<stud.GPA<<endl;
                cout<<"Stud Level   : "<<stud.Level<<endl;
                cout<<"Stud Dept_ID : "<<stud.Dept_ID<<endl;
                cout<<endl;
            }
        }
    }
    in.close();
}


void Student::Delete_stud(char id[])
{
    change_Stud_IndexState(1);
    fstream file("stud_data.txt",ios::out | ios::in);
    char ch;
    int curr_index= StudPIndexBinarySearch(id);
    if(curr_index==-1)
    {
        cout<<"Not found !"<<endl;
        return;
    }
    int offset = S_PIndex[curr_index].offset;
    file.seekg(offset,ios::beg);
    file.read((char*)&ch, sizeof(ch));
    if(file.eof())
        return;
    file.seekp(offset,ios::beg);
    file.write((char*)&"*",sizeof(char));
    file.close();
    cout << "Stud Deleted" << endl;
    // shift PIndex
    for(int i=curr_index ; i<s_index-1; i++)
    {
        Stud_PIndex temp =S_PIndex[i+1];
        S_PIndex[i]=temp;
    }
    s_index--;
}

void Student::get_stud_joined_Dept_Managed_By(char Name[] )
{
    Department d;
    Department dept = d.get_Dept_Managed_By(Name);
    char Name1[50] ;
    char ID1[13];

    d.getID(dept,ID1);
    strcpy(Name1 ,d.getHeadOfDept(dept));
    if(strcmp( Name1 , Name)==0)
    {
        ifstream in("stud_data.txt",ios::in);
        Student stud;
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
                buff.getline(stud.ID,13,'|');
                buff.getline(stud.Name,50,'|');
                buff.getline(stud.GPA,13,'|');
                buff.getline(stud.Level,13,'|');
                buff.getline(stud.Dept_ID,13,'|');
                if(strcmp( ID1 ,stud.Dept_ID) == 0)
                {
                    cout<<"Stud ID      : "<<stud.ID<<endl;
                    cout<<"Stud Name    : "<<stud.Name<<endl;
                    cout<<"Stud GPA     : "<<stud.GPA<<endl;
                    cout<<"Stud Level   : "<<stud.Level<<endl;
                    cout<<"Stud Dept_ID : "<<stud.Dept_ID<<endl;
                    cout<<endl;
                }
            }
        }
        in.close();
    }
}
/*
void Student::printPIndex()
{
    Stud_PIndex stud;
    for(int i=0 ; i<s_index; i++)
    {
        stud = S_PIndex[i];
        cout<<"Dept ID  : "<<stud.ID<<" - "<<"Dept offset : "<<stud.offset<<endl;
    }
}

void Student::printSIndex()
{
   Stud_SIndex stud;
    cout<<"================ Stud Secondary Index =========================="<<endl;
    for(int i=0 ; i<SS_index; i++)
    {
        stud = S_SIndex[i];
        cout<<"Dept Name  : "<<stud.Name<<" - "<<" Pointer : "<<stud.pointer<<endl;
    }
}
*/

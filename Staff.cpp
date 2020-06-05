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
#include "Department.h"

using namespace std;

static int Staff_index =0 ;
static int Staff_S_index =0 ;
static int Staff_size=0 ;

Staff_PIndex Sta_PIndex[20];
Staff_SIndex Sta_SIndex[20];

static std::vector<Dept_Counter> dept_counter;

void Staff::change_Staff_IndexState(bool state)
{
    ifstream in("status.txt" , ios::in);
    bool data;
    bool data_arr[3];
    in.seekg(1,ios::beg);
    for(int i = 0; i<3; i++)
    {
        in.read((char*)&data, sizeof(data));
        data_arr[i] = data;
    }
    in.close();

    data_arr[1] = state; // update state of staff

    ofstream out("status.txt" , ios::out | ios::trunc);
    out<<data_arr[0]<<data_arr[1]<<data_arr[2];
    out.close();
}

bool Staff::get_Staff_IndexState()
{
    ifstream inf("status.txt",ios::in);
    bool state;
    inf.seekg(1,ios::beg);
    inf.read((char*)&state, sizeof(state));
    inf.close();
    return state;
}

void Staff::reocnstruct_Staff_PIndex()
{
    ifstream in("Staff_data.txt",ios::in);
    Staff Staff;
    short len;
    char ch;
    Staff_index=0;
    int address;
    in.seekg(0,ios::beg);
    char record[150];

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
                in.read(record,150);
                continue;
            }
            in.read(record,len);
            istrstream buff(record);
            buff.getline(Staff.ID,13,'|');
            buff.getline(Staff.Name,50,'|');
            buff.getline(Staff.Degree,50,'|');
            buff.getline(Staff.Dept_ID,13,'|');
            Staff_PIndex temp1 ;
            strcpy(temp1.ID,Staff.ID);
            temp1.offset=address;
            Sta_PIndex[Staff_index++] = temp1;
        }
        sort_Staff_PIndex();
    }
    in.close();
}

void Staff::reocnstruct_Staff_SIndex()
{
    ifstream in("Staff_data.txt",ios::in);
    Staff Staff;
    char ch;
    Staff_S_index=0;
    Staff_size=0;
    in.seekg(0,ios::beg);
    char record[150];

    if(in.good())
    {
        while(!in.eof())
        {
            in.read ((char*) &ch, sizeof (char));
            if(in.eof())
                break;
            if(ch=='*')
            {
                in.read(record,150);
                continue;
            }
            in.read(record,150);
            istrstream buff(record);
            buff.getline(Staff.ID,13,'|');
            buff.getline(Staff.Name,50,'|');
            buff.getline(Staff.Degree,50,'|');
            buff.getline(Staff.Dept_ID,13,'|');

            write_Staff_SIndex(Staff.Name, Staff.ID);
            Staff_size++;
            Staff_S_index++;
        }
        sort_Staff_SIndex();
    }
    in.close();
}

void Staff::load_Staff_PIndex()
{
    if(!get_Staff_IndexState())
    {
        ifstream in("Staff_PIndex.txt",ios::in);
        Staff_PIndex temp;
        if(in.good())
        {
            in.seekg(0,ios::beg);
            Staff_index = 0;
            while(!in.eof())
            {
                in.read((char*) &temp,sizeof(temp));
                if(in.eof())
                    break;
                Sta_PIndex[Staff_index++] = temp;
            }
            in.close();
            sort_Staff_PIndex();
        }
    }
    else
        reocnstruct_Staff_PIndex();
}

void Staff::sort_Staff_SIndex()
{
    int  j;
    char Name[50];
    Staff_SIndex temp;
    for(int i = 1; i<Staff_S_index; i++)
    {
        strcpy(Name,Sta_SIndex[i].Name);
        strcpy(temp.Name,Sta_SIndex[i].Name);
        temp.pointer = Sta_SIndex[i].pointer;
        j = i-1;
        while( (j >= 0 ) && strcmp(Sta_SIndex[j].Name, temp.Name ) > 0)
        {
            strcpy( Sta_SIndex[j+1].Name, Sta_SIndex[j].Name );
            Sta_SIndex[j+1].pointer = Sta_SIndex[j].pointer;
            j--;
        }
        Sta_SIndex[j+1] = temp;
    }

}


void Staff::load_Staff_SIndex()
{
    if(!get_Staff_IndexState())
    {
        ifstream in("Staff_SIndex.txt",ios::in);
        Staff_SIndex temp;
        if(in.good())
        {
            in.seekg(0,ios::beg);
            Staff_S_index = 0;
            Staff_size =0;
            while(!in.eof())
            {
                Staff_size++;
                in.read((char*) &temp,sizeof(temp));
                if(in.eof())
                    break;
                Sta_SIndex[Staff_S_index++] = temp;
            }
            in.close();
            sort_Staff_SIndex();
        }

    }
    else
        reocnstruct_Staff_SIndex();
}

void Staff::save_Staff_PIndex()
{
    ofstream out("Staff_PIndex.txt",ios::out | ios::trunc);
    if(out.good())
    {
        Staff_PIndex Staff;
        for(int i=0 ; i<Staff_index; i++)
        {
            Staff = Sta_PIndex[i];
            out.write((char*)& (Staff),sizeof(Staff));
        }
        out.close();
        change_Staff_IndexState(0);
    }
}

void Staff::save_Staff_SIndex()
{
    ofstream out("Staff_SIndex.txt",ios::out | ios::trunc);
    if(out.good())
    {
        Staff_SIndex Staff;
        for(int i=0 ; i<Staff_S_index; i++)
        {
            Staff = Sta_SIndex[i];
            out.write((char*)& (Staff),sizeof(Staff));
        }
        out.close();
        change_Staff_IndexState(0);
    }
}

// sort Primary Index key (ID) ascending
void Staff::sort_Staff_PIndex()
{
    int  j;
    char id[13];
    Staff_PIndex temp;
    for(int i = 1; i<Staff_index; i++)
    {
        strcpy(id,Sta_PIndex[i].ID);
        strcpy(temp.ID,Sta_PIndex[i].ID);
        temp.offset = Sta_PIndex[i].offset;
        j = i-1;
        while( (j >= 0 ) && strcmp(Sta_PIndex[j].ID, temp.ID ) > 0)
        {
            strcpy( Sta_PIndex[j+1].ID, Sta_PIndex[j].ID );
            Sta_PIndex[j+1].offset = Sta_PIndex[j].offset;
            j--;
        }
        Sta_PIndex[j+1] = temp;
    }
}

void Staff::update_record_InvertedList(Staff_InvertedList invertupdate,int address)
{
    fstream Invert_file("Staff_InvertedList.txt",ios::out | ios::in);
    Invert_file.seekp((address * sizeof(invertupdate) ),ios::beg);
    Invert_file.write((char*)& invertupdate, sizeof (invertupdate));
    Invert_file.close();
}

void Staff::appendTo_Staff_InvertedList(Staff_InvertedList invert)
{
    ofstream file("Staff_InvertedList.txt",ios::out | ios::app);
    if(file.good())
    {
        file.seekp( 0,ios::end);
        file.write((char*)&invert, sizeof(invert));
    }
    file.close();
}

void Staff::update_Staff_SIndex(int next)
{
    ifstream file("Staff_InvertedList.txt",ios::in);
    Staff_InvertedList invertupdate;
    Staff_InvertedList invertTemp;
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
                invertupdate.next = Staff_index;
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

void Staff::write_Staff_SIndex( char Name[], char ID[])
{
    Staff_SIndex temp;
    Staff_InvertedList invert;
    strcpy(temp.Name,Name);
    strcpy(invert.ID, ID);
    invert.next = -1;
    int next = checkNameSIndex(Name);
    if(next == -1)
    {
        temp.pointer = Staff_size;
        Sta_SIndex[Staff_S_index] = temp;
        Staff_S_index++;
    }
    else
    {
        update_Staff_SIndex(next);
    }
    appendTo_Staff_InvertedList(invert);
}

int Staff::checkNameSIndex(char Name[])
{
    int pointer = -1;
    for(int i=0 ; i<Staff_S_index; i++)
    {
        if( strcmp( Sta_SIndex[i].Name, Name) == 0)
        {
            pointer = Sta_SIndex[i].pointer;
            break;
        }
    }
    return pointer;
}

void Staff::add_Staff()
{
    Staff Staff;
    Department d;
    cout<<"Enter Staff ID : ";
    cin.getline(Staff.ID,13);
    cout<<"Enter Staff Name : ";
    cin.getline(Staff.Name,50);
    cout<<"Enter Staff Degree : ";
    cin.getline(Staff.Degree,50);
    cout<<"Enter Staff Dept_ID : ";
    cin.getline(Staff.Dept_ID,13);

    //int found = d.checkNameSIndex(Staff.Dept_ID);

    //if(found != -1 )
    //{
        change_Staff_IndexState(1);
        ofstream out("Staff_data.txt",ios::out | ios::app);
        out.seekp(0,ios::end);
        int address=out.tellp();
        char* buffer = new char[150];
        strcpy(buffer,Staff.ID);
        strcat(buffer,"|");
        strcat(buffer,Staff.Name);
        strcat(buffer,"|");
        strcat(buffer,Staff.Degree);
        strcat(buffer,"|");
        strcat(buffer,Staff.Dept_ID);
        strcat(buffer,"|");
        strcat(buffer,"\n");
        short len = strlen(buffer);
        out.write((char*)&"$", sizeof(char));
        out.write(buffer, len);

        //save to Primary index array
        Staff_PIndex temp1 ;
        strcpy(temp1.ID,Staff.ID);
        temp1.offset=address;
        Sta_PIndex[Staff_index] = temp1;

        //save to Secondary index array
        write_Staff_SIndex(Staff.Name, Staff.ID);

        Staff_index++;
        Staff_size++;
        sort_Staff_PIndex();
        sort_Staff_SIndex();
        delete [] buffer;

        out.close();
    //}
}


void Staff::printAll_Staff()
{
    ifstream in("Staff_data.txt",ios::in);
    Staff Staff;
    char ch;
    in.seekg(0,ios::beg);
    char record[150];
    if(in.good())
    {
        while(!in.eof())
        {
            in.read ((char*) &ch, sizeof (char));
            if(in.eof())
                break;
            if(ch=='*')
            {
                in.getline(record, 150);
                continue;
            }
            in.getline(record, 150);  // delim defaults to '\n'
            istrstream buff(record);
            buff.getline(Staff.ID,13,'|');
            buff.getline(Staff.Name,50,'|');
            buff.getline(Staff.Degree,50,'|');
            buff.getline(Staff.Dept_ID,13,'|');
            cout<<"Staff ID      : "<<Staff.ID<<endl;
            cout<<"Staff Name    : "<<Staff.Name<<endl;
            cout<<"Staff Degree  : "<<Staff.Degree<<endl;
            cout<<"Staff Dept_ID : "<<Staff.Dept_ID<<endl;
            cout<<endl;
        }
    }
    in.close();

}

Staff Staff::get_Staff(int offset)
{
    ifstream in("Staff_data.txt",ios::in);
    Staff staff;
    char ch;
    in.seekg(offset,ios::beg);
    char record[150];
    if(in.good())
    {
        in.read ((char*) &ch, sizeof (char));
        if(ch !='*')
        {
            in.getline(record, 150);  // delim defaults to '\n'
            istrstream buff(record);
            buff.getline(staff.ID,13,'|');
            buff.getline(staff.Name,50,'|');
            buff.getline(staff.Degree,50,'|');
            buff.getline(staff.Dept_ID,13,'|');
        }
    }
    in.close();
    return staff;
}
/*
void Staff::printInverted()
{
    ifstream in("Staff_InvertedList.txt",ios::in);
    in.seekg(0,ios::beg);
    Staff_InvertedList invert;
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
int Staff::StaffPIndexBinarySearch(char id[])
{
    int low=0 ;
    int high=Staff_index;
    while(low<=high)
    {
        int mid=(low+high)/2;
        if( strcmp( Sta_PIndex[mid].ID, id) == 0)
            return mid;
        else if( strcmp(Sta_PIndex[mid].ID,id)<0)
            low=mid+1;
        else
            high=mid-1;
    }
    return -1 ;
}

void Staff::Delete_Staff(char id[])
{
    change_Staff_IndexState(1);
    fstream file("Staff_data.txt",ios::out | ios::in);
    char ch;
    int curr_index= StaffPIndexBinarySearch(id);
    if(curr_index==-1)
    {
        cout<<"Not found !"<<endl;
        return;
    }
    int offset = Sta_PIndex[curr_index].offset;
    file.seekg(offset,ios::beg);
    file.read((char*)&ch, sizeof(ch));
    if(file.eof())
        return;
    file.seekp(offset,ios::beg);
    file.write((char*)&"*",sizeof(char));
    file.close();
    cout << "Staff Deleted" << endl;
    // shift PIndex
    for(int i=curr_index ; i<Staff_index-1; i++)
    {
        Staff_PIndex temp =Sta_PIndex[i+1];
        Sta_PIndex[i]=temp;
    }
    Staff_index--;
}

void Staff::increament_Counter(char id[])
{
    for(int i=0 ; i<dept_counter.size(); i++)
    {
        if(strcmp(dept_counter.at(i).ID , id)==0)
        {
            Dept_Counter c ;
            strcpy(c.ID , dept_counter[i].ID);
            c.counter = dept_counter[i].counter+1;
            dept_counter[i] = c;
        }
    }
}

void Staff::get_Dept_With_Max_Number_of_staff()
{

    Department p ;
    int d_size = p.get_D_index();
    dept_counter = p.get_Dept_IDs();
    for(int i=0 ; i<Staff_index; i++)
    {
        Staff st = st.get_Staff(Sta_PIndex[i].offset);
        increament_Counter( st.Dept_ID);
    }
    if(dept_counter.size()>0)
    {
        Dept_Counter max_element = dept_counter.at(0);
        for(int i=1 ; i<dept_counter.size(); i++)
        {
            if(dept_counter.at(i).counter > max_element.counter)
            {
                max_element = dept_counter.at(i);
            }
        }
        int index = p.DeptPIndexBinarySearch(max_element.ID);
        int offset = p.getOffset(index);
        Department dept = p.get_Dept(offset);
        cout<<"\n"<<p.getName(dept)<<" Department has "<<max_element.counter<<" members\n"<<endl;
    }

}

/*
void Staff::printPIndex()
{
    Staff_PIndex Staff;
    for(int i=0 ; i<Staff_index; i++)
    {
        Staff = Sta_PIndex[i];
        cout<<"Staff ID  : "<<Staff.ID<<" - "<<"Staff offset : "<<Staff.offset<<endl;
    }
}

void Staff::printSIndex()
{
   Staff_SIndex Staff;
    cout<<"================ Staff Secondary Index =========================="<<endl;
    for(int i=0 ; i<Staff_S_index; i++)
    {
        Staff = Sta_SIndex[i];
        cout<<"Staff Name  : "<<Staff.Name<<" - "<<" Pointer : "<<Staff.pointer<<endl;
    }
}
*/

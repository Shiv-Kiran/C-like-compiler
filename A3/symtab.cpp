#include "symtab.hh"
#include <iostream>
using namespace std;
int off = 0;
void SymTab::printgst()
{
    cout<<"[";
    //iterate through the map and print the entries
    for (auto it = Entries.begin(); it != Entries.end(); ++it)
    {
        cout<<"[";
        cout<<"\""<<it->first<<"\",";
        cout<<"\""<<it->second.varfun<<"\",";
        cout<<"\""<<it->second.scope<<"\",";
        cout<<it->second.size<<",";
        cout<<it->second.offset<<",";
        cout<<"\""<<it->second.returntype<<",";
        cout<<"]\n";
        if (next(it,1) != Entries.end()) cout << "," << endl;
    }
    cout<<"]\n";
}

void SymTab::print()
    {
        cout << "[\n";
        // iterate through the map and print the entries
        for (auto it = Entries.begin(); it != Entries.end(); ++it)
        {
            cout << "[";
            cout << "\"" << it->first << "\",";
            cout << "\"" << it->second.varfun << "\",";
            cout << "\"" << it->second.scope << "\",";
            cout << it->second.size << ",";
            cout << it->second.offset << ",";
            cout << "\"" << it->second.returntype << "\"";
            cout << "]\n";
            if (next(it, 1) != Entries.end())
                cout << "," << endl;
        }
        cout << "]\n";
    }

void SymTabEntry::print()
{
cout<<"[";
cout<<"\""<<varfun<<"\",";
cout<<"\""<<scope<<"\",";
cout<<size<<",";
cout<<offset<<",";
cout<<"\""<<returntype<<"\"";
cout<<"] \n";
}


void SymTab::push(std::string name, SymTabEntry* entry, int type)
{
   if(type==2){
    off = 0;
    return;
   }
   if(entry->scope == "param"){
    entry->offset = 12 + entry->offset;
   }
   if(entry->scope == "local"){
    if(type==1){
        entry->offset = -(entry->size + off);
        off += entry->size;
    }
   }
   Entries.insert({name, *entry});

}

int get_size_local(SymTab gst, string name){
    SymTab *temp = gst.Entries[name].symbtab;
    // Loop through *temp and get the size of the variables
    int size = 0;
    for (auto it = temp->Entries.begin(); it != temp->Entries.end(); ++it)
    {
        if (it->second.scope == "local")
        size += it->second.size;
    }
    return size;
}

int get_size_param(SymTab gst, string name){
    SymTab *temp = gst.Entries[name].symbtab;
    // Loop through *temp and get the size of the variables
    int size = 0;
    for (auto it = temp->Entries.begin(); it != temp->Entries.end(); ++it)
    {
        if (it->second.scope == "param"){

            std::string s = it->second.returntype;
            if(s[s.length()-1] ==']' || s[s.length()-1] =='*' ){
                size += 4;
            }else
            size += it->second.size;
        }
    }
    return size;
}

int get_offset(SymTab gst, string fname, string name){

    SymTab *temp = gst.Entries[fname].symbtab;
    int struct_offset = gst.Entries[fname].size;
    // Find name from *temp entries 
    int offset = 0;
    offset = temp->Entries[name].offset;
    int size = temp->Entries[name].size;
    string scope = temp->Entries[name].scope;
    return offset;
}

int get_offset_func(SymTab gst, string fname){

    SymTab *temp = gst.Entries[fname].symbtab;
    // Find name from *temp entries 
    int offset = 0;
    // Get maximum offset from the function
    for (auto it = temp->Entries.begin(); it != temp->Entries.end(); ++it)
    {
        if (it->second.offset > offset)
        {
            offset = it->second.offset;
        }
    }
    return offset;
   
}

int get_struct_size(SymTab gst, string name){
    // check if name is present in gst
    if (gst.Entries.find(name) == gst.Entries.end())
    {
        return 4;
    }

    SymTab *temp = gst.Entries[name].symbtab;
    // Loop through *temp and get the size of the variables
    int size = 0;
    for (auto it = temp->Entries.begin(); it != temp->Entries.end(); ++it)
    {
        size += it->second.size;
    }
    return size;
}

string get_struct_type(SymTab gst, string fname, string varname){
    SymTab *temp = gst.Entries[fname].symbtab;
    // Loop through *temp and get the size of the variables
    string type ;
    for (auto it = temp->Entries.begin(); it != temp->Entries.end(); ++it)
    {
        if (it->first == varname)
        {
            type = it->second.returntype;
        }
    }
    return type;
}

int get_return_size(SymTab  gst, string fname){
    
    string return_type = gst.Entries[fname].returntype;
    if(return_type.substr(0, 6) != "struct") return 4;
    return gst.Entries[return_type].size;
}

// int get_offset_param(SymTab gst, string fname, string name){

//     SymTab *temp = gst.Entries[fname].symbtab;
//     // Find name from *temp entries 
//     int offset = 0;
//     offset = temp->Entries[name].offset;
//     int size = temp->Entries[name].size;
//     return offset - size;
// }

// bool SymTab::search(std::string name)
// {
//     if (Entries.find(name) != Entries.end())
//     {
//         return true;
//     }
//     else
//     {
//         return false;
//     }
// }

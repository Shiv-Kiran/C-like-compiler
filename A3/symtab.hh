#ifndef SYMTAB_HH
#define SYMTAB_HH
#include <string>
#include <map>
#include <iostream>
using namespace std;

class SymTabEntry;
class SymTab;


int get_size_local(SymTab gst, string name);
int get_size_param(SymTab gst, string name);
int get_offset(SymTab gst, string fname, string name);
int get_offset_func(SymTab gst, string fname);
// int get_offset_func(SymTab gst, string fname);
int get_struct_size(SymTab gst, string name);
// int get_offset_param(SymTab gst, string fname, string name);
string get_struct_type(SymTab gst, string fname, string varname);
int get_return_size(SymTab  gst, string fname);

class SymTab
{
public:
    std::map<std::string, SymTabEntry> Entries;
    void printgst();
    void print();
    void push(std::string name, SymTabEntry* entry, int type);
};

class SymTabEntry
{
    // std::string name;
public:
    std::string varfun;
    std::string scope;
    int size;
    int offset;
    std::string returntype;
    SymTab *symbtab;
    void print();
    SymTabEntry(string varfun, string scope, int size, int offset, string returntype, SymTab *symbtab){
        this->varfun = varfun;
        this->scope = scope;
        this->size = size;
        this->offset = offset;
        this->returntype = returntype;
        this->symbtab = symbtab;
    }
    SymTabEntry(string varfun, string scope, string returntype, SymTab *symbtab){
        this->varfun = varfun;
        this->scope = scope;
        this->size = 0;
        this->offset = 0;
        this->returntype = returntype;
        this->symbtab = symbtab;
    }
    SymTabEntry(){
        this->varfun = "";
        this->scope = "";
        this->size = 0;
        this->offset = 0;
        this->returntype = "";
        this->symbtab = NULL;
    }
};
#endif

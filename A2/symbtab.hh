#ifndef SYMBTAB_HH
#define SYMBTAB_HH
#include <string>
#include <map>
#include <iostream> 
using namespace std;

class SymbTabEntry;

bool isPoin(string type);
bool iscompatible(string type1, string type2);
bool isrelacompat(string type1, string type2);
string totype(string type1, string type2);
string gettype(string s);

class SymbTab{
    public:
    std::map<std::string, SymbTabEntry> Entries;
    void printgst();
    void print();
    void push(string name, SymbTabEntry *e, int k, int i);
    int getStructSize(string type_name);
    string getrettype(string name, int i);
    string getrettype(string name);
    string gettype(string name, string mem);
    bool isvalid(string name, int n);
    bool isvalidparam(string name, string param, int k);
    bool isvalStruct(string name, string member);
    bool isPresent(string name);
    SymbTab();
};

class SymbTabEntry{
    public: 
    std::string varfun;
    std::string scope;
    int size; 
    int offset; 
    int paramnum = 0;
    std::string returntype;
    SymbTab* symbtab;
    void print();
    SymbTabEntry(string varfun, string scope, string returntype, SymbTab* symbtab);
    SymbTabEntry(string varfun, string scope, int size, int offset, string returntype, SymbTab* symbtab);

};

#endif
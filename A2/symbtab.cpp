#include "symbtab.hh"
#include <iostream> 
#include <vector>
using namespace std;

int off = 0;
vector<string> inbuilt {"printf", "scanf", "void"}; 
void SymbTab::printgst(){
    cout<<"[";
    for(std::map<std::string, SymbTabEntry>::iterator it = Entries.begin(); it != Entries.end();++it )
    {
        cout<<"[  ";
        cout<<"\""<<it->first<<"\",";
        cout<<"\""<<it->second.varfun<<"\",";
        cout<<"\""<<it->second.scope<<"\",";
        cout<<it->second.size<<",    ";
        if(it->second.varfun =="struct") cout<<"\"-\""<<",      ";
        else
        cout<<it->second.offset<<",     ";
        cout<<"\""<<it->second.returntype<<"\"\n]";
        // std::map<std::string, SymbTabEntry>::iterator temp = it
        // if (++it != Entries.end()) cout<<","<<endl;
        if (next(it, 1) != Entries.end()) cout<<",\n"<<endl;

    }
    cout<<"\n]\n";

};

SymbTab::SymbTab(){
    
}

void SymbTab::print(){

    cout<<"[";
    for(std::map<std::string, SymbTabEntry>::iterator it = Entries.begin(); it != Entries.end();++it )
    {
        cout<<"[  ";
        cout<<"\""<<it->first<<"\",";
        cout<<"\""<<it->second.varfun<<"\",";
        cout<<"\""<<it->second.scope<<"\",";
        cout<<it->second.size<<",    ";
        cout<<it->second.offset<<",     ";
        cout<<"\""<<it->second.returntype<<"\"\n]";
        // std::map<std::string, SymbTabEntry>::iterator temp = it
        // if (++it != Entries.end()) cout<<","<<endl;
        if (next(it, 1) != Entries.end()) cout<<"\n,\n"<<endl;

    }
    cout<<"\n]\n";

};

void SymbTabEntry::print(){

    cout<<"[\n";
    cout<<"\""<<varfun<<"\",";
    cout<<"\""<<scope<<"\",";
    cout<<size<<"\",";
    cout<<offset<<"\",";
    cout<<"\""<<returntype<<"\",";
    cout<<"]\n";
};

SymbTabEntry::SymbTabEntry(string varfun, string scope, string returntype, SymbTab* symbtab){
    this->varfun = varfun;
    this->scope = scope;
    this->returntype = returntype;
    this->symbtab = symbtab;
    this->size = 0;
    this->offset = 0;
}

SymbTabEntry::SymbTabEntry(string varfun, string scope, int size, int offset,  string returntype, SymbTab* symbtab){
    this->varfun = varfun;
    this->scope = scope;
    this->returntype = returntype;
    this->symbtab = symbtab;
    this->size = size;
    this->offset = offset;
}

int SymbTab::getStructSize(string name){
    if (Entries.find(name) != Entries.end()) return Entries.find(name)->second.size;
    else return 0;
}

string SymbTab::getrettype(string name, int i ){
    SymbTab* st;
    if (Entries.find(name) != Entries.end()) {
        auto k = Entries.find(name);
        st = k->second.symbtab;
        for(auto j = st->Entries.begin(); j!= st->Entries.end(); ++j){
            if(j->second.scope=="param"){
                if(j->second.paramnum == i)return j->second.returntype;
            }
        }
    }
}


string SymbTab::getrettype(string name){
   if (name ==inbuilt[0] || name ==inbuilt[1] ) return "void";
    if(name == inbuilt[2])return "int";
    SymbTab* st;
    if (Entries.find(name) != Entries.end()) {
        auto k = Entries.find(name);
        return k->second.returntype;
    }
    return "";
}

string SymbTab::gettype(string name, string mem){
    SymbTab* st;
    if (Entries.find(name) != Entries.end()) {
        st = Entries.find(name)->second.symbtab;
        if(st->Entries.find(mem) != st->Entries.end()) 
            return st->Entries.find(mem)->second.returntype;
    }
    return "";
}

bool SymbTab::isPresent(string name){
    if (Entries.find(name) != Entries.end()) return true;
    else return false;
}

void SymbTab::push(string name, SymbTabEntry *e, int init, int i){
    if(init==2){
        off = 0;
        return ;
    }
    if (e->scope == "param"){
        e->paramnum = i;
        e->offset = 12 + e->offset;
    }
    if(e->scope == "local"){
        if(init==1){
            e->offset = -(e->size + off);
            off += e->size;
        }
    }
    Entries.insert({name, *e});
};

string gettype(string s){
    if(s[s.length()-1]=='*')
        return s.substr(0,s.length()-1);
    else{
        if(s[s.length()-1] ==']'){
            int p = 0;
            for(int i = s.size()-2; i>=0;i--){
                if(s[i] == '[')break;
                p++;
            }
            return s.substr(0,s.length()-p-2);
        }
    }
    return "";
}

bool SymbTab::isvalid(string name, int n){
    if (name =="printf" || name =="scanf" ||name =="mod") return true;
    SymbTab* st;
    if (Entries.find(name) != Entries.end()){
        auto i = Entries.find(name);
        if(i->second.varfun != "fun") return false;
        int d = 0;
        st = i->second.symbtab;
        for(auto j = st->Entries.begin(); j!= st->Entries.end(); ++j){
            if(j->second.scope=="param")d++;
        }
        if(d==n) return true;
    }
    return false;
}

bool SymbTab::isvalidparam(string name, string param, int k){
    if (name ==inbuilt[0] || name ==inbuilt[1] ||name ==inbuilt[2]) return true;
    SymbTab* st;
    if (Entries.find(name) != Entries.end()) {
        auto i = Entries.find(name);
        st = i->second.symbtab;
        for(auto j = st->Entries.begin(); j!= st->Entries.end(); ++j){
        // cout<<name<<" "<<param <<" inside "<<j->second.returntype <<endl;
            if(j->second.scope=="param"){
                if(j->second.returntype == "void*" && isPoin(param))
                    if( j->second.paramnum == k) return true;
                if(isrelacompat(param, j->second.returntype))
                    if(j->second.paramnum ==k) return true;
            }
        }
        
    }
    return false;
}

bool SymbTab::isvalStruct(string name, string member){
    SymbTab* st;
    // if (Entries.find(name) != Entries.end()){
    //     auto i = Entries.find(name);
    if (Entries.find(name) != Entries.end()) {
        auto i = Entries.find(name);
        // if(i->second.varfun != "fun") return false;
        st = i->second.symbtab;
        for(auto j = st->Entries.begin(); j!= st->Entries.end(); ++j){
            if(j->first==member) return true;
        }
        
    }
    return false;
}

bool isPoin(string type){
    if(type[type.length()-1] == '*'){
        if(type=="void")return false;
        return true;
    }
    else 
        if (type[type.length()-1] == ']') return true;
    return false;
}

bool iscompatible(string type1, string type2){
    int n1 = 0;
    int n2 = 0;
    while(type1[type1.length()-1] == ']' || type1[type1.length()-1]=='*'){
        n1++;
        type1 = gettype(type1);

    }
    while(type2[type2.length()-1] == ']' || type2[type2.length()-1]=='*'){
        n2++;
        type2 = gettype(type2);
    }
    return n1==n2;
}

bool isrelacompat(string type1, string type2){
    if((type1=="int" && (type2 == "float" || type2 == "int")) )
        return true; 
    if ((type1=="float" && (type2 == "float" || type2 == "int")))
        return true;
    if(iscompatible(type1, type2)) return true;
    return false;
}

string totype(string type1, string type2){
    if(type1 == "float" || type2=="float")return "float";
    if(type1 == "int" && type2=="int")return "int";
    if(type1 == "int" && isPoin(type2))return type2;
    if(type2 == "int" && isPoin(type1))return type1;
    if(isPoin(type1) && isPoin(type2)) return "int";
    return "";
}
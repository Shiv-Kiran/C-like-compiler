#include "type.hh"
using namespace std;
datatype createtype(basic_type type){
    datatype temp;
    switch(type){
        case INT_TYPE:
            temp.type = "int";
            temp.size = 4;
            break;
        case FLOAT_TYPE:
            temp.type = "float";
            temp.size = 4;
            break;
        case VOID_TYPE:
            temp.type = "void";
            temp.size = 0;
            break;
        case CHAR_TYPE:
            temp.type = "char";
            temp.size = 1;
            break;
    }
    return temp;
}

void datatype::print(){
    // cout << type << " " << size << endl;
}


int getSize(type_specifier_class* t, declarator_class* d){
    if(d->type==NULL) return t->type.size;
    else{
        int size = t->type.size;
        std::string s = d->type->type;
        if(s[0] == '*')size = 4;
        while(s[s.length() -1] ==']'){
            int i =1;
            while(s[s.length() - i -1] != '[') i++;
            size = size * stoi(s.substr(s.length() - i, i));
            s = s.substr(0, s.length() - i - 1);
        }
        return size;
    }
}

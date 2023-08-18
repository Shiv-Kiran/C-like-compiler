#include "type.hh"
#include <iostream>

datatype createtype(basic_type type){
    datatype temp;
    switch(type){
        case INT_TYPE:
            temp.type_name = "int";
            temp.size = 4;
            break;
        case FLOAT_TYPE:
            temp.type_name = "float";
            temp.size = 4;
            break;
        case VOID_TYPE:
            temp.type_name = "void";
            temp.size = 0;
            break;
        case CHAR_TYPE:
            temp.type_name = "char";
            temp.size = 1;
            break;
    }
    temp.isPointer = false;
    temp.arrsize = -1;
    temp.next = NULL;
    return temp;
};

datatype::datatype(std::string type_name, int size){
    this->isPointer = false;
    this->arrsize = -1;
    this->next = NULL;
    this->type_name = type_name;
    this->size = size;
}

datatype::datatype(){
    this->isPointer = false;
    this->arrsize = -1;
    this->next = NULL;
    this->type_name = "";
    this->size = 0;
}

type_specifier_class::type_specifier_class(basic_type bas){
    this->type = createtype(bas);
}

type_specifier_class::type_specifier_class(datatype type){
    this->type = type;
};

fun_declarator_class::fun_declarator_class(std::string name, parameter_list_class* parameter_list){
    this->name = name;
    this->parameter_list = parameter_list;
};


parameter_declaration_class::parameter_declaration_class(type_specifier_class* type, declarator_class* declarator){
    this->type = type;
    this->declarator = declarator;
};

declaration_class::declaration_class(type_specifier_class* type, declarator_list_class* declarator_list){
        this->type = type;
        this->declarator_list = declarator_list;
};

int calculateSize(type_specifier_class* t, declarator_class* d){
    if(d->type==NULL) return t->type.size;
    else{
        string s = d->type->type_name;
        int size = t->type.size;
        if(s[0] =='*')size = 4;
        while(s[s.length() -1] == ']'){
            int i=1;
        while(s[s.length() -i -1] != '[') i++;
        size = size * stoi(s.substr(s.length() -i , i));
        s = s.substr(0, s.length() - i-1);
        }
    return size;
    }


}   

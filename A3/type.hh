// #pragma once
#ifndef TYPE_HH
#define TYPE_HH

#include <vector> 
#include <string>

class datatype{
    // this datatype should be able to handle many types of types such as pointer to a datatype , array of a datatype , 
public:
    std::string type;
    int size;
    void print();
    datatype(){
        this->type = "";
        this->size = 0;
    }
};

// create a enum of basic types such as int , float , void , char
enum basic_type{
    INT_TYPE,
    FLOAT_TYPE,
    VOID_TYPE,
    CHAR_TYPE
};

datatype createtype(basic_type type);


class type_specifier_class{

    public:
    datatype type;
};

class declarator_class{

    public:
    std::string name;
    datatype* type;
    
};

class declarator_list_class{

    public:
    std::vector<declarator_class*> declarator_list;
};
class declaration_class{

    public:
    type_specifier_class* type_specifier;
    declarator_list_class* declarator_list;
    declaration_class(type_specifier_class* type_specifier,declarator_list_class* declarator_list){
        this->type_specifier = type_specifier;
        this->declarator_list = declarator_list;
    }
};



class declaration_list_class{

    public:
    std::vector<declaration_class*> declaration_list;
};



class fun_declarator_class{

    public:
    std::string name;
    declarator_list_class declarator_list;
    datatype type;
};
class parameter_declaration_class{
    public:
    type_specifier_class* type_specifier;
    declarator_class* declarator;
    parameter_declaration_class(type_specifier_class* type_specifier,declarator_class* declarator){
        this->type_specifier = type_specifier;
        this->declarator = declarator;
    }
};
class parameter_list_class{

    public:
    std::vector<parameter_declaration_class* > parameter_list;
    parameter_list_class(){
        this->parameter_list = std::vector<parameter_declaration_class* >();
    }
};

int getSize(type_specifier_class* t, declarator_class* d);

#endif


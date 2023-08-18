#ifndef TYPE_HH
#define TYPE_HH

#include <string> 
#include <vector> 

using namespace std;

class datatype{
    public:
    std::string type_name;
    int size;
    datatype* next;
    bool isPointer;
    int arrsize;
    void print();
    datatype(std::string type_name, int size);
    datatype();
};

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
    type_specifier_class(basic_type bas);
    type_specifier_class(datatype type);
    type_specifier_class(){}
};

class declarator_class{
    public:
    std::string name;
    datatype* type;
    declarator_class(){
        this->name = "";
        this->type = new datatype();
    }
};


class declarator_list_class{
    public:
        std::vector<declarator_class*> declarator_list;
    // declarator_list_class(std::vector<declarator_class*> declarator_list ){
    //     this-> declarator_list = declarator_list;
    // };
    // declarator_list_class();
};

class declaration_class{
    public:
    type_specifier_class* type;
    declarator_list_class* declarator_list;
    declaration_class(type_specifier_class* type, declarator_list_class* declarator_list);
};

class parameter_declaration_class{
    public:
    type_specifier_class* type;
    declarator_class* declarator;
    parameter_declaration_class(type_specifier_class* type, declarator_class* declarator);
};

class declaration_list_class{
    public:
    std::vector<declaration_class*> declaration_list;
};

class parameter_list_class{
    public:
    std::vector<parameter_declaration_class*> parameter_list;
};

class fun_declarator_class{
    public:
    std::string name;
    parameter_list_class* parameter_list;
    datatype type;
    fun_declarator_class(std::string name, parameter_list_class* parameter_list);
};

int calculateSize( type_specifier_class* type_s, declarator_class* decl);


#endif


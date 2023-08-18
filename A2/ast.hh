#ifndef AST_HH 
#define AST_HH 
#include <vector> 
#include <string> 
enum typeExp
{
    Int_type, 
};

using namespace std;

class abstract_astnode{
    public: 
    virtual void print(int blanks) = 0;
    enum typeExp astnode_type;
    // bool lvalue = false;
    // bool is_calculable = false;
};

class statement_astnode : public abstract_astnode
{
    public: 
    virtual void print(int blanks) = 0;
}; 

class exp_astnode : public abstract_astnode{
    public: 
    std::string type = "";
    int value1 = 1;
    bool ismod = true;
    virtual void print(int blanks ) =0;
};

class ref_astnode : public exp_astnode{
    public:
    virtual void print(int blanks ) =0;
    // bool lvalue;
    // ref_astnode();
}; 

class empty_astnode : public statement_astnode{
    public:
    void print(int blanks);
    empty_astnode();
}; 

class seq_astnode : public statement_astnode{
    public: 
    std::vector<statement_astnode*> statements;
    void print(int blanks);
    seq_astnode(vector<statement_astnode*> statements);
    seq_astnode();
};

class assignS_astnode : public statement_astnode{
    public: 
    exp_astnode *left;
    exp_astnode *right;
    void print(int blanks);
    assignS_astnode(exp_astnode* left, exp_astnode* right );
};

class return_astnode : public statement_astnode{
    public: 
    exp_astnode* exp;
    void print(int blanks);
    return_astnode(exp_astnode* exp);
};

class identifier_astnode : public ref_astnode{
    public: 
    std::string name;
    void print(int blanks);
    identifier_astnode(std::string name);
};

class if_astnode : public statement_astnode{
    public: 
    exp_astnode* condition;
    statement_astnode* then;
    statement_astnode* elsestat;
    void print(int blanks);
    if_astnode(exp_astnode* condition, statement_astnode* then, statement_astnode* elsestat);
};

class while_astnode : public statement_astnode{
    public:
    exp_astnode* condition;
    statement_astnode* body;
    void print(int blanks);
    while_astnode(exp_astnode* condition, statement_astnode* body);
};

class for_astnode : public statement_astnode{
    public: 
    exp_astnode* init;
    exp_astnode* condition;
    exp_astnode* update;
    statement_astnode* body;
    void print(int blanks);
    for_astnode(exp_astnode* init, exp_astnode* condition, exp_astnode* update, statement_astnode* body);
};

class proccall_astnode : public statement_astnode{
    public: 
    identifier_astnode* name;
    std::vector<exp_astnode*> args;
    void print(int blanks);
    proccall_astnode(identifier_astnode* name, std::vector<exp_astnode*> args);
    proccall_astnode(identifier_astnode* name);

};

class op_binary_astnode : public exp_astnode{
    public: 
    std::string op;
    exp_astnode* left;
    exp_astnode* right;
    void print(int blanks);
    op_binary_astnode(std::string op, exp_astnode* left, exp_astnode* right);
};

class op_unary_astnode : public exp_astnode{
    public:
    std::string op;
    exp_astnode* exp;
    void print(int blanks);
    op_unary_astnode(std::string op, exp_astnode* exp);
};

class assignE_astnode : public exp_astnode {
    public: 
    exp_astnode* left;
    exp_astnode* right;
    void print(int blanks);
    assignE_astnode(exp_astnode* left, exp_astnode* right);
};

class funcall_astnode : public exp_astnode {
    public: 
    identifier_astnode* name;
    std::vector<exp_astnode*> args;
    void print(int blanks);
    funcall_astnode(identifier_astnode* name, std::vector<exp_astnode*> args); 
    funcall_astnode(identifier_astnode* name);
};

class intconst_astnode : public exp_astnode{
    public: 
    int value;
    void print(int blanks);
    intconst_astnode(int value);
};

class floatconst_astnode : public exp_astnode{
    public:
    float value;
    void print(int blanks);
    floatconst_astnode(float value);
};

class stringconst_astnode : public exp_astnode{
    public:
    std::string value;
    void print(int blanks);
    stringconst_astnode(std::string value);
};

class arrayref_astnode : public ref_astnode {
    public: 
    exp_astnode* array;
    exp_astnode* index;
    void print(int blanks);
    arrayref_astnode(exp_astnode* array, exp_astnode* index);
};

class member_astnode : public ref_astnode {
    public: 
    exp_astnode* exp;
    identifier_astnode* member;
    void print(int blanks);
    member_astnode(exp_astnode* exp, identifier_astnode* member);
};

class arrow_astnode : public ref_astnode {
    public: 
    exp_astnode* exp;
    identifier_astnode* member; 
    void print(int blanks);
    arrow_astnode(exp_astnode* exp, identifier_astnode* member);
    arrow_astnode( identifier_astnode* member);

};

#endif




#ifndef AST_HH
#define AST_HH
#include<vector>
#include<string>
enum typeExp
{
    Empty_astnode,
    Seq_astnode,
    AssignS_astnode,
    Return_astnode,
    If_astnode,
    While_astnode,
    For_astnode,
    Proccall_astnode,
    Op_binary_astnode,
    Op_unary_astnode,
    AssignE_astnode,
    Funcall_astnode,
    Intconst_astnode,
    Floatconst_astnode,
    Stringconst_astnode,
    IDENTIFIER_astnode,
    Arrayref_astnode,
    Member_astnode,
    Arrow_astnode,
};


using namespace std;
class abstract_astnode
{
public:
    virtual void print(int blanks) = 0;
    virtual void gen_code() = 0;
    enum typeExp astnode_type;
};

class statement_astnode : public abstract_astnode
{
public:
    virtual void print(int blanks) = 0;
    virtual void gen_code() = 0;
// protected:
//     statement_astnode(enum typeExp astnode_type);
};

class exp_astnode : public abstract_astnode
{
public:
    virtual void print(int blanks) = 0;
    virtual void gen_code() = 0;
    virtual int get_address() { return 0;};
    virtual std::string  get_name() {return "";};
    
// protected:
//     exp_astnode(enum typeExp astnode_type);
};

class ref_astnode : public exp_astnode
{
public:
    virtual void print(int blanks) = 0;
    virtual void gen_code() = 0;
    virtual int get_address() { return 0;};
    virtual std::string  get_name() {return "";};


// protected:
//     ref_astnode(enum typeExp astnode_type);
};

class empty_astnode : public statement_astnode
{
public:
    void print(int blanks);
    void gen_code();
    empty_astnode();
};

class identifier_astnode : public ref_astnode
{
public:
    std::string name;
    void print(int blanks);
    void gen_code();
    int get_address();
    std::string get_name();

    identifier_astnode(std::string name);
};

class seq_astnode : public statement_astnode
{
public:
    std::vector<statement_astnode*> statements;
    void print(int blanks);
    void gen_code();

    seq_astnode(statement_astnode* statement){
        this->statements = vector<statement_astnode*>();
        this->statements.push_back(statement);
    }
    seq_astnode(){
        this->statements = vector<statement_astnode*>();
    }
};

class assignS_astnode : public statement_astnode
{
public:
    exp_astnode* exp;
    void print(int blanks);
    void gen_code();

    assignS_astnode(exp_astnode* exp){
        this->exp = exp;
    }
};

class return_astnode : public statement_astnode
{
public:
    exp_astnode* exp;
    void print(int blanks);
    void gen_code();

    return_astnode(exp_astnode* exp);
};

class if_astnode : public statement_astnode
{
public:
    exp_astnode* condition;
    statement_astnode* then;
    statement_astnode* elsee;
    void print(int blanks);
    void gen_code();

    if_astnode(exp_astnode* condition,statement_astnode* then,statement_astnode* elsee);
};

class while_astnode : public statement_astnode
{
public:
    exp_astnode* condition;
    statement_astnode* body;
    void print(int blanks);
    void gen_code();
    while_astnode(exp_astnode* condition,statement_astnode* body);
};

class for_astnode : public statement_astnode
{
public:
    exp_astnode* init;
    exp_astnode* condition;
    exp_astnode* update;    
    statement_astnode* body;
    void print(int blanks);
    void gen_code();
    for_astnode(exp_astnode* init,exp_astnode* condition,exp_astnode* update,statement_astnode* body);
};


class proccall_astnode : public statement_astnode
{
public:
    identifier_astnode* name;
    std::vector<exp_astnode*> args;
    void print(int blanks);
    void gen_code();
    proccall_astnode(identifier_astnode* name,std::vector<exp_astnode*>args)
    {
        this->name = name;
        this->args = args;
    }

};



class op_binary_astnode : public exp_astnode
{
public:
    std::string op;
    exp_astnode* left;
    exp_astnode* right;
    void print(int blanks);
    void gen_code();
    
    op_binary_astnode(std::string op,exp_astnode* left,exp_astnode* right);
};

class op_unary_astnode : public exp_astnode
{
public:
    std::string op;
    exp_astnode* exp;
    void print(int blanks);
    void gen_code();
    int get_address();

    op_unary_astnode(std::string op,exp_astnode* exp);
};

class assignE_astnode : public exp_astnode
{
public:
    exp_astnode* left;
    exp_astnode* right;
    void print(int blanks);
    void gen_code();

    assignE_astnode(exp_astnode* left,exp_astnode* right);
};

class funcall_astnode : public exp_astnode
{
public:
    identifier_astnode* name;    
    std::vector<exp_astnode*> args;
    void print(int blanks);
    void gen_code();

    funcall_astnode(identifier_astnode* name,std::vector<exp_astnode*>args){
        this->name = name;
        this->args = args;
    }
};

class intconst_astnode : public exp_astnode
{
public:
    int value;
    void print(int blanks);
    void gen_code();

    intconst_astnode(int value){
        this->value = value;    
    }
};


class stringconst_astnode : public exp_astnode
{
public:
    std::string value;
    void print(int blanks);
    void gen_code();

    stringconst_astnode(std::string value);
};


class arrayref_astnode : public ref_astnode
{
public:
    exp_astnode* array;
    exp_astnode* index;
    void print(int blanks);
    void gen_code();
    int get_address();
    std::string get_name();

    arrayref_astnode(exp_astnode* array,exp_astnode* index){
        this->array = array;
        this->index = index;
        this->astnode_type = typeExp::Arrayref_astnode;
    }
};

class member_astnode : public ref_astnode
{
public:
    exp_astnode* exp;
    identifier_astnode* member;
    void print(int blanks);
    void gen_code();
    int get_address();
    std::string get_name();

    member_astnode(exp_astnode* exp,identifier_astnode* member){
        this->astnode_type = typeExp::Member_astnode;
        this->exp = exp;
        this->member = member;
    }
};

class arrow_astnode : public ref_astnode
{
public:
    exp_astnode* exp;
    identifier_astnode* member;
    void print(int blanks);
    void gen_code();
    int get_address();
    std::string get_name();


    arrow_astnode(exp_astnode* exp,identifier_astnode* member){
        this->exp = exp;
        this->member = member;
        this->astnode_type = typeExp::Arrow_astnode;
    }
};

class printf_astnode: public statement_astnode
{
    public: 
        std::string format;
        std::vector<exp_astnode*> args;
        void print(int blanks);
        void gen_code();

        printf_astnode(std::string format,std::vector<exp_astnode*> args){
            this->format = format;
            this->args = args;
        }
};

#endif





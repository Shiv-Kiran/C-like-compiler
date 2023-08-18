#include "ast.hh"
#include <iostream>
#include <cstdarg>
using namespace std;

void printAst(const char *astname, const char *fmt...) // fmt is a format string that tells about the type of the arguments.
{   
	typedef vector<abstract_astnode *>* pv;
	va_list args;
	va_start(args, fmt);
	if ((astname != NULL) && (astname[0] != '\0'))
	{
		cout << "{ ";
		cout << "\"" << astname << "\"" << ": ";
	}
	cout << "{" << endl;
	while (*fmt != '\0')
	{
		if (*fmt == 'a')
		{
			char * field = va_arg(args, char *);
			abstract_astnode *a = va_arg(args, abstract_astnode *);
			cout << "\"" << field << "\": " << endl;
			
			a->print(0);
		}
		else if (*fmt == 's')
		{
			char * field = va_arg(args, char *);
			char *str = va_arg(args, char *);
			cout << "\"" << field << "\": ";

			cout << str << endl;
		}
		else if (*fmt == 'i')
		{
			char * field = va_arg(args, char *);
			int i = va_arg(args, int);
			cout << "\"" << field << "\": ";

			cout << i;
		}
		else if (*fmt == 'f')
		{
			char * field = va_arg(args, char *);
			double f = va_arg(args, double);
			cout << "\"" << field << "\": ";
			cout << f;
		}
		else if (*fmt == 'l')
		{
			char * field = va_arg(args, char *);
			pv f =  va_arg(args, pv);
			cout << "\"" << field << "\": ";
			cout << "[" << endl;
			for (int i = 0; i < (int)f->size(); ++i)
			{
				(*f)[i]->print(0);
				if (i < (int)f->size() - 1)
					cout << "," << endl;
				else
					cout << endl;
			}
			cout << endl;
			cout << "]" << endl;
		}
		++fmt;
		if (*fmt != '\0')
			cout << "," << endl;
	}
	cout << "}" << endl;
	if ((astname != NULL) && (astname[0] != '\0'))
		cout << "}" << endl;
	va_end(args);
}

empty_astnode::empty_astnode(){
    this->astnode_type = Int_type;
};

void empty_astnode::print(int blanks){
    cout << "\"empty\"" <<endl;
};

seq_astnode::seq_astnode(vector<statement_astnode*> s){
    this->astnode_type = Int_type;
    this->statements = s;
};

seq_astnode::seq_astnode(){
    this->astnode_type = Int_type;
    this->statements = vector<statement_astnode*>();
}


void seq_astnode::print(int blanks){
    printAst("", "l", "seq", statements);
};

assignS_astnode::assignS_astnode(exp_astnode* left, exp_astnode*right){
    this->astnode_type = Int_type;
    this->left = left;
    this->right = right;
};

void assignS_astnode::print( int blanks){
    printAst("assignS", "aa", "left",left, "right", right);
};

return_astnode::return_astnode(exp_astnode *exp){
    this->astnode_type = Int_type;
    this->exp = exp;
};

void return_astnode::print(int blanks){
    printAst("", "a", "return", exp);
};

if_astnode::if_astnode(exp_astnode* condition, statement_astnode* then, statement_astnode* elsestat){
    this->condition = condition;
    this->then = then; 
    if(elsestat == NULL)
        this->elsestat = new empty_astnode();
    else 
        this->elsestat = elsestat;
};

void if_astnode::print(int blanks){
    printAst("if", "aaa", "cond", condition, "then", then, "else", elsestat);
};

while_astnode::while_astnode(exp_astnode *condition, statement_astnode* body){
    this->astnode_type = Int_type;
    this->condition = condition;
    this->body = body;
};

void while_astnode::print(int blanks){
    printAst("while", "aa", "cond", condition, "stmt", body);
};

for_astnode::for_astnode(exp_astnode* init, exp_astnode* condition, exp_astnode* update, statement_astnode* body){
    this->astnode_type = Int_type;
    this->init = init;
    this->condition = condition;
    this->update = update;
    this->body = body;
};

void for_astnode::print(int blanks){
    printAst("for", "aaaa", "init", init, "guard", condition, "step", update, "body", body);
};

proccall_astnode::proccall_astnode(identifier_astnode* fname, vector<exp_astnode*>args){
    this->astnode_type = Int_type;
    this->name = fname;
    this->args = args;
};
proccall_astnode::proccall_astnode(identifier_astnode* fname){
    this->astnode_type = Int_type;
    this->name = fname;
    this->args = vector<exp_astnode*>();
};

void proccall_astnode::print(int blanks){
    printAst("proccall", "al", "fname", name, "params", args);
};

op_binary_astnode::op_binary_astnode(std::string op, exp_astnode* left, exp_astnode* right){
    this->astnode_type = Int_type;
    this->op = op;
    this->left = left;
    this->right = right;
};

void op_binary_astnode::print(int blanks){
    string str = "\"" + op + "\"";
    char* str1 = const_cast<char*>(str.c_str());
    printAst("op_binary", "saa", "op", str1, "left", left, "right", right);
};

op_unary_astnode::op_unary_astnode(std::string op, exp_astnode* exp){
    this->astnode_type = Int_type;
    this->op = op;
    this->exp = exp;
};

void op_unary_astnode::print(int blanks){
    string str = "\"" + op + "\"";
    char* str1 = const_cast<char*>(str.c_str());
    printAst("op_unary", "sa", "op", str1, "child", exp);
};

assignE_astnode::assignE_astnode(exp_astnode* left, exp_astnode*right){
    this->astnode_type = Int_type;
    this->left = left;
    this->right = right;
};

void assignE_astnode::print( int blanks){
    printAst("assignE", "aa", "left",left, "right", right);
};

funcall_astnode::funcall_astnode(identifier_astnode* name, vector<exp_astnode*>args){
    this->astnode_type = Int_type;
    this->name = name;
    this->args = args;
};

funcall_astnode::funcall_astnode(identifier_astnode* name){
    this->astnode_type = Int_type;
    this->name = name;
    this->args = vector<exp_astnode*>();
};

void funcall_astnode::print(int blanks){
    printAst("funcall", "al", "fname", name, "params", args);
};

intconst_astnode::intconst_astnode(int v){
    this->astnode_type = Int_type;
    this->value = v;
};

void intconst_astnode::print(int blanks){
    printAst("", "i", "intconst", value);
};

floatconst_astnode::floatconst_astnode(float v){
    this->astnode_type = Int_type;
    this->value = v;
};

void floatconst_astnode::print(int blanks){
    printAst("", "f", "floatconst", value);
};

stringconst_astnode::stringconst_astnode(string v){
    this->astnode_type = Int_type;
    this->value = v;
};

void stringconst_astnode::print(int blanks){
    char* str = const_cast<char*>(value.c_str());
    printAst("", "s", "stringconst", str);
};

identifier_astnode::identifier_astnode(std::string name){
    this->astnode_type = Int_type;
    this->name = name;
};

void identifier_astnode::print(int blanks){
    string str = "\"" + name + "\"";
    char* str1 = const_cast<char*>(str.c_str());
    printAst("", "s", "identifier", str1);
};

arrayref_astnode::arrayref_astnode(exp_astnode* array, exp_astnode* index){
    this->astnode_type = Int_type;
    this->array = array;
    this->index = index;
};
void arrayref_astnode::print(int blanks){
    printAst("arrayref","aa", "array", array, "index", index);
};

member_astnode::member_astnode(exp_astnode* exp, identifier_astnode* member){
    this->astnode_type = Int_type;
    this->exp = exp;
    this->member = member;
};

void member_astnode::print(int blanks){
    printAst("member", "aa", "struct", exp, "field", member);
};

arrow_astnode::arrow_astnode(exp_astnode* exp, identifier_astnode* member){
    this->astnode_type = Int_type;
    this->exp = exp;
    this->member = member;
};


void arrow_astnode::print(int blanks){
    printAst("arrow", "aa", "pointer", exp, "field", member);
}; 

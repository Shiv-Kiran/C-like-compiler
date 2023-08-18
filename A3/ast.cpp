#include "ast.hh"
#include "symtab.hh"
#include <iostream>
#include <cstdarg>
#include <stack>
// #include <fstream>
// #include <string>
// #include <vector>
using namespace std;

extern SymTab gst;
extern std::map<string, abstract_astnode *> ast;
extern std::map<std::string, vector<string>> asm_code;
extern int label_count;
extern string currFunc;
extern std::stack<std::string> registers;
extern std::stack<std::string> live_registers;

/*

void empty_astnode::print(int blank) {
				cout << "empty_astnode" << endl;
}


void seq_astnode::print(int blank) {
				cout << "seq_astnode" << endl;
}


void assignS_astnode::print(int blank) {
				cout << "assignS_astnode" << endl;
}

void return_astnode::print(int blank) {
				cout << "return_astnode" << endl;
}

void if_astnode::print(int blank) {
				cout << "if_astnode" << endl;
}

void while_astnode::print(int blank) {
				cout << "while_astnode" << endl;
}

void for_astnode::print(int blank) {
				cout << "for_astnode" << endl;
}

void proccall_astnode::print(int blank) {
				cout << "proccall_astnode" << endl;
}



void op_binary_astnode::print(int blank) {
				cout << "op_binary_astnode" << endl;
}

void arrayref_astnode::print(int blank) {
				cout << "arrayref_astnode" << endl;
}

void arrow_astnode::print(int blank) {
				cout << "arrow_astnode" << endl;
}

void member_astnode::print(int blank) {
				cout << "member_astnode" << endl;
}


void assignE_astnode::print(int blank) {
				cout << "assignE_astnode" << endl;
}

void funcall_astnode::print(int blank) {
				cout << "funcall_astnode" << endl;
}


void intconst_astnode::print(int blank) {
				cout << "intconst_astnode" << endl;
}

void op_unary_astnode::print(int blank) {
				cout << "op_unary_astnode" << endl;
}

void stringconst_astnode::print(int blank) {
				cout << "stringconst_astnode" << endl;
}

void identifier_astnode::print(int blank) {
				cout << "identifier_astnode" << endl;
}
*/

empty_astnode::empty_astnode()
{
	this->astnode_type = typeExp::Empty_astnode;
};
return_astnode::return_astnode(exp_astnode *e)
{
	this->astnode_type = typeExp::Return_astnode;
	this->exp = e;
}

if_astnode::if_astnode(exp_astnode *c, statement_astnode *t, statement_astnode *f)
{
	this->astnode_type = typeExp::If_astnode;
	this->condition = c;
	this->then = t;
	this->elsee = f;
}

while_astnode::while_astnode(exp_astnode *c, statement_astnode *b)
{
	this->astnode_type = typeExp::While_astnode;
	this->condition = c;
	this->body = b;
}

for_astnode::for_astnode(exp_astnode *i, exp_astnode *c, exp_astnode *u, statement_astnode *b)
{
	this->astnode_type = typeExp::For_astnode;
	this->init = i;
	this->condition = c;
	this->update = u;
	this->body = b;
}

op_binary_astnode::op_binary_astnode(std::string o, exp_astnode *l, exp_astnode *r)
{
	this->astnode_type = typeExp::Op_binary_astnode;
	this->left = l;
	this->right = r;
	this->op = o;
}

op_unary_astnode::op_unary_astnode(std::string o, exp_astnode *e)
{
	this->astnode_type = typeExp::Op_unary_astnode;
	this->exp = e;
	this->op = o;
}

assignE_astnode::assignE_astnode(exp_astnode *l, exp_astnode *r)
{
	this->astnode_type = typeExp::AssignE_astnode;
	this->left = l;
	this->right = r;
}

stringconst_astnode::stringconst_astnode(std::string v)
{
	this->astnode_type = typeExp::Stringconst_astnode;
	this->value = v;
}

identifier_astnode::identifier_astnode(std::string n)
{
	this->astnode_type = typeExp::IDENTIFIER_astnode;
	this->name = n;
}

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

// void printf_astnode::print(int blank) {
//     cout << "printf_astnode" << endl;
// }

void printAst(const char *astname, const char *fmt...) // fmt is a format string that tells about the type of the arguments.
{
	typedef vector<exp_astnode *> *pv;
	va_list args;
	va_start(args, fmt);
	if ((astname != NULL) && (astname[0] != '\0'))
	{
		cout << "{ ";
		cout << "\"" << astname << "\""
			 << ": ";
	}
	cout << "{" << endl;
	while (*fmt != '\0')
	{
		if (*fmt == 'a')
		{
			char *field = va_arg(args, char *);
			abstract_astnode *a = va_arg(args, abstract_astnode *);
			cout << "\"" << field << "\": " << endl;

			a->print(0);
		}
		else if (*fmt == 's')
		{
			char *field = va_arg(args, char *);
			char *str = va_arg(args, char *);
			cout << "\"" << field << "\": ";

			cout << str << endl;
		}
		else if (*fmt == 'i')
		{
			char *field = va_arg(args, char *);
			int i = va_arg(args, int);
			cout << "\"" << field << "\": ";

			cout << i;
		}
		else if (*fmt == 'f')
		{
			char *field = va_arg(args, char *);
			double f = va_arg(args, double);
			cout << "\"" << field << "\": ";
			cout << f;
		}
		else if (*fmt == 'l')
		{
			char *field = va_arg(args, char *);
			pv f = va_arg(args, pv);
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

void empty_astnode::print(int blank)
{
	cout << "\"empty\"" << endl;
}

void seq_astnode::print(int blank)
{
	printAst("", "l", "seq", statements);
}

void assignS_astnode::print(int blank)
{
	// cout << "assignS_astnode" << endl;
	printAst("assignS", "a", "left", exp);
}

void return_astnode::print(int blank)
{
	// cout << "return_astnode" << endl;
	printAst("", "a", "return", exp);
}

void if_astnode::print(int blank)
{
	// cout << "if_astnode" << endl;
	printAst("if", "aaa", "cond", condition, "then", then, "else", elsee);
}

void while_astnode::print(int blank)
{
	// cout << "while_astnode" << endl;
	printAst("while", "aa", "cond", condition, "stmt", body);
}

void for_astnode::print(int blank)
{
	// cout << "for_astnode" << endl;
	printAst("for", "aaaa",
			 "init", init,
			 "guard", condition,
			 "step", update,
			 "body", body);
}

// proccall_astnode::proccall_astnode(std::string n, vector<exp_astnode *> a) {
//     this->astnode_type = typeExp::Proccall_astnode;
//     this->args = a;
// }
void proccall_astnode::print(int blank)
{
	// cout << "proccall_astnode" << endl;
	printAst("proccall", "al", "fname", name, "params", args);
}

void op_binary_astnode::print(int blank)
{
	// cout << "op_binary_astnode" << endl;
	string str = "\"" + op + "\"";
	char *str1 = const_cast<char *>(str.c_str());
	printAst("op_binary", "saa", "op", str1, "left", left, "right", right);
}
// Here change type based on values of ref_count and star_count
// Keep it Pointer, int, float depending on them.

void op_unary_astnode::print(int blank)
{
	// cout << "op_unary_astnode" << endl;
	string str = "\"" + op + "\"";
	char *str1 = const_cast<char *>(str.c_str());
	printAst("op_unary", "sa", "op", str1, "child", exp);
}

void assignE_astnode::print(int blank)
{
	// cout << "assignE_astnode" << endl;
	printAst("assignE", "aa", "left", left, "right", right);
}

void funcall_astnode::print(int blank)
{
	// cout << "funcall_astnode" << endl;
	printAst("funcall", "al", "fname", name, "params", args);
}

void printf_astnode::print(int blank)
{
	// cout << "printf_astnode" << endl;
	char *str1 = const_cast<char *>(format.c_str());
	printAst("printf", "sl", "string_term", str1, "params", args);
}

void intconst_astnode::print(int blank)
{
	// cout << "intconst_astnode" << endl;
	printAst("", "i", "intconst", value);
}

char *stringtoChar(std::string s)
{
	char *str = const_cast<char *>(s.c_str());
	return str;
}
void stringconst_astnode::print(int blank)
{
	// cout << "stringconst_astnode" << endl;
	printAst("", "s", "stringconst", stringtoChar(value));
}

void identifier_astnode::print(int blank)
{
	// cout << "identifier_astnode" << endl;
	string str = "\"" + name + "\"";
	char *str1 = const_cast<char *>(str.c_str());
	printAst("", "s", "identifier", str1);
}

void arrayref_astnode::print(int blank)
{
	// cout << "arrayref_astnode" << endl;
	printAst("arrayref", "aa", "array", array, "index", index);
}

void member_astnode::print(int blank)
{
	printAst("member", "aa", "struct", exp, "field", member);
}

void arrow_astnode::print(int blank)
{
	// cout << "Arrow_astnode" << endl;
	printAst("arrow", "aa", "pointer", exp, "field", member);
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

// Here asm code generation of each node is to be done

void empty_astnode::gen_code()
{
	cout << "#  empty_astnode" << endl;
}

void seq_astnode::gen_code()
{
	for (int i = 0; i < statements.size(); i++)
		statements[i]->gen_code();
}

void for_astnode::gen_code()
{
	// contains init, condition, update, body
	asm_code[currFunc].push_back("# Inside for loop ");
	init->gen_code();
	string label_head = ".LC" + to_string(label_count);
	label_count++;
	asm_code[currFunc].push_back(label_head + ":");
	condition->gen_code();
	string live_reg = live_registers.top();
	live_registers.pop();
	registers.push(live_reg);
	string exit_loop = ".LC" + to_string(label_count);
	label_count++;
	asm_code[currFunc].push_back("\tcmpl $1 , " + live_reg);
	asm_code[currFunc].push_back("\tjne " + exit_loop);
	body->gen_code();
	update->gen_code();
	asm_code[currFunc].push_back("\tjmp " + label_head);

	asm_code[currFunc].push_back(exit_loop + ":");
}


void funcall_astnode::gen_code()
{
	// contains name, args
	asm_code[currFunc].push_back("# Inside funcall_astnode");
	// space for return   value of struct 
	int return_size  = get_return_size(gst, name ->name);
	asm_code[currFunc].push_back("\tsubl $" +to_string(return_size) + ", %esp");
	for (int i=0 ;i<args.size(); i++)
	{
		args[i]->gen_code();
		string live_reg = live_registers.top();
		live_registers.pop();
		registers.push(live_reg);
		asm_code[currFunc].push_back("\tpushl " + live_reg);
	}
	// Push space for return value 
	asm_code[currFunc].push_back("\tpushl (%ebp)");
	asm_code[currFunc].push_back("\tcall " + name->name);
	asm_code[currFunc].push_back("\taddl $4 , %esp");
	// asm_code[currFunc].push_back("\taddl $" +return_size ", %esp");

	int size = get_size_param(gst, name->name);
	string new_reg = registers.top();
	registers.pop();
	asm_code[currFunc].push_back("\taddl $" + to_string(size) + ", %esp");
	asm_code[currFunc].push_back("\tpopl "  + new_reg); // This contains return value.  
	live_registers.push(new_reg);

}


void proccall_astnode::gen_code()
{
	asm_code[currFunc].push_back("# Inside proccall_astnode");
	if(gst.Entries[name->name].returntype != "void")
		asm_code[currFunc].push_back("\tsubl $4, %esp");
	for (int i=0 ;i<args.size(); i++)
	{
		args[i]->gen_code();
		string live_reg = live_registers.top();
		live_registers.pop();
		registers.push(live_reg);
		asm_code[currFunc].push_back("\tpushl " + live_reg);
	}
	// Push space for return value 
	asm_code[currFunc].push_back("\tpushl (%ebp)");
	asm_code[currFunc].push_back("\tcall " + name->name);
	if(gst.Entries[name->name].returntype != "void")
	asm_code[currFunc].push_back("\taddl $4 , %esp");
	int size = get_size_param(gst, name->name);
	asm_code[currFunc].push_back("\taddl $" + to_string(size) + ", %esp");
	if(gst.Entries[name->name].returntype != "void")
	{

		string new_reg = registers.top();
		registers.pop();
		asm_code[currFunc].push_back("\tpopl "  + new_reg); // This contains return value.  
		live_registers.push(new_reg);
	}
}

void arrayref_astnode::gen_code()
{	
	// contains array, index
	// need to do similar to *(array + index)
	index->gen_code();
	string live_reg = live_registers.top();

	asm_code[currFunc].push_back("\timull $4 , " + live_reg);
	// now live_reg contains index * 4
	// add this to offset of array 
	int offset = 0;
	if(array->astnode_type == typeExp::IDENTIFIER_astnode){
	offset = array->get_address();
	

	asm_code[currFunc].push_back("\taddl $" + to_string(offset) + " , " + live_reg);
	asm_code[currFunc].push_back("\taddl %ebp , " + live_reg);
	asm_code[currFunc].push_back("\tmovl (" + live_reg + ") , " + live_reg);
	} else{
		offset = array->get_address();
		array->gen_code();
		string live_reg2 = live_registers.top();
		live_registers.pop();
		asm_code[currFunc].pop_back();
		asm_code[currFunc].pop_back();
		asm_code[currFunc].pop_back();
	asm_code[currFunc].push_back("\timull $4 , " + live_reg2);
	asm_code[currFunc].push_back("\taddl $" + to_string(offset) + " , " + live_reg2);
	asm_code[currFunc].push_back("\taddl %ebp , " + live_reg2);

		asm_code[currFunc].push_back("\taddl " + live_reg2 + " , " + live_reg);
		asm_code[currFunc].push_back("\tmovl (" + live_reg + ") , " + live_reg);
		registers.push(live_reg2);

	}

	// add offset to live_reg 
	// now live_reg contains offset + index * 4
	// now add this to ebp



}

void arrow_astnode::gen_code()
{
	// contains exp, member is right.
	asm_code[currFunc].push_back("# Inside arrow_astnode");
	int offset = exp->get_address();
	string new_reg = registers.top();
	registers.pop();
	// string struct_type = get_struct_type(gst, currFunc, var_value);
	// struct_type = struct_type.substr(0, struct_type.size()-1);
	// int sub_offset = get_offset(gst, struct_type, member->name);
	// int  sub_offset  = exp->get_address();
	string struct_name = exp->get_name();
	int  sub_offset =   get_offset(gst, struct_name.substr(0, struct_name.size()-1), member->name);
	cout<<"# arrow offset " + member->name<<offset <<" sub_offset "<<sub_offset<<endl;
	asm_code[currFunc].push_back("\tmovl " + to_string(offset) + "(%ebp) , " + new_reg);
	live_registers.push(new_reg);
	asm_code[currFunc].push_back("\tmovl  "  +to_string(sub_offset)+  "(" + new_reg + ") , " + new_reg);
	

}


void op_unary_astnode::gen_code()
{
	// contains
	// op, exp
	asm_code[currFunc].push_back("#Inside OP_Unary " + op);
	
	if(op == "ADDR"){
		int offset = exp->get_address();

		string new_reg = registers.top();
		registers.pop();
		asm_code[currFunc].push_back("\tleal " + to_string(offset) + "(%ebp) , " + new_reg);	
		live_registers.push(new_reg);
		return;
	}
	if(op == "PTR*"){
		int offset = exp->get_address();
		string new_reg = registers.top();
		registers.pop();
		asm_code[currFunc].push_back("\tmovl " + to_string(offset) + "(%ebp) , " + new_reg);
		live_registers.push(new_reg);
		asm_code[currFunc].push_back("\tmovl (" + live_registers.top() + ") , " + live_registers.top());
		cout<<"#Exiting PTR*"<<endl;
		return;
	}
	
	exp->gen_code();
	if (op == "INC")
	{
		asm_code[currFunc].push_back("\taddl $1, " + live_registers.top());
		int offset = exp->get_address();

		asm_code[currFunc].push_back("\tmovl " + live_registers.top() + " , " + to_string(offset) + "(%ebp)");
		asm_code[currFunc].push_back("\tsubl $1, " + live_registers.top());
	}
	if (op == "UMINUS")
	{
		asm_code[currFunc].push_back("\tneg " + live_registers.top());
	}
	if (op == "NOT")
	{
		asm_code[currFunc].push_back("\tcmpl  $0 , " + live_registers.top());
		string currLabel1 = ".LC" + to_string(label_count);
		label_count++;
		asm_code[currFunc].push_back("\tjne " + currLabel1);
		string currLabel2 = ".LC" + to_string(label_count);
		label_count++;
		asm_code[currFunc].push_back("\tjmp " + currLabel2);
		string currLabel3 = ".LC" + to_string(label_count);
		label_count++;
		asm_code[currFunc].push_back(currLabel1 + ":\n\t movl $0, " + live_registers.top() + "\n\tjmp " + currLabel3);
		asm_code[currFunc].push_back(currLabel2 + ":\n\t movl $1, " + live_registers.top());
		asm_code[currFunc].push_back(currLabel3 + ":");
	}





}

// TODO
void member_astnode::gen_code()
{
	// contains exp, member 
	// member is right side one. 
	cout<<"# Inside  member astnode " + member->name<<endl;
	asm_code[currFunc].push_back("# Inside member_astnode");
	// string struct_type = get_struct_type(gst, currFunc, var_value);
	// int offset = get_offset(gst, currFunc, var_value);
		int offset = exp->get_address();
	string struct_var = exp->get_name();
	offset += get_offset(gst, struct_var, member->name);


	// cout<<"Offset : "<<offset<<" Sub_offset : "<<sub_offset<<endl;
	string new_reg = registers.top();
	registers.pop();
	asm_code[currFunc].push_back("\tmovl " + to_string(offset) + "(%ebp), " + new_reg);
	live_registers.push(new_reg);
}

void while_astnode::gen_code()
{
	// contains condition, body
	asm_code[currFunc].push_back("# Inside While_astnode");
	string label_head = ".LC" + to_string(label_count);
	label_count++;
	asm_code[currFunc].push_back(label_head + ":");
	condition->gen_code();
	string live_reg = live_registers.top();
	live_registers.pop();
	registers.push(live_reg);
	string exit_loop = ".LC" + to_string(label_count);
	label_count++;
	asm_code[currFunc].push_back("\tcmpl $1 , " + live_reg);
	asm_code[currFunc].push_back("\tjne " + exit_loop);
	body->gen_code();
	asm_code[currFunc].push_back("\tjmp " + label_head);

	asm_code[currFunc].push_back(exit_loop + ":");
}

void if_astnode::gen_code()
{
	// condition exp_astnode, statement then, else
	asm_code[currFunc].push_back("#Inisde If_astnode ");
	condition->gen_code();
	string live_reg = live_registers.top();
	live_registers.pop();
	registers.push(live_reg);
	asm_code[currFunc].push_back("\tcmpl $1 , " + live_reg);
	string currLabel1 = ".LC" + to_string(label_count);
	label_count++;
	asm_code[currFunc].push_back("\tjne " + currLabel1);
	string currLabel2 = ".LC" + to_string(label_count);
	label_count++;
	asm_code[currFunc].push_back("\tjmp " + currLabel2);
	string currLabel3 = ".LC" + to_string(label_count);
	label_count++;
	asm_code[currFunc].push_back(currLabel1 + ":");
	elsee->gen_code();
	asm_code[currFunc].push_back("\tjmp " + currLabel3);
	asm_code[currFunc].push_back(currLabel2 + ":");
	then->gen_code();
	asm_code[currFunc].push_back(currLabel3 + ":");

}

void op_binary_astnode::gen_code()
{
	// contains
	// string op;
	// exp_astnode* left;
	// exp_astnode* right;
	asm_code[currFunc].push_back("# Inside op_binary Astnode " + op);
	right->gen_code();
	string live_reg = live_registers.top();
	live_registers.pop();
	registers.push(live_reg);
	asm_code[currFunc].push_back("\tpushl "  + live_reg);
	left->gen_code();
	asm_code[currFunc].push_back("\tpopl "+ registers.top());
	live_reg = registers.top();

	if(left->astnode_type == typeExp::IDENTIFIER_astnode){
		string temp_type = left ->get_name();
		if(temp_type[temp_type.size() -1] =='*'){
			asm_code[currFunc].push_back("\timull $4 , " + live_reg);
		}
	}

	if (op == "/")
	{
		asm_code[currFunc].push_back("\tcltd\n\tidivl " + registers.top());

	}
	if (op == "+")
	{
		asm_code[currFunc].push_back("\taddl " + live_reg + ", " + live_registers.top());
	}
	if (op == "-")
	{
		asm_code[currFunc].push_back("\tsubl " + live_reg + ", " + live_registers.top());
	}
	if (op == "*")
	{
		asm_code[currFunc].push_back("\timull " + live_reg + ", " + live_registers.top());
	}
	if (op == "||")
	{
		asm_code[currFunc].push_back("\tcmpl $0, " + live_reg);
		string currLabel1 = ".LC" + to_string(label_count);
		label_count++;
		asm_code[currFunc].push_back("\tjne " + currLabel1);
		asm_code[currFunc].push_back("\tcmpl $0, " + live_registers.top());
		string currLabel2 = ".LC" + to_string(label_count);
		label_count++;
		asm_code[currFunc].push_back("\tjne " + currLabel2);

		asm_code[currFunc].push_back(currLabel1 + ":\n\tmovl $1, " + live_registers.top());
		string currLabel3 = ".LC" + to_string(label_count);
		label_count++;
		asm_code[currFunc].push_back("\tjmp " + currLabel3);
		asm_code[currFunc].push_back(currLabel2 + ":\n\tmovl $1, " + live_registers.top());
		asm_code[currFunc].push_back(currLabel3 + ":");
	}

	if (op == "&&")
	{
		asm_code[currFunc].push_back("\tcmpl $0, " + live_reg);
		string currLabel1 = ".LC" + to_string(label_count);
		label_count++;
		asm_code[currFunc].push_back("\tje " + currLabel1);
		asm_code[currFunc].push_back("\tcmpl $0, " + live_registers.top());

		asm_code[currFunc].push_back("\tje " + currLabel1);

		string currLabel2 = ".LC" + to_string(label_count);
		label_count++;
		asm_code[currFunc].push_back("\tmovl $1, " + live_registers.top());
		asm_code[currFunc].push_back("\tjmp " + currLabel2);

		asm_code[currFunc].push_back(currLabel1 + ":\n\tmovl $0, " + live_registers.top());

		asm_code[currFunc].push_back(currLabel2 + ":");
	}

	if (op == "==")
	{
		asm_code[currFunc].push_back("\tcmpl " + live_reg + ", " + live_registers.top());
		string currLabel1 = ".LC" + to_string(label_count);
		label_count++;
		asm_code[currFunc].push_back("\tjne " + currLabel1);
		string currLabel2 = ".LC" + to_string(label_count);
		label_count++;
		asm_code[currFunc].push_back("\tjmp " + currLabel2);
		string currLabel3 = ".LC" + to_string(label_count);
		label_count++;
		asm_code[currFunc].push_back(currLabel1 + ":\n\t movl $0, " + live_registers.top() + "\n\tjmp " + currLabel3);
		asm_code[currFunc].push_back(currLabel2 + ":\n\t movl $1, " + live_registers.top());
		asm_code[currFunc].push_back(currLabel3 + ":");
	}
	if (op == "!=")
	{
		asm_code[currFunc].push_back("\tcmpl " + live_reg + ", " + live_registers.top());
		string currLabel1 = ".LC" + to_string(label_count);
		label_count++;
		asm_code[currFunc].push_back("\tje " + currLabel1);
		string currLabel2 = ".LC" + to_string(label_count);
		label_count++;
		asm_code[currFunc].push_back("\tjmp " + currLabel2);
		string currLabel3 = ".LC" + to_string(label_count);
		label_count++;
		asm_code[currFunc].push_back(currLabel1 + ":\n\t movl $0, " + live_registers.top() + "\n\tjmp " + currLabel3);
		asm_code[currFunc].push_back(currLabel2 + ":\n\t movl $1, " + live_registers.top());
		asm_code[currFunc].push_back(currLabel3 + ":");
	}

	if (op == ">=")
	{
		asm_code[currFunc].push_back("\tcmpl " + live_reg + ", " + live_registers.top());
		string currLabel1 = ".LC" + to_string(label_count);
		label_count++;
		asm_code[currFunc].push_back("\tjl " + currLabel1);
		string currLabel2 = ".LC" + to_string(label_count);
		label_count++;
		asm_code[currFunc].push_back("\tjmp " + currLabel2);
		string currLabel3 = ".LC" + to_string(label_count);
		label_count++;
		asm_code[currFunc].push_back(currLabel1 + ":\n\t movl $0, " + live_registers.top() + "\n\tjmp " + currLabel3);
		asm_code[currFunc].push_back(currLabel2 + ":\n\t movl $1, " + live_registers.top());
		asm_code[currFunc].push_back(currLabel3 + ":");
	}

	if (op == "<=")
	{
		asm_code[currFunc].push_back("\tcmpl " + live_reg + ", " + live_registers.top());
		string currLabel1 = ".LC" + to_string(label_count);
		label_count++;
		asm_code[currFunc].push_back("\tjg " + currLabel1);
		string currLabel2 = ".LC" + to_string(label_count);
		label_count++;
		asm_code[currFunc].push_back("\tjmp " + currLabel2);
		string currLabel3 = ".LC" + to_string(label_count);
		label_count++;
		asm_code[currFunc].push_back(currLabel1 + ":\n\t movl $0, " + live_registers.top() + "\n\tjmp " + currLabel3);
		asm_code[currFunc].push_back(currLabel2 + ":\n\t movl $1, " + live_registers.top());
		asm_code[currFunc].push_back(currLabel3 + ":");
	}

	if (op == ">")
	{
		asm_code[currFunc].push_back("\tcmpl " + live_reg + ", " + live_registers.top());
		string currLabel1 = ".LC" + to_string(label_count);
		label_count++;
		asm_code[currFunc].push_back("\tjle " + currLabel1);
		string currLabel2 = ".LC" + to_string(label_count);
		label_count++;
		asm_code[currFunc].push_back("\tjmp " + currLabel2);
		string currLabel3 = ".LC" + to_string(label_count);
		label_count++;
		asm_code[currFunc].push_back(currLabel1 + ":\n\t movl $0, " + live_registers.top() + "\n\tjmp " + currLabel3);
		asm_code[currFunc].push_back(currLabel2 + ":\n\t movl $1, " + live_registers.top());
		asm_code[currFunc].push_back(currLabel3 + ":");
	}

	if (op == "<")
	{
		asm_code[currFunc].push_back("\tcmpl " + live_reg + ", " + live_registers.top());
		string currLabel1 = ".LC" + to_string(label_count);
		label_count++;
		asm_code[currFunc].push_back("\tjge " + currLabel1);
		string currLabel2 = ".LC" + to_string(label_count);
		label_count++;
		asm_code[currFunc].push_back("\tjmp " + currLabel2);
		string currLabel3 = ".LC" + to_string(label_count);
		label_count++;
		asm_code[currFunc].push_back(currLabel1 + ":\n\t movl $0, " + live_registers.top() + "\n\tjmp " + currLabel3);
		asm_code[currFunc].push_back(currLabel2 + ":\n\t movl $1, " + live_registers.top());
		asm_code[currFunc].push_back(currLabel3 + ":");
	}

}


void assignE_astnode::gen_code()
{
	// contains
	// exp_astnode* left;
	// exp_astnode* right;
	asm_code[currFunc].push_back("# Inside assignE Astnode" + to_string(left->astnode_type));
	right->gen_code();

	if (left->astnode_type == typeExp::Arrayref_astnode)
	{
		string live_reg = live_registers.top();
		live_registers.pop();
		left->gen_code();
		string live_reg2 = live_registers.top();

		asm_code[currFunc].pop_back();
		asm_code[currFunc].push_back("\tmovl " + live_reg + ", (" + live_reg2 + ")");
		registers.push(live_reg);
		registers.push(live_reg2);
		live_registers.pop();

		return;
	}

	if(left->astnode_type == typeExp::Op_unary_astnode){
		string live_reg2 = registers.top();
		string live_reg = live_registers.top();
		live_registers.pop();
		int offset = left->get_address();
		asm_code[currFunc].push_back("\tmovl " + to_string(offset)+ "(%ebp), " + live_reg2);
		asm_code[currFunc].push_back("\tmovl " + live_reg + ",(" + live_reg2 + ")");
		registers.push(live_reg);
		return;
	}
	//  TODO
	if(left->astnode_type == typeExp::Arrow_astnode)
	{
		string live_reg =  live_registers.top();
		int offset = 0;
		int sub_offset = 0;
		live_registers.pop();
		left->gen_code();
		asm_code[currFunc].pop_back();
		// string var_name = left->get_address();
		// string var_name1 = var_name.substr(0, var_name.find(" "));
		// string var_name2 = var_name.substr(var_name.find(" ") + 1);
		// offset = stoi(var_name1);
		// sub_offset = stoi(var_name2);
		// offset = left->get_address();

		sub_offset = left->get_address();
		cout<<"#Offset , suboffset  "<<offset<<sub_offset<<endl;
		asm_code[currFunc].push_back("\tmovl " + live_reg + ", " + to_string(sub_offset)+"(" + live_registers.top() + ")");
		registers.push(live_reg);
		return;
	}


	string live_reg = live_registers.top();
	live_registers.pop();
	registers.push(live_reg);
	int offset;
	if (left->astnode_type == typeExp::IDENTIFIER_astnode){

	offset = left->get_address();
	}
	if (left->astnode_type == typeExp::Member_astnode){ 
		offset = left->get_address();
	}


	asm_code[currFunc].push_back("\tmovl " + live_reg + ", " + to_string(offset) + "(%ebp)");

	// left->gen_code();
}

void identifier_astnode::gen_code()
{
	asm_code[currFunc].push_back("# Inside identifier Astnode " + name);
	// Contains
	// string name;

	int offset = get_offset(gst, currFunc, name);
	string curr_reg = registers.top();
	registers.pop();
	live_registers.push(curr_reg);
	asm_code[currFunc].push_back("\tmovl " + to_string(offset) + "(%ebp), " + curr_reg);
}

void assignS_astnode::gen_code()
{
	// Contains
	// identifier_astnode* exp;
	asm_code[currFunc].push_back("# Inside assignS Astnode");
	exp->gen_code();
}

void intconst_astnode::gen_code()
{
	// contains
	// int value;
	asm_code[currFunc].push_back("# Inside intconst Astnode");

	// Remove top element of registers to curr_reg
	string curr_reg = registers.top();
	registers.pop();
	// Push curr_reg to live_registers
	live_registers.push(curr_reg);
	asm_code[currFunc].push_back("\tmovl $" + to_string(value) + "," + curr_reg);
}

void printf_astnode::gen_code()
{
	// prinf_astnode contains
	// 1. format string
	// 2. vector of expressions
	asm_code[currFunc].push_back("# Inside printf Astnode");
	string currLabel = ".LC" + to_string(label_count);
	// asm_code [currLabel] = vector<string>();
	// asm_code[currLabel].push_back(currLabel+ ":");
	label_count++;
	// asm_code[currLabel].push_back( );
	// Insert the label in front of currFunc vector
	asm_code[currFunc].insert(asm_code[currFunc].begin(), "\t.string " + format);
	asm_code[currFunc].insert(asm_code[currFunc].begin(), ".LC" + to_string(label_count - 1) + ":");

	for (int i = args.size() - 1; i >= 0; i--)
	{
		args[i]->gen_code();
		string live_reg = live_registers.top();
		live_registers.pop();
		registers.push(live_reg);
		asm_code[currFunc].push_back("\t pushl " + live_reg);
	}
	asm_code[currFunc].push_back("\t pushl $" + currLabel);
	asm_code[currFunc].push_back("\t call printf");
	asm_code[currFunc].push_back("\t addl $" + to_string(4 * (args.size() + 1)) + ", %esp");
}

void return_astnode::gen_code()
{
	// return_astnode contains
	// exp_astnode* exp;
	asm_code[currFunc].push_back("# Inside Return Astnode");
	exp->gen_code();
	if (currFunc == "main"){

		asm_code[currFunc].push_back("\tleave");
		if(!live_registers.empty()){
			asm_code[currFunc].push_back("# main function has live registers " + live_registers.top());
			registers.push(live_registers.top());
			live_registers.pop();
			}
	}
	else 
	{
		int size = get_size_local(gst, currFunc);
		asm_code[currFunc].push_back("\taddl $" + to_string(size) + ", %esp");
		
		if(!live_registers.empty()){
			cout<< "# currfunction has live registers " + live_registers.top()<<endl;
			asm_code[currFunc].push_back("# currfunction has live registers " + live_registers.top());
			int offset = get_offset_func(gst, currFunc) +4 ;
			asm_code[currFunc].push_back("\tmovl " + live_registers.top() + ", " + to_string(offset) + "(%ebp)");
			registers.push(live_registers.top());
			live_registers.pop();
		}
		asm_code[currFunc].push_back("\tpopl %ebp");
	}
	asm_code[currFunc].push_back("\tret");
	// cout << "# return_astnode" << endl;
}

void stringconst_astnode::gen_code()
{
	cout << "stringconst_astnode" << endl;
}

int identifier_astnode::get_address()
{
	int  offset = get_offset(gst, currFunc, name);
	// cout << "get_address" << endl;
	return offset;
}

// TODO
int member_astnode::get_address(){
	int offset = 0;
	// cout<<"# get address of member " + member->name <<" "<<struct_var<<endl;
	offset += exp->get_address();
	string struct_var = exp->get_name();
	offset += get_offset(gst, struct_var, member->name);
	cout<<"# address = " + member->name <<offset<<endl;
	// string struct_type = "";
	// if (exp->astnode_type == typeExp::IDENTIFIER_astnode)
	// 	string var_value = exp->get_address();
	// 	offset = get_offset(gst, currFunc, var_value);
	// 	struct_type = get_struct_type(gst, currFunc, var_value);
	// else if (exp->astnode_type == typeExp::Member_astnode){
	// 	string var_value = exp->get_address();
	// 	offset = stoi(var_name.substr(0, var_value.find(" ")));
	// 	var_value = var_name.substr(var_value.find(" ") + 1);
	// }
	// cout<<"# get address of  member_astnode " + var_value<<endl;

	// int sub_offset = get_offset(gst, struct_type, member->name);
	// cout<<"# exit from  member_astnode "<<offset+sub_offset<<endl;
	// return to_string(offset + sub_offset) + " " member->name;
	return offset;
}

int op_unary_astnode::get_address()
{
	// cout << "op_unary_astnode" << endl;
	int offset = exp->get_address();
	return  offset;
}

// TODO
int arrow_astnode::get_address()
{
	cout<<"# inside arrow  get_address"<<endl;
	int offset = exp->get_address();
	string  struct_var = exp->get_name();
	offset = get_offset(gst, struct_var.substr(0, struct_var.size()-1), member->name);

	// string struct_type = get_struct_type(gst, currFunc, var_value);
	// struct_type = struct_type.substr(0, struct_type.size()-1);
	// int sub_offset = get_offset(gst, struct_type, member->name);
	// int struct_size = get_struct_size(gst, struct_type);
	// // send offset and sub_offset sepearted by space 
	// return to_string(offset) + " " + to_string(sub_offset);
	return  offset;
}

int arrayref_astnode::get_address(){
	// contains 
	cout<<"#   inside  arrayref_address ";
	int var_value = array->get_address();
	return var_value;
	//  offset = get_offset(gst, currFunc, var_value);
	// } 
	// else{
	// 	cout<<" # arrayref_astnode : array is not identifier"<<endl;
	// }
}


std::string arrayref_astnode::get_name(){
	string var_value = array->get_name();
	return var_value;
}

std::string  member_astnode::get_name(){
	// contains exp  and member if exp is identi use currFunc
	string struct_name = get_struct_type(gst, exp->get_name(), member->name);
	// string  var_value =  exp->get_name();
	// return var_value + " " + member->name;
	return  struct_name;
}

std::string  arrow_astnode::get_name(){
	cout<<"# inside arrow  get_name "<<endl;
	string  temp = exp->get_name();
	string struct_name = get_struct_type(gst, temp.substr(0, temp.size()-1), member->name);
	// string  var_value =  exp->get_name();
	// return var_value + " " + member->name;
	return  struct_name;
}

std::string identifier_astnode::get_name(){
	// return name;
	return get_struct_type(gst, currFunc, name);
	// return currFunc;
}


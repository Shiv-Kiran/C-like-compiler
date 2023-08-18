
#include "scanner.hh"
#include "parser.tab.hh"
#include <fstream>
#include <stack>
using namespace std;

SymTab gst, gstfun, gststruct; 

stack <std::string> registers ;
stack <std::string> live_registers;
// = {"%eax", "%ebx", "%ecx", "%edx", "%esi", "%edi"};




// Map to store output code for each function
std::map<std::string, vector<string>> asm_code;


int label_count = 0;
string currFunc = "";

string tab = "    ";
string filename;
extern std::map<string,abstract_astnode*> ast;
std::map<std::string, datatype> predefined {
            {"printf", createtype(VOID_TYPE)},
            {"scanf", createtype(VOID_TYPE)},
            {"mod", createtype(INT_TYPE)}
	};

void print_ast(bool include_ast){
	cout << "{\"globalST\": " << endl;
	gst.printgst();
	cout << "," << endl;

	cout << "  \"structs\": [" << endl;
	for (auto it = gststruct.Entries.begin(); it != gststruct.Entries.end(); ++it)

	{   cout << "{" << endl;
		cout << "\"name\": " << "\"" << it->first << "\"," << endl;
		cout << "\"localST\": " << endl;
		it->second.symbtab->print(); // local localST
		cout << "}" << endl;
		if (next(it,1) != gststruct.Entries.end()) 
		cout << "," << endl;
	}
	cout << "]," << endl;
	cout << "  \"functions\": [" << endl;

	for (auto it = gstfun.Entries.begin(); it != gstfun.Entries.end(); ++it)

	{
		cout << "{" << endl;
		cout << "\"name\": " << "\"" << it->first << "\"," << endl;
		cout << "\"localST\": " << endl;
		it->second.symbtab->print();
		cout << "," << endl;
		cout << "\"ast\": " << endl;
		if (include_ast)
			ast[it->first]->print(0); // printing abstract syntax tree using the ast map string. 
		cout << "}" << endl;
		if (next(it,1) != gstfun.Entries.end()) cout << "," << endl;
		
	}
		cout << "]" << endl;
		cout << "}" << endl;
};

void generate_code(){

	for (auto it = ast.begin(); it != ast.end(); ++it)
	{
		currFunc = it->first;
		asm_code[it->first] = vector<string> {it->first + ":"};
		asm_code[it->first].push_back("\tpushl %ebp");
		asm_code[it->first].push_back("\tmovl %esp, %ebp");
		
		// get the last entry in it->second.symtab 



		if (gst.Entries[it->first].symbtab->Entries.size() > 0){

		int size = get_size_local(gst, it->first);
		// cout << "offset: " << offset << endl;
		asm_code[it->first].push_back("\tsubl $" + to_string(size) + ", %esp");
		}
		it->second->gen_code();

		asm_code[currFunc].insert(asm_code[currFunc].begin(), "\t.text\n\t.globl\t" + currFunc + "\n\t.type\t" + currFunc + ", @function");
		// check if return type of currFunc is void
		if(gst.Entries[currFunc].returntype == "void"){
		int size = get_size_local(gst, it->first);

			asm_code[it->first].push_back("\taddl $" + to_string(size) + ", %esp");
		
			asm_code[currFunc].push_back("\tnop");
			//asm_code[currFunc].push_back("\tleave");
			asm_code[currFunc].push_back("\tpopl %ebp");
			
			asm_code[currFunc].push_back("\tret");
		}
		// Empty live_registers into 

	}



	// Push front "\t.text\n\t.globl\tmain\n\t.type\tmain, @function" to front of vector with labe "main"


	// Print out all the entries in the code map
	for (auto it = asm_code.begin(); it != asm_code.end(); ++it)
	{
		for (auto it2 = it->second.begin(); it2 != it->second.end(); ++it2)
		{
			cout << *it2 << endl;
		}
	}
}


int main(int argc, char **argv)
{
	using namespace std;
	fstream in_file, out_file;
	

	in_file.open(argv[1], ios::in);

	IPL::Scanner scanner(in_file);

	IPL::Parser parser(scanner);

	#ifdef YYDEBUG
		parser.set_debug_level(1);
	#endif
	parser.parse();
	// create gstfun with function entries only

	for (const auto &entry : gst.Entries)
	{
		if (entry.second.varfun == "func")
		gstfun.Entries.insert({entry.first, entry.second});
	}
	// create gststruct with struct entries only

	for (const auto &entry : gst.Entries)
	{
		if (entry.second.varfun == "struct")
		gststruct.Entries.insert({entry.first, entry.second});
	}
	registers.push("%edi");
	registers.push("%esi");
	registers.push("%edx");
	registers.push("%ecx");
	registers.push("%ebx");
	registers.push("%eax");

	generate_code();



	// print_ast(true);

	fclose(stdout);
}


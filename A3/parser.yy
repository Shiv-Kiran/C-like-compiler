%skeleton "lalr1.cc"
%require  "3.5"

%defines 
%define api.namespace {IPL}
%define api.parser.class {Parser}
%define parse.trace

%code requires{
  #include "ast.hh"
  #include "symtab.hh"
  #include "type.hh"
   namespace IPL {
      class Scanner;
   }

}
/* %printer {std::cerr<<$$;} IDENTIFIER
%printer {std::cerr<<$$;} INT_CONST
%printer {std::cerr<<$$;} FLOAT_CONST
%printer {std::cerr<<$$;} STRING_LITERAL */

%parse-param {Scanner &scanner}
%locations
%code{
   #include<iostream>
   #include<cstdlib>
   #include<fstream>
   #include<string>


   #include"scanner.hh"
  //  int nodeCount = 0;

#undef yylex
#define yylex IPL::Parser::scanner.yylex

// string filename;
extern SymTab gst;
std::map<string,abstract_astnode*> ast;
bool funcdef = false, structdef = false;
SymTab* st;
SymTabEntry *ste;
std::string currStruct, currfun, current;
int offset = 0;
int currSize =0;
extern datatype createtype(basic_type t);
// std::string 
extern std::map<string,datatype> predefined;

}

%define api.value.type variant
%define parse.assert


%token <std::string> MAIN PRINTF IDENTIFIER CONSTANT_INT CONSTANT_STR INT VOID STRUCT IF ELSE WHILE FOR RETURN LRB RRB EOS LSB RSB LCB RCB COMMA OP_MUL OP_ADD OP_SUB OP_DIV OP_LT OP_GT OP_LTE OP_GTE OP_EQ OP_NEQ OP_AND OP_NOT OP_ASSIGN OP_INC OP_MEM OP_PTR OP_OR OP_ADDR OTHERS

/* %nterm  translation_unit struct_specifier function_definition type_specifier fun_declarator parameter_list parameter_declaration declarator_arr declarator compound_statement statement_list statement assignment_expression assignment_statement procedure_call expression logical_and_expression equality_expression relational_expression additive_expression unary_expression multiplicative_expression postfix_expression primary_expression expression_list unary_operator selection_statement iteration_statement declaration_list declarator_list */
/* %type <int> translation_unit struct_specifier function_definition type_specifier fun_declarator parameter_list parameter_declaration declarator_arr declarator compound_statement statement_list statement assignment_expression assignment_statement procedure_call expression logical_and_expression equality_expression relational_expression additive_expression unary_expression multiplicative_expression postfix_expression primary_expression expression_list unary_operator selection_statement iteration_statement declaration_list declarator_list declaration */
%start program
/* add left associativity rules for - and + and also precedence rules */
%nonassoc '='
%left OP_OR OP_AND OP_EQ OP_NEQ OP_LT OP_GT OP_LTE OP_GTE OP_ADD OP_SUB OP_MUL OP_DIV OP_PTR OP_INC OP_MEM
%right OP_ADDR 


%nterm <abstract_astnode*> translation_unit struct_specifier function_definition;
%nterm <type_specifier_class*> type_specifier;
%nterm <declarator_class*>declarator_arr declarator;
%nterm <declaration_class*> declaration;
%nterm <declaration_list_class*> declaration_list;
%nterm <declarator_list_class*> declarator_list;

/* %nterm <exp_astnode *> expression; */
%nterm <fun_declarator_class*> fun_declarator;
%nterm <parameter_list_class*> parameter_list;
%nterm <parameter_declaration_class*> parameter_declaration;
/* %nterm <statement_astnode *> statement; */
%nterm <assignS_astnode*> assignment_statement;

%nterm <abstract_astnode*> compound_statement 
%nterm <seq_astnode*> statement_list;
%nterm <assignE_astnode*> assignment_expression;
%nterm <exp_astnode*> logical_and_expression equality_expression relational_expression additive_expression multiplicative_expression unary_expression postfix_expression primary_expression expression;
/* %nterm <op_unary_astnode*> unary_operator; */
%nterm <std::string> unary_operator;
// is expression_list funcall_astnode
%nterm <proccall_astnode*> expression_list ;
%nterm <proccall_astnode*> procedure_call;
%nterm <statement_astnode*> selection_statement iteration_statement statement;
%nterm <printf_astnode*> printf_call;


%%
/* Need to complete translation unit  */
program
  : main_definition // P1 
  {
    
  }
  | translation_unit main_definition // P3

translation_unit: 
    struct_specifier // P4
  |  function_definition // P3
  | translation_unit struct_specifier // P4
  | translation_unit function_definition // P3

struct_specifier:
    STRUCT IDENTIFIER
    {
      st = new SymTab();
      currStruct = "struct " + $2;
      currSize =0;
      offset = 0;
      structdef = true;
    }
     LCB declaration_list RCB EOS // P4  
    {
      structdef = false;
      offset = 0;
      gst.push(currStruct, new SymTabEntry("struct", "global",currSize, 0, "-", st), 0);
      currStruct = "";
      currSize = 0;
    }
   

function_definition:  
    type_specifier IDENTIFIER LRB RRB  // P3
    {
      offset = 0;
      funcdef = true;
      st = new SymTab();
    }
    compound_statement
    {
      currfun = $2;
      offset = 0;
      funcdef = false;
      gst.push(currfun, new SymTabEntry("func", "global", $1->type.type,st), 0);
      st->push("", new SymTabEntry("","",0,0,"",NULL),2);
      ast.insert({$2,$6});
    }
    | type_specifier IDENTIFIER LRB {
      offset = 0;
      funcdef = true;
      st = new SymTab();
    }parameter_list RRB  // P3
    {
      funcdef = true;
      st = new SymTab();
      vector<parameter_declaration_class*> param_list = $5->parameter_list;
      for(int i=param_list.size()-1; i>=0 ;i--)
      {
        declarator_class* decl = param_list[i]->declarator;
        type_specifier_class* type_spec = param_list[i]->type_specifier;
        string type_name = "";
        int size = getSize(type_spec, decl);
        if(decl->type != NULL)
          type_name = decl->type->type;
        st->push(decl->name, new SymTabEntry("var", "param", size, offset,type_spec->type.type + type_name, NULL), 0);
        offset += size;
      }
      offset = 0;
    }
    compound_statement
    {
      currfun = $2;
      offset = 0;
      funcdef = false;
      gst.push(currfun, new SymTabEntry("func", "global", $1->type.type,st), 0);
      st->push("", new SymTabEntry("","",0,0,"",NULL),2);
      ast.insert({$2,$8});
    }

/*  Main Function Need to push into Symbol Table */
main_definition:
    INT MAIN LRB RRB
    {
      offset = 0;
      funcdef = true;
      st = new SymTab();
    }
     compound_statement // P1
    {
      currfun = $2;
      offset = 0;
      funcdef = false;
      gst.push(currfun, new SymTabEntry("func", "global", $1,st), 0);
      ast.insert({"main",$6});
    }

/* Type Specifier Need to change for struct  */

type_specifier:
    VOID // P3
    {
      $$ = new type_specifier_class();
      $$->type.type = "void";
      $$->type.size = 0;
    }
  | INT // P1
  {
    $$ = new type_specifier_class();
    $$->type.type = "int";
    $$->type.size = 4;
  }
  | STRUCT IDENTIFIER // P4
  {
    $$ = new type_specifier_class();
    $$->type.type = "struct " + $2;
    $$->type.size = get_struct_size(gst, $$->type.type);
  }

/* Declartion List */

declaration_list:
    declaration // P1
    {
      $$ = new declaration_list_class();
      $$->declaration_list.push_back($1);
    }
  | declaration_list declaration // P1
  {
    $$ = $1;
    $$->declaration_list.push_back($2);
  }

declaration:
    type_specifier declarator_list EOS // P1
    {
      $$ = new declaration_class($1,$2);
      if(structdef){
        vector<declarator_class*> declist = $2->declarator_list;
        for(auto decl: declist){
          int size = getSize($1, decl);
          string type_name = "";
          if(decl->type != NULL)
            type_name = decl->type->type;
          st->push(decl->name, new SymTabEntry("var", "local", size, offset, $1->type.type+type_name, NULL), 0);
          offset += size;
          currSize += size;
        }
      }
      if(funcdef){
        vector<declarator_class*> declist = $2->declarator_list;
        for(auto decl: declist){
          int size = getSize($1, decl);
          string type_name = "";
          if(decl->type != NULL)
            type_name = decl->type->type;
          st->push(decl->name, new SymTabEntry("var", "local", size, offset, $1->type.type+type_name, NULL), 1);
          offset += size;
        }
      }
    }
  
declarator_list:
    declarator // P1 
    {
      $$ = new declarator_list_class();
      $$->declarator_list.push_back($1);
    }
  | declarator_list COMMA declarator // P1
  {
    $$ = $1;
    $$->declarator_list.push_back($3);
  }

/* Even here need to change */
declarator:
    declarator_arr // P1
    {
      $$ = $1;
    }
    | OP_MUL declarator // P5 
    {
      $$ = $2;
      datatype* oldtype = $$->type;
      string prevtype = "";
      if(oldtype != NULL)
        prevtype = oldtype->type;
      $$->type = new datatype();
      $$->type->type = "*" + prevtype;
      int prevSize = 1;
      if(oldtype != NULL)
        prevSize = oldtype->size;
      $$->type->size = prevSize;
    }

/* Need to Change 2nd rule for symbol table */
declarator_arr:
    IDENTIFIER // P1
    {
      $$ = new declarator_class();
      $$->name = $1;
    }
  | declarator_arr LSB CONSTANT_INT RSB // P1
  {
    $$ = $1;
    datatype* oldtype = $$->type;
    string prevtype = "";
    $$->type = new datatype();
    if(oldtype != NULL)
      prevtype = oldtype->type;
    
    $$->type->type = prevtype + "[" + $3 +"]";
    int prevSize = 1;
    if(oldtype != NULL)
      prevSize = oldtype->size;
    $$->type->size = prevSize * stoi($3);

    // $$->arr = true;
    // $$->arr_size = $3;
  }


/* Parameter List */ 
parameter_list:
    parameter_declaration // P3
    {
      $$ = new parameter_list_class();
      $$->parameter_list.push_back($1);
    }
  | parameter_list COMMA parameter_declaration // P3
  {
    $$ = $1;
    $$->parameter_list.push_back($3);
  }

parameter_declaration:
    type_specifier declarator // P3
    {
      $$ = new parameter_declaration_class($1,$2);
    }

/* Statements */

compound_statement:
    LCB RCB // P1
    {
      $$ = new seq_astnode();
    }
  | LCB statement_list RCB // P1
  {
    $$ = $2;
  }
  | LCB declaration_list statement_list RCB // P1
  {
    $$ = $3;
  }

statement_list:
    statement // P1
    {
      $$ = new seq_astnode($1);
    }
  | statement_list statement // P1  
  {
    $$ = $1;
    $$->statements.push_back($2);
  }

statement:
    EOS // P1 
    {
      $$ = new empty_astnode();
    }
    | LCB statement_list RCB // P1
    {
      $$ = $2;
    }
    | assignment_expression EOS // P1
    {
      $$ = new assignS_astnode($1);
    }
    | selection_statement // P2
    {
      $$ = $1;
    }
    | iteration_statement // P2
    {
      $$ = $1;
    }
    | procedure_call // P3
    {
      $$ = $1;
    }
    | printf_call // P1 
    {
      $$ = $1;
    }
    | RETURN expression EOS // P1
    {
      $$ = new return_astnode($2);
    }

/* Expression */
assignment_expression:
  unary_expression OP_ASSIGN expression // P1
  {
    $$ = new assignE_astnode($1, $3);
  }

expression:
  logical_and_expression // P1
  {
    $$ = $1;
  }
  | expression OP_OR logical_and_expression // P1
  {
    $$ = new op_binary_astnode($2, $1, $3);
  }

logical_and_expression:
  equality_expression // P1
  {
    $$ = $1;
  }
  | logical_and_expression OP_AND equality_expression // P1
  {
    $$ = new op_binary_astnode($2, $1, $3);
  }

equality_expression:
  relational_expression // P1
  {
    $$ = $1;
  }
  | equality_expression OP_EQ relational_expression // P1
  {
    $$ = new op_binary_astnode($2, $1, $3);
  }
  | equality_expression OP_NEQ relational_expression // P1
  {
    $$ = new op_binary_astnode($2, $1, $3);
  }


relational_expression:
    additive_expression 
    {
      $$ = $1;
    }
  | relational_expression OP_LT additive_expression // P1
  {
    $$ = new op_binary_astnode($2, $1, $3);
  }
  | relational_expression OP_GT additive_expression // P1
  {
    $$ = new op_binary_astnode($2, $1, $3);
  }
  | relational_expression OP_LTE additive_expression // P1
  {
    $$ = new op_binary_astnode($2, $1, $3);
  }
  | relational_expression OP_GTE additive_expression // P1
  {
    $$ = new op_binary_astnode($2, $1, $3);
  }

additive_expression:
    multiplicative_expression // P1
    {
      $$ = $1;
    }
  | additive_expression OP_ADD multiplicative_expression
  {
    $$ = new op_binary_astnode($2, $1, $3);
  }
  | additive_expression OP_SUB multiplicative_expression 
  {
    $$ = new op_binary_astnode($2, $1, $3);
  } 

multiplicative_expression:
    unary_expression  // P1
    {
      $$ = $1;
    }
  | multiplicative_expression OP_MUL unary_expression 
  {
    $$ = new op_binary_astnode($2, $1, $3);
  }
  | multiplicative_expression OP_DIV unary_expression   
  {
    $$ = new op_binary_astnode($2, $1, $3);
  }
  
unary_expression:
    postfix_expression // P1
    {
      $$ = $1;
    }
  | unary_operator unary_expression // P1
  {
    $$ = new op_unary_astnode($1, $2);
  }


postfix_expression:
    primary_expression // P1
    {
      $$ = $1;
    }
  | postfix_expression OP_INC // P1
  {
    $$ = new op_unary_astnode("INC", $1);
  }
  | IDENTIFIER LRB RRB // P3
  {
    $$ = new funcall_astnode(new identifier_astnode($1), std::vector<exp_astnode*>());
  }
  | IDENTIFIER LRB expression_list RRB // P3
  {
    $$ = new funcall_astnode(new identifier_astnode($1), $3->args);
  }
  | postfix_expression OP_MEM IDENTIFIER // P4
  {
    $$ = new member_astnode($1, new identifier_astnode($3));
  }
  | postfix_expression OP_PTR IDENTIFIER // P5
  {
    $$ = new arrow_astnode($1, new identifier_astnode($3));
  }
  | postfix_expression LSB expression RSB // P6
  {
    $$ = new arrayref_astnode($1, $3);
  }

primary_expression:
    IDENTIFIER // P1
    {
      $$ = new identifier_astnode($1);
    }
  | CONSTANT_INT // P1
  {
    $$ = new intconst_astnode(std::stoi($1));
  }
  | LRB expression RRB // P1
  {
    $$ = $2;
  }

unary_operator:
  OP_SUB // P1 
  {
    $$ = "UMINUS";
  }
  | OP_NOT // P1
  {
    $$ = "NOT";
  }
  | OP_ADDR // P5
  {
    $$ = "ADDR";
  }
  | OP_MUL // P5
  {
    $$ = "PTR*";
  }
  ;
/* Selection Statement */ 
selection_statement:
 IF LRB expression RRB statement ELSE statement // P2
 {
    $$ = new if_astnode($3,$5,$7);
 }
  ;
iteration_statement: 
  WHILE LRB expression RRB statement // P2
  {
    $$ = new while_astnode($3,$5);
  }
  | FOR LRB assignment_expression EOS expression EOS assignment_expression RRB statement // P2
  {
    $$ = new for_astnode($3,$5,$7,$9);
  }

/* Expression List */
expression_list:
  expression // P1
  {
    $$ = new proccall_astnode(new identifier_astnode(""), std::vector<exp_astnode*>());
    $$->args.push_back($1);
  }
  | expression_list COMMA expression // P1
  {
    $$ = $1;
    $$->args.push_back($3);
  }

/* printf Call */
printf_call:
  PRINTF LRB CONSTANT_STR RRB EOS // P1
  {
    $$ = new printf_astnode($3, std::vector<exp_astnode*>());
  }
  | PRINTF LRB CONSTANT_STR COMMA expression_list RRB EOS // P1
  {
    $$ = new printf_astnode($3, $5->args);
  }

/* Procedure Call */
procedure_call:
  IDENTIFIER LRB RRB EOS // P3
  {
    $$ = new proccall_astnode(new identifier_astnode($1), std::vector<exp_astnode*>());
  }
  | IDENTIFIER LRB expression_list RRB EOS // P3
  {
    // Push into Symbol Table 
    $$ = new proccall_astnode(new identifier_astnode($1), $3->args);
  }



// Identify all the tokes i.e Capital words in the above and list them down 
// in the following section


%%
void IPL::Parser::error(const location_type&l,const std::string &err_message){
  std::cerr<<"Error: "<<err_message<<" at "<<l<<"\n";
}


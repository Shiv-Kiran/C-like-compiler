%skeleton "lalr1.cc"
%require  "3.5"

%defines 
%define api.namespace {IPL}
%define api.location.type {IPL::location}
%define api.parser.class {Parser}
%define parse.trace

%code requires{
  #include "ast.hh"
  #include "symbtab.hh"
  #include "location.hh"
  #include "type.hh"
   namespace IPL {
      class Scanner;
   }

  // # ifndef YY_NULLPTR
  // #  if defined __cplusplus && 201103L <= __cplusplus
  // #   define YY_NULLPTR nullptr
  // #  else
  // #   define YY_NULLPTR 0
  // #  endif
  // # endif

}


%printer { std::cerr << $$; } IDENTIFIER
%printer { std::cerr << $$; } STRING_LITERAL
%printer { std::cerr << $$; } INT_CONST
%printer { std::cerr << $$; } FLOAT_CONST




%parse-param { Scanner  &scanner  }
%locations
%code{
   #include <iostream>
   #include <cstdlib>
   #include <fstream>
   #include "scanner.hh"
   //int nodeCount = 0;

#undef yylex
#define yylex IPL::Parser::scanner.yylex

std::map<std::string, abstract_astnode*> ast;
bool funcdef = false, structdef = false; 
SymbTab* st;
SymbTabEntry* ste;
std::string currStruct, currfun, current;
int offset = 0; 
int currSize = 0; 
datatype currType;
extern SymbTab gst;
extern datatype createtype(basic_type t);
extern map<string, datatype> predefined;
}




%define api.value.type variant
%define parse.assert

//%start program


%token <std::string> INT
%token <std::string> VOID  
%token <std::string> FLOAT
%token <std::string> STRUCT
%token <std::string> INT_CONST
%token <std::string> FLOAT_CONST
%token <std::string> RETURN
%token <std::string> IF
%token <std::string> FOR
%token <std::string> WHILE
%token <std::string> AND_OP
%token <std::string> EQ_OP
%token <std::string> NE_OP
%token <std::string> LE_OP
%token <std::string> GE_OP
%token <std::string> INC_OP
%token <std::string> PTR_OP
%token <std::string> OR_OP
%token <std::string> IDENTIFIER
%token <std::string> STRING_LITERAL
%token <std::string> ELSE
%token <std::string> OTHERS
%token '[' ']' '{' '}' '+' ',' '(' ')'
%token '!' '-' '&' ';' '*' '/' '=' '>' '<'


%nterm <abstract_astnode*> translation_unit struct_specifier function_definition;
%nterm <type_specifier_class*> type_specifier;
%nterm <declarator_class*> declarator_arr declarator;
%nterm <declaration_class*> declaration;
%nterm <declaration_list_class*> declaration_list;
%nterm <declarator_list_class*> declarator_list;

%nterm <fun_declarator_class*> fun_declarator;
%nterm <parameter_list_class*> parameter_list;
%nterm <parameter_declaration_class*> parameter_declaration;
%nterm <assignS_astnode*> assignment_statement;
%nterm <abstract_astnode*> compound_statement;

%nterm <seq_astnode*> statement_list;
%nterm <assignE_astnode*> assignment_expression;
%nterm <exp_astnode*> logical_and_expression equality_expression relational_expression additive_expression multiplicative_expression unary_expression postfix_expression  primary_expression expression;
%nterm <std::string> unary_operator;
%nterm <proccall_astnode*> procedure_call expression_list;
%nterm <statement_astnode*> selection_statement iteration_statement statement;
/* %nterm <op_unary_astnode*> unary_operator; 
%nterm <funcall_astnode*> expression_list;

*/



%%

translation_unit:
    struct_specifier
    {

    }
    | function_definition
    {
       
    }
    | translation_unit struct_specifier 
    {

    }
    | translation_unit function_definition 
    {
      
    }
    ;
  
/* Just check the rules */
struct_specifier:
    STRUCT IDENTIFIER   
    {
      st = new SymbTab();
      currStruct = "struct " + $2;
      currSize =0;
      offset = 0;
      structdef = true;
    }
    '{' declaration_list '}' ';'
    {
      structdef = false;
      offset = 0;
      if(gst.isPresent(currStruct)){
        error(@$, "Same struct defined");
      }
      gst.push(currStruct, new SymbTabEntry("struct", "global", currSize, 0, "-", st),0,0);
      currStruct = "";
      currSize = 0;
    }
    ;

function_definition:  
    type_specifier{
      current = $1->type.type_name;
    } fun_declarator compound_statement 
    {
      currfun = $3->name;
      funcdef = false;
      offset = 0;
      if(gst.isPresent($3->name)){
        error(@$, "Same func defined");
      }
      gst.push($3->name, new SymbTabEntry("fun","global", $1->type.type_name, st),0,0);
      st->push("", new SymbTabEntry("","",0,0,"",NULL),2,0);
      ast.insert({$3->name, $4});
    }
    ;
    
type_specifier:
    VOID 
    {
       $$ = new type_specifier_class(VOID_TYPE);
       currType = $$->type;
    }
  | INT 
  {
       $$ = new type_specifier_class(INT_TYPE);
       currType = $$->type;
  }
  | FLOAT 
  {
       $$ = new type_specifier_class(FLOAT_TYPE);
       currType = $$->type;
    
  }
  | STRUCT IDENTIFIER 
  {
    $$ = new type_specifier_class();
    $$->type.type_name = "struct " + $2;
    $$->type.size = gst.getStructSize($$->type.type_name);
    currType = $$->type;
  }

/* Check symtab entry offset */
fun_declarator: 
    IDENTIFIER '(' parameter_list ')' 
    {
       $$ = new fun_declarator_class($1, $3);
       funcdef = true;
       st = new SymbTab();
       vector<parameter_declaration_class*> param_list = $3->parameter_list;
      int n = param_list.size();
      for(int i= n-1; i>=0; i--){
        string type_name = "";
        if(param_list[i]->declarator->type != NULL){
          type_name = param_list[i]->declarator->type->type_name;
        }
        int size = calculateSize(param_list[i]->type, param_list[i]->declarator);
        if(st->isPresent(param_list[i]->declarator->name)){
          error(@$, "same fun declared");
        }
        st->push(param_list[i]->declarator->name, new SymbTabEntry("var", "param", size, offset, param_list[i]
        ->type->type.type_name + type_name, NULL),0,i);
        offset += size;
      }
      offset = 0;
    } 
  | IDENTIFIER '(' ')' 
  {
    currfun = $1;
      $$ = new fun_declarator_class($1, nullptr);
      funcdef = true;
      st = new SymbTab();
  }
  ;


parameter_list:
    parameter_declaration 
    {
        $$ = new parameter_list_class();
        $$->parameter_list.push_back($1);
    }
  | parameter_list ',' parameter_declaration 
  {
    $$ = $1;
    $$->parameter_list.push_back($3);
  }        
  ;

parameter_declaration:
    type_specifier declarator  
    {
         // cout<<$2->name<<"type specifier"<<endl;

      if($2->type!=NULL){
        if($1->type.type_name + $2->type->type_name == "void"){
          error(@$, "cannot define void type");
        }
      }
      if($2->type == NULL)
         // cout<<$2->name<<endl;
        if($1->type.type_name == "void"){
         error(@$, "cannot define void type = NULL");
        }
        
      $$ = new parameter_declaration_class($1, $2);
    }
    ;
    
declarator_arr:
    IDENTIFIER 
    {
      $$ = new declarator_class();
      $$->name = $1;
    }
  | declarator_arr '[' INT_CONST ']' 
  {
    $$ = $1;
    datatype* oldtype = $$->type;
    $$->type = new datatype();
    string old_type;
    if(oldtype!=NULL) old_type = oldtype->type_name;
    else old_type = "";
    $$->type->next = oldtype;
    $$->type->arrsize = stoi($3);
    $$->type->isPointer = false;
    $$->type->type_name = old_type + "[" +$3 +"]";
    int tempsize = 1;
    if(oldtype != NULL) tempsize = oldtype->size;
    $$->type->size = tempsize*stoi($3);
  }
  ;

declarator: 
    declarator_arr {
      $$ = $1;
    }
    | '*' declarator {
    $$ = $2;
    datatype* temp_type = $$->type;
    int tempsize = 1;
    string old_type;
    $$->type = new datatype();
    if(temp_type!=NULL) old_type = temp_type->type_name;
    else old_type = "";
    $$->type->next = temp_type;
    $$->type->arrsize = 0;
    $$->type->isPointer = true;
    $$->type->type_name = "*" + old_type ;
    if(temp_type != NULL) tempsize = temp_type->size;
    $$->type->size = tempsize;
    }
    ;

declarator_list:
    declarator 
    {
      $$ = new declarator_list_class();
      $$->declarator_list.push_back($1);
    }
  | declarator_list ',' declarator 
    {
      $$ = $1;
      $$->declarator_list.push_back($3);
    }
    ;

compound_statement: 
    '{' '}' 
    {
    $$ = new seq_astnode();
    }
  | '{' statement_list '}' 
  {
    $$ = $2;
   
  }
  | '{' declaration_list statement_list '}' 
  {
       $$ = $3;
  }
  | '{' declaration_list '}' 
  {
       $$ = new seq_astnode();
  }
  ;

statement_list:
    statement   
    {
   $$ = new seq_astnode(vector<statement_astnode*>());
   $$->statements.push_back($1);

    }
  | statement_list statement  
  {
  $$ = $1;
  $$->statements.push_back($2);
  }
  ;

/* Need to check Return */
statement:
      ';'  
      {
      $$ = new empty_astnode();
      }
    |  '{' statement_list '}' 
    {
       $$ = $2;
    }
    | selection_statement   
    {
     $$ = $1;
    }
    | iteration_statement  
    {
     $$ = $1;
      
    }
    | assignment_statement  
    {
     $$ = $1;
    
    }
    | procedure_call     
    {
     $$ = $1;
     
    }   
    | RETURN expression ';'  
    {
      if(!isrelacompat($2->type, gst.getrettype(currfun))){
        error(@$, "error at return");
      }
      if(current != $2->type){

      if(current=="int"){
        $2 = new op_unary_astnode("TO_INT", $2);
      }
      if(current == "float") $2 = new op_unary_astnode("TO_FLOAT", $2);
      }
      $$ = new return_astnode($2);
    }
    ;

assignment_statement: 
    assignment_expression ';' 
    {
      $$ = new assignS_astnode($1->left, $1->right);
    }
    ;

/* ismod and type check */
procedure_call:
  IDENTIFIER '(' ')' ';'
  {
    if(!gst.isvalid($1,0)){
      error(@$, "not valid identif at procedure cll");
    }
    $$ = new proccall_astnode(new identifier_astnode($1));
    $$->name->type = "void";
   $$->name->ismod = false;
  }
  |  IDENTIFIER '(' expression_list ')' ';' 
    {
      if(!gst.isvalid($1,$3->args.size())){
      error(@$, "not valid identif at procedure_call exp");
    }
    if($1 != "printf" && $1 != "scanf" && $1 != "mod" ){

    for(int i=0;i < $3->args.size();i++){
      if(!gst.isvalidparam($1,$3->args[i]->type, i)){
        error(@$, "not valid param at procedure_call ");
      }
      if(gst.getrettype($1, i) != $3->args[i]->type){
        if($3->args[i]->type == "int") $3->args[i] = new op_unary_astnode("TO_FLOAT",$3->args[i]);
        if($3->args[i]->type == "float") $3->args[i] = new op_unary_astnode("TO_INT",$3->args[i]);

      }
    }
    }
    $$ = $3;
    $$->name = new identifier_astnode($1); 
    if($1 != "mod") $$->name->type = gst.getrettype($1);
    else $$->name->type = "int";
    $$->name->type = "void";
    $$->name->ismod = false;
    }
    ;


assignment_expression:
     unary_expression '=' expression 
     {
      //$1->print(0);
      // $3->print(0);
      if(!($1->ismod && (isrelacompat($1->type, $3->type) || $1->type == $3->type || (isPoin($1->type) && ($3->type == "void*" || ($3->type =="int" && $3->value1 == 0))) || (isPoin($3->type) && $1->type =="void*")))){
        error(@$, "error at assignment_expression");
      }
      if(totype($1->type, $3->type) == "float"){
        if($1->type == "int"){
          $1 = new op_unary_astnode("TO_FLOAT", $1);
          $1->type = "float";
        }
        if($3->type == "int"){
          $3 = new op_unary_astnode("TO_FLOAT", $3);
          $3->type = "float";
        }
      }
       $$ = new assignE_astnode($1, $3);
       $$->type = "int";
        $$->ismod = false; 
     }
     ;



expression:
    logical_and_expression 
    {
   $$ = $1;
    }
  | expression OR_OP logical_and_expression 
  {
    if(!isrelacompat($1->type, $3->type) && !(isPoin($1->type) &&isPoin($3->type))){
      error(@$, "error at expression");
    }
    $$ = new op_binary_astnode("OR_OP", $1, $3);
    $$->type = totype($1->type, $3->type);
   $$->ismod = false;
  }
  ;

logical_and_expression:
    equality_expression 
    {
   $$ = $1;
    }
  | logical_and_expression AND_OP equality_expression 
  {

   $$ = new op_binary_astnode("AND_OP", $1, $3);
    $$->type = totype($1->type, $3->type);
     $$->ismod = false;
  }
  ;

equality_expression :
    relational_expression 
    {
   $$ = $1;
    }
  | equality_expression EQ_OP relational_expression 
  {
    if(!(isrelacompat($1->type, $3->type) || $1->type =="void*" || $3->type =="void*" ||(isPoin($1->type) && $3->type =="int" && $3->value1 == 0) || (isPoin($3->type) && $1->type == "int" && $1->value1 == 0))){
      error(@$, "error");
    }
  if(totype($1->type, $3->type) =="float"){
    if($1->type == "int") {
      $1 = new op_unary_astnode("TO_FLOAT", $1);
    }
    if($3->type == "int") {
      $3 = new op_unary_astnode("TO_FLOAT", $3);
    }
   $$ = new op_binary_astnode("EQ_OP_FLOAT", $1, $3);

  }
  else{

    $$ = new op_binary_astnode("EQ_OP_INT", $1, $3);
  }

    $$->type = "int";
    $$->ismod = false;
  }
  | equality_expression NE_OP relational_expression 
  {
    if(!(isrelacompat($1->type, $3->type)  && !($1->type == "void*") && !($3->type == "void*"))){
      error(@$, "error");
    }
  if(totype($1->type, $3->type) =="float"){
    if($1->type == "int") {
      $1 = new op_unary_astnode("TO_FLOAT", $1);
    }
    if($3->type == "int") {
      $3 = new op_unary_astnode("TO_FLOAT", $3);
    }
   $$ = new op_binary_astnode("NE_OP_FLOAT", $1, $3);

  }
  else{

    $$ = new op_binary_astnode("NE_OP_INT", $1, $3);
    }

    $$->type = "int";
    $$->ismod = false;
  }
  ;

relational_expression:
    additive_expression 
    {
   $$ = $1;
    }
  | relational_expression '<' additive_expression 
  {

    if(!isrelacompat($1->type, $3->type) ){
      error(@$, "error in relational ");
    }
if(totype($1->type, $3->type) =="float"){
    if($1->type == "int") {
      $1 = new op_unary_astnode("TO_FLOAT", $1);
      $1->type = "float";
    }
    if($3->type == "int") {
      $3 = new op_unary_astnode("TO_FLOAT", $3);
      $1->type = "float";

    }
   $$ = new op_binary_astnode("LT_OP_FLOAT", $1, $3);
}
else{

   $$ = new op_binary_astnode("LT_OP_INT", $1, $3);
}
    $$->type = "int";
     $$->ismod = false;
  }
  | relational_expression '>' additive_expression  
  {
   if(!isrelacompat($1->type, $3->type) ){
      error(@$, "error in relational ");
    }
if(totype($1->type, $3->type) =="float"){
    if($1->type == "int") {
      $1 = new op_unary_astnode("TO_FLOAT", $1);
      $1->type = "float";
    }
    if($3->type == "int") {
      $3 = new op_unary_astnode("TO_FLOAT", $3);
      $1->type = "float";

    }
   $$ = new op_binary_astnode("GT_OP_FLOAT", $1, $3);
}
else{

   $$ = new op_binary_astnode("GT_OP_INT", $1, $3);
}
    $$->type = "int";
     $$->ismod = false;
  
  }
  | relational_expression LE_OP additive_expression  
  {
   if(!isrelacompat($1->type, $3->type) ){
      error(@$, "error in relational ");
    }
if(totype($1->type, $3->type) =="float"){
    if($1->type == "int") {
      $1 = new op_unary_astnode("TO_FLOAT", $1);
      $1->type = "float";
    }
    if($3->type == "int") {
      $3 = new op_unary_astnode("TO_FLOAT", $3);
      $1->type = "float";

    }
   $$ = new op_binary_astnode("LE_OP_FLOAT", $1, $3);
}
else{

   $$ = new op_binary_astnode("LE_OP-INT", $1, $3);
}
    $$->type = "int";
     $$->ismod = false;
  
  }
  | relational_expression GE_OP additive_expression 
    { 
if(!isrelacompat($1->type, $3->type) ){
      error(@$, "error in relational ");
    }
if(totype($1->type, $3->type) =="float"){
    if($1->type == "int") {
      $1 = new op_unary_astnode("TO_FLOAT", $1);
      $1->type = "float";
    }
    if($3->type == "int") {
      $3 = new op_unary_astnode("TO_FLOAT", $3);
      $1->type = "float";

    }
   $$ = new op_binary_astnode("GE_OP_FLOAT", $1, $3);
}
else{

   $$ = new op_binary_astnode("GE_OP-INT", $1, $3);
}
    $$->type = "int";
     $$->ismod = false;
  
  }
  ;

additive_expression:
    multiplicative_expression 
    {
   $$ = $1;
   
    }
  | additive_expression '+' multiplicative_expression
    {
      if(!(($1->type=="int" && ($3->type =="float" || $3->type =="int" || isPoin($3->type))) || ($1->type =="float" && ($3->type =="float" || $3->type =="int")) || (isPoin($1->type) && $3->type =="int"))){
      error(@$, "error additive");
      }
    
  if(totype($1->type, $3->type) =="float"){
    if($1->type == "int") {
      $1 = new op_unary_astnode("TO_FLOAT", $1);
      $1->type = "float";
    }
    if($3->type == "int") {
      $3 = new op_unary_astnode("TO_FLOAT", $3);
      $3->type = "float";

    }
   $$ = new op_binary_astnode("PLUS_FLOAT", $1, $3);

  }
  else{

    $$ = new op_binary_astnode("PLUS_INT", $1, $3);
  }

    $$->type = totype($1->type, $3->type);
   $$->ismod = false;
  }
  | additive_expression '-' multiplicative_expression  
    {
      if(!(($1->type=="int" && ($3->type =="float" || $3->type =="int")) || ($1->type =="float" && ($3->type =="float" || $3->type =="int")) || (isPoin($1->type) && $3->type =="int") || (isPoin($1->type) && isPoin($3->type) && iscompatible($1->type, $3->type)))){
      error(@$, "error additive");
      }
  if(totype($1->type, $3->type) =="float"){
    if($1->type == "int") {
      $1 = new op_unary_astnode("TO_FLOAT", $1);
      $1->type = "float";
    }
    if($3->type == "int") {
      $3 = new op_unary_astnode("TO_FLOAT", $3);
      $3->type = "float";

    }
   $$ = new op_binary_astnode("MINUS_FLOAT", $1, $3);

  }
  else{

    $$ = new op_binary_astnode("MINUS_INT", $1, $3);
  }

    $$->type = totype($1->type, $3->type);
   $$->ismod = false;
  }
  ;

/* Type Checks */

unary_expression:
    postfix_expression 
    {
      $$ = $1;
    }
  | unary_operator unary_expression 
    {
    $$ = new op_unary_astnode($1, $2);
    if($1 == "ADDRESS"){
      if(!$2->ismod){
        error(@$, "error at unary");
      }
      $$->type = $2->type + "*";
      $$->ismod = false;
    }
    else if ($1 ==  "DEREF"){
      if(!isPoin($2->type)){
                error(@$, "error at unary");
      }
      $$->type = gettype($2->type);
    }
    else if ($1 == "UMINUS"){
      if($2->type != "int" && $2->type != "float" ){
                error(@$, "error at unary");

      }
      $$->type = $2->type;
      $$->ismod = false;
    }
        else if ($1 == "NOT"){
      if($2->type != "float"&& $2->type != "int" && !isPoin($2->type)){
                error(@$, "error at unary not");

      }
      $$->type = "int";
      $$->ismod = false;
    }
    }
    ;

multiplicative_expression:
    multiplicative_expression '*' unary_expression 
    {
      if(($1->type != "int" && $1->type != "float") || ($3->type != "int" && $3->type != "float")){
      error(@$, "error mult");
      }
  if(totype($1->type, $3->type) =="float"){
    if($1->type == "int") {
      $1 = new op_unary_astnode("TO_FLOAT", $1);
      $1->type = "float";
    }
    if($3->type == "int") {
      $3 = new op_unary_astnode("TO_FLOAT", $3);
      $3->type = "float";

    }
   $$ = new op_binary_astnode("MULT_FLOAT", $1, $3);

  }
  else{

    $$ = new op_binary_astnode("MULT_INT", $1, $3);
  }

    $$->type = totype($1->type, $3->type);
   $$->ismod = false;


  }
  | multiplicative_expression '/' unary_expression  
    {
          if(($1->type != "int" && $1->type != "float") || ($3->type != "int" && $3->type != "float")){
      error(@$, "error mult");
      }
  if(totype($1->type, $3->type) =="float"){
    if($1->type == "int") {
      $1 = new op_unary_astnode("TO_FLOAT", $1);
      $1->type = "float";
    }
    if($3->type == "int") {
      $3 = new op_unary_astnode("TO_FLOAT", $3);
      $3->type = "float";

    }
   $$ = new op_binary_astnode("DIV_FLOAT", $1, $3);

  }
  else{

    $$ = new op_binary_astnode("DIV_INT", $1, $3);
  }

    $$->type = totype($1->type, $3->type);
   $$->ismod = false;


  } 
  |  unary_expression 
  {
   $$ = $1;
  } 
  ;


/* Type Checks */
postfix_expression: 
    primary_expression 
    { 
      $$ = $1;
    }
  | IDENTIFIER '(' ')' 
  {
    if(!gst.isvalid($1, 0)){
      error(@$, "error at postfix_expression");
    }
   $$ = new funcall_astnode(new identifier_astnode($1));
   $$->type = gst.getrettype($1);
   $$->ismod = false;
  }
  | IDENTIFIER '(' expression_list ')' 
  {
    if(!gst.isvalid($1, $3->args.size())){
      error(@$, "error at postfix_expression");
    }

 if($1 != "printf" && $1 != "scanf" && $1 != "mod" ){

    for(int i=0;i < $3->args.size();i++){
      if(!gst.isvalidparam($1,$3->args[i]->type, i)){
        error(@$, "error at postfix_expression");
      }
      if(gst.getrettype($1, i) != $3->args[i]->type){
        if($3->args[i]->type == "int") $3->args[i] = new op_unary_astnode("TO_FLOAT",$3->args[i]);
        if($3->args[i]->type == "float") $3->args[i] = new op_unary_astnode("TO_INT",$3->args[i]);

      }

    }}

   $$ = new funcall_astnode(new identifier_astnode($1), $3->args);
   if($1=="mod"){
     $$->type = "int";
     $$->ismod = false;
   }
   else {
     $$->type = gst.getrettype($1);
     $$->ismod = false;
   }


  }
  | postfix_expression INC_OP 
  {
    if(!$1->ismod){
      error(@$, "error at postfix_expression");
    }
    if(!isPoin($1->type) && $1->type != "int" &&  $1->type != "float"  ){
      error(@$, "error at postfix_expression inc");
    }
   $$ = new op_unary_astnode("PP", $1);
   $$->type = $1->type;
   $$->ismod = false;
  }
  | postfix_expression '[' expression ']' 
  {
    if(!isPoin($1->type) || $3->type != "int"){
      error(@$, "erro");
    }

     $$ = new arrayref_astnode($1, $3);
      $$->type = gettype($1->type);

  }
  | postfix_expression '.' IDENTIFIER 
  {
    if(!gst.isvalStruct($1->type, $3)){
      error(@$, "error at postfix_expression at mem");

    }
   $$ = new member_astnode($1, new identifier_astnode($3));
   $$->type = gst.gettype($1 ->type, $3);
  }
  | postfix_expression PTR_OP IDENTIFIER
  {
    if(!gst.isvalStruct(gettype($1->type), $3)){
      error(@$, "error at postfix_expression at ptr_op");

    }
   $$ = new arrow_astnode($1, new identifier_astnode($3));
   $$->type = gst.gettype(gettype($1->type), $3);
  }
    ;

primary_expression: IDENTIFIER
  {
   $$ = new identifier_astnode($1);
   $$->type = st->getrettype($1);
  } 
  | INT_CONST
  {
   $$ = new intconst_astnode(std::stoi($1));
    $$->type = "int";
    $$->value1 = stoi($1);
    $$->ismod = false;
  } 
  | FLOAT_CONST 
  {
   $$ = new floatconst_astnode(std::stof($1));
     $$->type = "float";
    $$->ismod = false;

  } 
  | STRING_LITERAL
    {
   $$ = new stringconst_astnode($1);
    $$->type = "string";
   $$->ismod = false;
  } 
  | '(' expression ')'  
  {
   $$ = $2;
  }


/* DONE */
expression_list: 
    expression 
    {
      $$ = new proccall_astnode(new identifier_astnode(""), std::vector<exp_astnode*>());
      $$->args.push_back($1);
    }
  | expression_list ',' expression 
      {
   $$ = $1;
   $$->args.push_back($3);
    }
    ;
/*
unary_operator:
    '-'
    {
      std::string unary_op = "UMINUS";
      $$ = new op_unary_astnode(unary_op, NULL);
    } 
  | '!'  
      {
      std::string unary_op = "NOT";
      $$ = new op_unary_astnode(unary_op, NULL);
    } 
  | '&'
      {
      std::string unary_op = "ADDR";
      $$ = new op_unary_astnode(unary_op, NULL);
    }  
    | '*'
      {
      std::string unary_op = "DEREF";
      $$ = new op_unary_astnode(unary_op, NULL);
    }
    ;

    */

unary_operator:
    '-'
    {
      $$ =  "UMINUS";
     
    } 
  | '!'  
      {
      $$ = "NOT";
    } 
  | '&'
      {
      $$ = "ADDRESS";
    }  
    | '*'
      {
      $$ = "DEREF";
    }
    ;

selection_statement: 
    IF '(' expression ')' statement ELSE statement 
    {
      if(!($3->type == "int" || isPoin($3->type) || $3->type =="float")){
        error(@$, "error at selection_statement");
      }
       $$ = new if_astnode($3, $5, $7);
    }
    ;

iteration_statement: 
    WHILE '(' expression ')' statement 
    {
      if(!($3->type == "int" || isPoin($3->type) || $3->type =="float")){
        error(@$, "error at iteration ");
      }
      $$ = new while_astnode($3,$5);

    }
  | FOR '(' assignment_expression ';' expression ';' assignment_expression ')' statement 
  {
     if(!($5->type == "float" || $5->type =="int" || isPoin($5->type))){
        error(@$, "error at for _statement");
      }
   $$ = new for_astnode($3, $5, $7, $9);
  }
  ;


declaration_list: 
    declaration 
    {
      $$ = new declaration_list_class();
      $$->declaration_list.push_back($1);
    }
  | declaration_list declaration 
    {
   $$ = $1;
    $$->declaration_list.push_back($2);
    }
    ;

/* DONE */
declaration:
    type_specifier declarator_list';' 
    {
      if(structdef){
        vector<declarator_class*> dec_list = $2->declarator_list;
        for(auto decl: dec_list){
          if(st->isPresent(decl->name)){
            error(@$, "multiple declarations");
          }
          if($1->type.type_name == "void" && !decl->type->isPointer ){
            error(@$, "void declaration");
          }
          int size = calculateSize($1, decl);
          std::string type_name = "";
          if(decl->type != NULL) type_name = decl->type->type_name;
          st->push(decl->name, new SymbTabEntry("var","local", size, offset, $1->type.type_name + type_name, NULL), 0,0);
          offset += size;
          currSize += size;
        }
      }
      if(funcdef){
        vector<declarator_class*> dec_list = $2->declarator_list;
        for(auto decl: dec_list){
          if(st->isPresent(decl->name)){
            error(@$, "multiple declarations");
          }
          if($1->type.type_name == "void" && !decl->type->isPointer ){
            error(@$, "void declaration");
          }
          int size = calculateSize($1, decl);
          string type_name = "";
          if(decl->type != NULL) type_name = decl->type->type_name;
          st->push(decl->name, new SymbTabEntry("var","local", size, offset, $1->type.type_name + type_name, NULL), 1,0);
          offset += size;
        }
      }
      $$ = new declaration_class($1, $2);


    };



%%
void IPL::Parser::error( const location_type &l, const std::string &err_message )
{
   std::cout << "Error at line " << l.begin.line << ": " << err_message << "\n";
  exit(1);
}


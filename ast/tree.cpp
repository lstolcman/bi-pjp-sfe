/* tree.cpp */

#include "tree.h"
#include "tabsym.h"
#include <new>          // ::operator new

// constructors and destructors

tree Node::build_string_constant(const char *string, int isConst)
{
	int length = strlen(string);
	// the string is just array of const chars so index_type and const_char_type
	// are obviously needed, size_int calls build_int_cst with type stk_sizetype
	// (normal representation of size in bytes) and the size is the size of the string
	tree index_type = build_index_type(size_int(length));
	tree const_char_type = isConst ? build_qualified_type(unsigned_char_type_node, TYPE_QUAL_CONST) : unsigned_char_type_node;
	// then the string type is the array of const chars with known size
	tree string_type = build_array_type(const_char_type, index_type);
	// for c/c++ not important but some languages may distinguis strings and arrays of chars viz tree.def
	TYPE_STRING_FLAG(string_type) = 1;
	// actual building of string
	tree res = build_string(length+1, string);
	//and the builded tree is of string type
	TREE_TYPE(res) = string_type;

	return res;
}

tree Node::build_print_integer_expr (location_t loc, tree int_expr)
{
	tree string = Node::build_string_constant("%d\n", true);

	tree *args_vec = XNEWVEC( tree, 2 );
	args_vec[0] = build1(ADDR_EXPR, build_pointer_type(TREE_TYPE(string)), string);
	args_vec[1] = int_expr;

	tree params = NULL_TREE;
	chainon( params, tree_cons (NULL_TREE, TREE_TYPE(args_vec[0]), NULL_TREE) );
	chainon( params, tree_cons (NULL_TREE, TREE_TYPE(args_vec[1]), NULL_TREE) );

	// function parameters
	tree param_decl = NULL_TREE;

	tree resdecl = build_decl (BUILTINS_LOCATION, RESULT_DECL, NULL_TREE, integer_type_node);
	DECL_ARTIFICIAL(resdecl) = true;
	DECL_IGNORED_P(resdecl) = true;

	tree fntype = build_function_type( TREE_TYPE(resdecl), params );
	tree fndecl = build_decl( UNKNOWN_LOCATION, FUNCTION_DECL, get_identifier("printf"), fntype );
	DECL_ARGUMENTS(fndecl) = param_decl;

	DECL_RESULT( fndecl ) = resdecl;

	DECL_ARTIFICIAL(resdecl) = true;
	DECL_IGNORED_P(resdecl) = true;
	DECL_EXTERNAL( fndecl ) = true;

	tree call = build_call_expr_loc_array( loc, fndecl, 2, args_vec );
	SET_EXPR_LOCATION(call, loc);
	TREE_USED(call) = true;

	return call;
}

tree Node::build_scan_integer (location_t loc, tree var_expr)
{
	tree string = build_string_constant( (const char*) "%d", true );

	tree *args_vec = XNEWVEC( tree, 2 );
	args_vec[0] = build1(ADDR_EXPR, build_pointer_type(TREE_TYPE(string)), string);
	args_vec[1] = build1(ADDR_EXPR, build_pointer_type(TREE_TYPE(var_expr)), var_expr);

	tree params = NULL_TREE;
	chainon( params, tree_cons (NULL_TREE, TREE_TYPE(args_vec[0]), NULL_TREE) );
	chainon( params, tree_cons (NULL_TREE, TREE_TYPE(args_vec[1]), NULL_TREE) );

	// function parameters
	tree param_decl = NULL_TREE;

	tree resdecl = build_decl (BUILTINS_LOCATION, RESULT_DECL, NULL_TREE, integer_type_node);
	DECL_ARTIFICIAL(resdecl) = true;
	DECL_IGNORED_P(resdecl) = true;

	tree fntype = build_function_type( TREE_TYPE(resdecl), params );
	tree fndecl = build_decl( UNKNOWN_LOCATION, FUNCTION_DECL, get_identifier("scanf"), fntype );
	DECL_ARGUMENTS(fndecl) = param_decl;

	DECL_RESULT( fndecl ) = resdecl;

	DECL_ARTIFICIAL(resdecl) = true;
	DECL_IGNORED_P(resdecl) = true;
	DECL_EXTERNAL( fndecl ) = true;

	tree call = build_call_expr_loc_array( loc, fndecl, 2, args_vec );
	SET_EXPR_LOCATION(call, loc);
	TREE_USED(call) = true;

	return call;
}

Var::Var(tree t)
{
	node = t;
}

Numb::Numb (int v)
{
	value = v;
}

int Numb::Value()
{
	return value;
}

Bop::Bop(Operator o, Expr *l, Expr *r)
{
	op = o;
	left = l;
	right = r;
}

Bop::~Bop()
{
	delete left;
	delete right;
}

UnMinus::UnMinus(Expr *e)
{
	expr = e;
}

UnMinus::~UnMinus()
{
	delete expr;
}

Assign::Assign(Var *v, Expr *e)
{
	var = v;
	expr = e;
}

Assign::~Assign()
{
	delete var;
	delete expr;
}

Write::Write(Expr *e)
{
	expr = e;
}

Write::~Write()
{
	delete expr;
}

Read::Read(Var *v)
{
	var = v;
}

Read::~Read()
{
	delete var;
}

If::If(Expr *c, Statm *ts, Statm *es)
{
	cond = c;
	thenstm = ts;
	elsestm = es;
}

If::~If()
{
	delete cond;
	delete thenstm;
	delete elsestm;
}

While::While(Expr *c, Statm *b)
{
	cond = c;
	body = b;
}

While::~While()
{
	delete body;
	delete cond;
}

StatmList::StatmList(Statm *s, StatmList *n)
{
	statm = s;
	next = n;
}

StatmList::~StatmList()
{
	delete statm;
	delete next;
}

Prog::Prog(StatmList *s)
{
	stm = s;
}

Prog::~Prog()
{
	delete stm;
}

// optimize methods

Node* Bop::Optimize()
{
	left = (Expr*) left->Optimize();
	right = (Expr*) right->Optimize();
	Numb *l = static_cast<Numb*>(left);
	Numb *r = static_cast<Numb*>(right);

	if (!l || !r)
	{
		return this;
	}
	int res;
	int leftval = l->Value();
	int rightval = r->Value();

	switch (op)
	{
	case Plus:
		res = leftval + rightval;
		break;

	case Minus:
		res = leftval - rightval;
		break;

	case Times:
		res = leftval * rightval;
		break;

	case Divide:
		res = leftval / rightval;
		break;

	case Eq:
		res = leftval == rightval;
		break;

	case NotEq:
		res = leftval != rightval;
		break;

	case Less:
		res = leftval < rightval;
		break;

	case Greater:
		res = leftval > rightval;
		break;

	case LessOrEq:
		res = leftval <= rightval;
		break;

	case GreaterOrEq:
		res = leftval >= rightval;
		break;

	case Error: //cannot happen
		break;
	}
	delete this;
	return new Numb(res);
}

Node* UnMinus::Optimize()
{
	expr = (Expr*) expr->Optimize();
	Numb *e = static_cast<Numb*>(expr);

	if (!e)
	{
		return this;
	}
	int res = -e->Value();
	delete this;
	return new Numb(res);
}

Node* Assign::Optimize()
{
	expr = (Var*)(expr->Optimize());
	return this;
}

Node* Write::Optimize()
{
	expr = (Expr*)(expr->Optimize());
	return this;
}

Node* Read::Optimize()
{
	var = (Var*)(var->Optimize());
	return this;
}

Node* If::Optimize()
{
	cond = (Expr*)(cond->Optimize());
	thenstm = (Statm*)(thenstm->Optimize());

	if(elsestm)
	{
		elsestm = (Statm*)(elsestm->Optimize());
	}
	Numb *c = static_cast<Numb*>(cond);

	if (!c)
	{
		return this;
	}
	Node *res;

	if (c->Value())
	{
		res = thenstm;
		thenstm = 0;
	}
	else
	{
		res = elsestm;
		elsestm = 0;
	}
	delete this;
	return res;
}

Node* While::Optimize()
{
	cond = (Expr*)(cond->Optimize());
	body = (Statm*)(body->Optimize());
	Numb *c = static_cast<Numb*>(cond);

	if (!c)
	{
		return this;
	}

	if (!c->Value())
	{
		delete this;
		return new Empty;
	}
	return this;
}

Node* StatmList::Optimize()
{
	StatmList *s = this;

	do
	{
		s->statm = (Statm*)(s->statm->Optimize());
		s = s->next;
	}
	while (s);
	return this;
}

Node* Prog::Optimize()
{
	stm = (StatmList*)(stm->Optimize());
	return this;
}

// translate methods

tree Var::Translate()
{
	return node;
	/*
	   Gener(TA, addr);

	   if (rvalue)
	   {
	        Gener(DR);
	   }
	 */
}

tree Numb::Translate()
{
	//Gener(TC, value);
	return build_int_cstu(integer_type_node, value);
}

tree Bop::Translate()
{
	//left->Translate();
	//right->Translate();
	//Gener(BOP, op);

	switch (op)
	{
	case Plus:
		return build2(PLUS_EXPR, integer_type_node, left->Translate(), right->Translate());
		break;

	case Minus:
		return build2(MINUS_EXPR, integer_type_node, left->Translate(), right->Translate());
		break;

	case Times:
		return build2(MULT_EXPR, integer_type_node, left->Translate(), right->Translate());
		break;

	case Divide:
		return build2(TRUNC_DIV_EXPR, integer_type_node, left->Translate(), right->Translate());
		break;

	case Eq:
		return build2(EQ_EXPR, integer_type_node, left->Translate(), right->Translate());
		break;

	case NotEq:
		return build2(NE_EXPR, integer_type_node, left->Translate(), right->Translate());
		break;

	case Less:
		return build2(LT_EXPR, integer_type_node, left->Translate(), right->Translate());
		break;

	case Greater:
		return build2(GT_EXPR, integer_type_node, left->Translate(), right->Translate());
		break;

	case LessOrEq:
		return build2(LE_EXPR, integer_type_node, left->Translate(), right->Translate());
		break;

	case GreaterOrEq:
		return build2(GE_EXPR, integer_type_node, left->Translate(), right->Translate());
		break;

	case Error: //cannot happen
		error("Bop::Translate\nop error\n");
		break;

	default:
		error("Bop::Translate\nUnknown operator\n");
	}
}

tree UnMinus::Translate()
{
	//expr->Translate();
	//Gener(UNM);
	return build1(NEGATE_EXPR, integer_type_node, expr->Translate());
}

tree Assign::Translate()
{
	return build2(MODIFY_EXPR, integer_type_node, var->Translate(), expr->Translate());
	//Gener(ST);
}

tree Write::Translate()
{
	//expr->Translate();
	//Gener(WRT);
	return build_print_integer_expr(UNKNOWN_LOCATION, expr->Translate());
}

tree Read::Translate()
{
	//var->Translate();
	//Gener(RD);
	//Gener(ST);
	return build_scan_integer(UNKNOWN_LOCATION, var->Translate());
}

tree If::Translate()
{
	//cond->Translate();
	//int a1 = Gener(IFJ);
	//thenstm->Translate();

	if (elsestm)
	{
		return build3(COND_EXPR, void_type_node, cond->Translate(), thenstm->Translate(), elsestm->Translate());
		//int a2 = Gener(JU);
		//PutIC(a1);
		//elsestm->Translate();
		//PutIC(a2);
	}
	else
	{
		return build3(COND_EXPR, void_type_node, cond->Translate(), thenstm->Translate(), NULL_TREE);
		//PutIC(a1);
	}
}

tree While::Translate()
{
	tree func_stmts_tree = alloc_stmt_list();

	// add exit condition
	tree exit_condition = build1(TRUTH_NOT_EXPR, integer_type_node, cond->Translate());
	append_to_statement_list(build1(EXIT_EXPR, void_type_node, exit_condition), &func_stmts_tree);
	// add body of a while
	append_to_statement_list(body->Translate(), &func_stmts_tree);


/*
        allocae stm list
        add the exit stm to this list
        add the body

        stm list
        add build1(EXIT_EXPR, void_type_node, cond->Translate());
        add body->Translate()

 */

	return build1(LOOP_EXPR, void_type_node, func_stmts_tree);


	/*
	   int a1 = GetIC();
	   cond->Translate();
	   int a2 = Gener(IFJ);
	   body->Translate();
	   Gener(JU, a1);
	   PutIC(a2);*/
}

tree StatmList::Translate()
{
	StatmList *s = this;
	tree func_stmts_tree = alloc_stmt_list();

	do
	{
		append_to_statement_list(s->statm->Translate(), &func_stmts_tree);
		s = s->next;
	}
	while (s);

	return func_stmts_tree;
}

tree Prog::Translate()
{
	return stm->Translate();
	//Gener(STOP);
}

Expr* VarOrConst(char *id)
{
	tree t;
	TypeId druh = varConstId(id, &t); // aquires a tabsym, retrieves some value. retrieved not int but a tree

	switch (druh)
	{
	case VarId:
	case ConstId:
		return new Var(t);

	default:
		return 0;
	}
}

#include "config.h"
#include "system.h"
#include "ansidecl.h"
#include "coretypes.h"

#include "toplev.h"

#include "diagnostic-core.h"
#include "input.h"

#include "tm.h"

#include "hash-set.h"
#include "machmode.h"
#include "vec.h"
#include "double-int.h"
#include "input.h"
#include "alias.h"
#include "symtab.h"
#include "options.h"
#include "wide-int.h"
#include "inchash.h"

#include "tree.h"
#include "fold-const.h"
#include "tree-dump.h"
#include "tree-iterator.h"

#include "tree-ssa-operands.h"
#include "tree-pass.h"
#include "tree-ssa-alias.h"
#include "bitmap.h"
#include "symtab.h"

#include "hard-reg-set.h"
#include "function.h"
#include "langhooks-def.h"
#include "langhooks.h"
#include "stringpool.h"
#include "is-a.h"

#include "gimple-expr.h"

#include "predict.h"
#include "basic-block.h"
#include "gimple.h"
#include "gimplify.h"
#include "ipa-ref.h"
#include "lto-streamer.h"
#include "cgraph.h"
#include "opts.h"

#include "print-tree.h"
#include "tree-cfg.h"
#include "tree-pretty-print.h"

void register_global_function_declaration(tree functionDecl);

void register_global_variable_declaration(tree variableDecl);

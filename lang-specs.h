/* 
   This is the contribution to the `default_compilers' array in gcc.c for
   treelang.  
   
   This file must compile with 'traditional', so no ANSI string concatenations
*/

{".sfe", "@sfe", NULL, 1, 0},
{"@sfe", "sfe1 %i %{f*} %{aaa} %{!fsyntax-only:%(invoke_as)}",
   /*cpp_spec=*/NULL, /*combinable=*/1, /*needs_preprocessing=*/0},

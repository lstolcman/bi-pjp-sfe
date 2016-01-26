# bi-pjp-sfe
New frontend able to parse a Pascal inspired language

Semestral work
==============

Extend a group of compilers gcc/llvm by a new frontend able to parse a Pascal inspired language.
The result shall be a abstract syntax tree in GENERIC format of gcc compiler/IR of llvm compiler.
* Limit the language to constant declaration, global variables declaration and function declarations and the int datatype.
* Allow numerical constant to be written in decimal, hexadecimal or octal format.
* Support arithmetic expressions with operators +, -, *, div, mod and parenthesis.
* Support assignment, if, while, for, readln and writeln.
* Support function/procedures calls with parameters including recursion.
* Input and output (writeln and readln) represent by scanf and printf function calls (examples available in the samples repository, see bellow)
* Read and write one number at a time.

Your frontend must be able to compile simple programs, these programs are mainly available so that the syntax is clear.
Inspire yourself with already existing compiler of Mila language.
(https://edux.fit.cvut.cz/courses/BI-PJP/_media/semestral_work/semestralka.zip)

Gcc compilation with empty fronted is tested on Ubuntu 30/64bit (13.10) and Opensuse 64bit (13.2).
First compilation (single-threaded) may take 1.5 hours (Core i7@2.0GHz), multi-threaded compilation is supported.
Source codes and object files take approximately 5GB of disk space.

Input of your compiler is a file with source code.
Output of your compiler is an abstract syntax tree in gcc's GENERIC format,
processed by backend that will create an executable binary file or a representation in internal format of llvm compiler,
processed by llvm so that an executable binary file is created.

Documentation of gcc about fronted creation and its structures is available on the web
(http://gcc.gnu.org/onlinedocs/gccint/ section 5. Language front Ends in GCC, 9. Passes and Files of the Compiler a 10. GENERIC).
There is a sample frontend available, showing samples of GENERIC structures for various needed constructs on
https://gitlab.fit.cvut.cz/pjp/gcc-samples . Documentation for llvm about frontend creation and its internal form can be
found on the web (http://llvm.org/docs/tutorial/).

There is a script for download gcc/llvm, empty fronted and compete compilation available.
Sample source codes are available on https://gitlab.fit.cvut.cz/pjp/semestralwork,
https://gitlab.fit.cvut.cz/pjp/gcc-emptyfrontend,
https://gitlab.fit.cvut.cz/pjp/gcc-samples and 
https://gitlab.fit.cvut.cz/pjp/llvm-emptyfrontend

Semestral work using llvm is not supported but it is accepted.

In a case you do not have Linux installed you may use images for VirtualBox with gcc source codes downloaded and
with precompiled empty frontend. 32 bit version http://users.fit.cvut.cz/~travnja3/BI-PJP/OpenSUSE_32.ova and
64 bit version http://users.fit.cvut.cz/~travnja3/BI-PJP/OpenSUSE.ova

Deadlines for submitting are:
-----------------------------

* before the 5th week of examination period – student may get full number of points
* after the 5th week of examination period – 50% penalty from the awarded number of points apply

Solutions are submitted via Progtest as zip file containing.

* Complete working frontend source code including configuration files a so on.
* At least two own sample programs in addition to provided ones.

Number of points depends on the number of features implemented:

* Main function, printing numbers, reading numbers, global variables support, expressions, assignment, number constants in decadic base: base solution, Number constants in hexadecimal and octal base (c like): 5%
* If, While (allong with exit statement): 10%
* For (to and downto; allong with exit statement): 10%
* Nested blocks: 10%
* Statically allocated arrays (indexed in any interval): 15%
* Procedures, Functions, Local variables: 15%
* Parameters of functions and procedures: 10%
* Recursion: 10%
* Indirect recursion: 15%.

The minimal percentage to get an assessment is 50%.

Source codes to study before you start to help you as mentioned on seminars:
* Lexical analyzer (lexan)
* Parser (parser)
* Implementation of translation (translation)
* Attributed translation (lltranslation)
* Construction of AST (tree)
* Translation to a pushdown computer (pushdowncomputer)
* Former semestral work samples (semestralwork/mila1,2).

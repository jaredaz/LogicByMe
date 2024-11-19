%{

MATLAB syntax cheat sheat
3-12-2019
%}


%2.9 Built-in functions for handling arrays
%{

length(A)   num of elements in the vector A
size(A)     returns a row vector [m,n], m=rows, n = columns
%}

% Logical & Relational Operators 6.1
%{
Relational Operators
<       Less than
>       Greater than
<-      Less than or equal to
>=      Greater than or equal to
==      Equal to
~=      Not equal to
Logical Operators
&       AND
|       OR
~       NOT
%}

%Order of precedence 6.1
%{
1 (highest) Parentheses (if nested parentheses exist, inner ones have precedence)
2           Exponentiation
3           Logical NOT (~)
4           Multiplication, division
5           Addition, subtraction
6           Relational operators (>,<,>=,<=,==,~=)
7           Logical AND (&)
8           Logical OR (|)
%}

% Functions 7.2
%{
User-defined Functions
- separate function file
- file name must be function_name.m

function [output arguments] = function_name (input arguments)

----------------------------
Anonymous Functions
-one line user-defined function
-define a function without creating separate function file

name = @ (arglist) expr
    name := name of anonymous function
    @ := the '@' symbol
    arglist := list of input arguments (independent varables)
    expr := mathematical expression

%}

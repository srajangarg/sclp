/*********************************************************************************
            Interpreter + compiler test file

                MinusOverload.c
                ---------------
           Use of only minus operator in an expression

This test file is using both unary minus and binary minus operator in an arithmetic
expression to distinguish between their precedence and evaluation and compilation method
*********************************************************************************/

main()
{
    int a;
    a = 3;
    a = (-a) - 3 - -a - -a - 4 - (-a - (-a));
}

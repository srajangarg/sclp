/********************************************************************************
            Interpreter + compiler test file

                GlobArith.c
                -----------
        Arithmetic operations on local and global variables

This test file demonstrates the arithmetic operations on local and global variables
********************************************************************************/

int a;
float b;

main()
{
    int c;
    float d;

    c = 2;
    d = 4.3;
    a = a * -c + a + a;
    d = -b * d / -d + b;
}

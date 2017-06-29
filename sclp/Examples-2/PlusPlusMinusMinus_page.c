/*********************************************************************************
            Interpreter + Compiler test file

                PlusPlusMinusMinus.c
                --------------------
                 Use of ++ and -- operator

This test file demonstrates the use of ++ and -- in different ways and how they are
evaluated and compiled by sclp
*********************************************************************************/

main()
{
    int a;
    float b;

    a = -3;
    b = 3;

    // Arith on int
    a++;
    a--;
    --a;
    ++a;
    a += 3;
    a = a++ + --a / -a;
    a = ++a - --a / (-a);
    a = a + -a++;

    // Arith on float
    b++;
    b--;
    --b;
    ++b;
    b -= 4;
    b = ++b + ++b + ++b - --b;
    b = b / -(b--);
}

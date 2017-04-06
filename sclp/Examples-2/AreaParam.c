/********************************************************************************
             Interpreter + Compiler test file

                AreaParam.c
                -----------
            Find area and parameter of figures

This test file calculated area and parameters of a rectangle and a circle.
*******************************************************************************/

float area;
float perimeter;

main()
{
    float x, y;
    x = 3.0;
    y = 9.9;

    // Rectangle
    area = x * y;
    perimeter = 2 * (x + y);

    // Circle
    area = 22 * x * x / 7;
    perimeter = 2 * 22 * x / 7;
}

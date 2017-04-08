float sum(float a, float b);
int main();

sum(float a, float b)
{
    float x;
    x = a + b;
    return x;
}

main()
{
    float x,y,z;
    x = 8.1;
    y = 10.4;
    z = 4.5;
    print(x);
    print("\n");
    print(y);
    print("\n");
    print(z);
    print("\n");
    z = sum(x,y);
    print(x);
    print("\n");
    print(y);
    print("\n");
    print(z);
    print("\n");
    return 0;
}

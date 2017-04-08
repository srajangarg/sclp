int main();
int fn1(int a, int b, float c);

main()
{
    int a, b;
    float c, d;
    a = 1;
    b = 2;
    c = 3.0;
    d = 4.0;
    b = fn1(a, b, c);
    print(b);
    print("\n");
    print(d);
    print("\n");
    return 2;
}

fn1(int a, int b, float c)
{
    int l;
    l = a + b;
    return l;
}
int fn2(int a, int b);
int main();

fn2(int a, int b)
{
    b = 2;
    return b;
}
main()
{
    int a;
    a = 3;
    print(a);
    print("\n");
    a = fn2(a, a);
    print(a);
    print("\n");
    return 0;   
}
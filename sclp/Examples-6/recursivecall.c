int r(int a);
void main();

r(int a)
{
    int i;
    if(a == 1)
        return 1;
    else
    {
        i = r(a-1);
        return i + 1;
    }
    return 0;
}

main()
{
    int x, y, z;
    x = 5;
    y = 10;
    z = 15;
    print(x);
    print("\n");
    print(y);
    print("\n");
    print(z);
    print("\n");
    y = r(x);
    print(x);
    print("\n");
    print(y);
    print("\n");
    print(z);
    print("\n");
    z = r(3); 
    print(x);
    print("\n");
    print(y);
    print("\n");
    print(z);
    print("\n");
    return;
}

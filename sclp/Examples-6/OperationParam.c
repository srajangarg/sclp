void main();
int fn(int a);

main()
{
	int a, b, c;
    a = 10;
    b = 2;
    print(a);
    print("\n");
    print(b);
    print("\n");
	c = fn (a + b * a / b - a + b);
    print(a);
    print("\n");
    print(b);
    print("\n");
    print(c);
    print("\n");
    return;
}

fn(int a)
{
	int c;
    c = a;
    return c;
}

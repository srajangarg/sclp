void fn1();
void fn2();
void fn3();
void main();

main()
{
	int a;
    a = 10;
    print(a);
    print("\n");
	fn1();
    print(a);
    print("\n");
	fn2();
    print(a);
    print("\n");
    return;
}

fn1()
{
	int d;
    d = 9;
    print(d);
    print("\n");
	fn3();
    print(d);
    print("\n");
    return;
}

fn2()
{
	int t;
    t = 2;
    print(t);
    print("\n");
    return;
}

fn3()
{
	int y;
    y = 0;
    print(y);
    print("\n");
    return;
}

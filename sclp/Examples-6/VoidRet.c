void fn(int a);
void main();

fn(int a)
{
	int b;
    b = 2;
    return;
}

main()
{
	int a;
    print(a);
    print("\n");
	fn(a);
    print(a);
    print("\n");
    return;
}

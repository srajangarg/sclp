void fn();
void main();

int a;
main()
{
	a = 3;
	fn();
	a = 4;
    print(a);
    print("\n");
    return;
}

fn()
{
	a = 10;
    print(a);
    print("\n");
    return;
}

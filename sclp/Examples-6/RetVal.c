int fn();
void main();
int a;
main()
{
	print(a);
    print("\n");
    a = fn();
    print(a);
    print("\n");
    return;
}

fn()
{
	return 2;
}

void main();
int fac(int a);

main()
{	
	int f, a, b, c;
	fac(5);
	f = 1;
	a = 1;
	b = f + a;
	c = 1;

	return;
}

fac(int a)
{
	int b;

	print("fac (");
	print(a);
	print(")\n");

	if (a == 0)
	{
		b = 1;
	}
	else
	{
		b = fac(a-1);
		b = b * a;
	}
	print("returned ");
	print(b);
	print("\n");
	print("\n");
	print("\n");
	return b;
}
void main();
float add1(int a1, float b1);

main()
{
	float c,d;
	int a,b;
	a=2;
	b=3;
	d = 5.0;
	c = add1(a,d);
	print(c);
	print("\n");
	return;
}

add1(int a1, float b1)
{
	float l;
	l = 2.0;
	return l+b1;
}

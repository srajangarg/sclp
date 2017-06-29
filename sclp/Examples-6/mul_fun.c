float add_fn(float a1, float b1);
float sub_fn(float a1, float b1);
float mul_fn(float a1, float b1);
float div_fn(float a1, float b1);
void main();

main()
{
	float a, b, c, d, e, f;
	a = 10.5;
	b = 1.5;
	c = 1.5;
	d = 2.5;
	e = 3.5;
	f = 4.5;
	print(a);
    print("\n");
    print(b);
    print("\n");
    print(c);
    print("\n");
    print(d);
    print("\n");
    print(e);
    print("\n");
    print(f);
    print("\n");
	c = add_fn(a, b);
	print(a);
    print("\n");
    print(b);
    print("\n");
    print(c);
    print("\n");
    print(d);
    print("\n");
    print(e);
    print("\n");
    print(f);
    print("\n");
	d = sub_fn(a, b);
	print(a);
    print("\n");
    print(b);
    print("\n");
    print(c);
    print("\n");
    print(d);
    print("\n");
    print(e);
    print("\n");
    print(f);
    print("\n");
	e = mul_fn(a, b);
	print(a);
    print("\n");
    print(b);
    print("\n");
    print(c);
    print("\n");
    print(d);
    print("\n");
    print(e);
    print("\n");
    print(f);
    print("\n");
	f = div_fn(a, b);
	print(a);
    print("\n");
    print(b);
    print("\n");
    print(c);
    print("\n");
    print(d);
    print("\n");
    print(e);
    print("\n");
    print(f);
    print("\n");
	return;
}

add_fn (float a1, float b1)
{
	return (a1+b1);
}

sub_fn (float a1, float b1)
{
	return (a1-b1);
}

mul_fn (float a1, float b1)
{
	return (a1*b1);
}

div_fn (float a1, float b1)
{
	return (a1/b1);
}

float add_fn(float a1, float b1);
void main();

main()
{
	float a, b, c;
    a = 2.0;
    b = 3.0;
    c = 4.0;
    // print(a);
    // print("\n");
    // print(b);
    // print("\n");
    // print(c);
    // print("\n");
	c = add_fn(a, b);
    print(a);
    print("\n");
    print(b);
    print("\n");
    print(c);
    print("\n");
    return;
}

add_fn(float a1, float b1)
{
	return (a1+b1);
}

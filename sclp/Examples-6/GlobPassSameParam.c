float b_1;
int a;

void fn(int c, float d);
void main();

fn(int c, float d)
{
	int a;
	float b_1;
	a = c;
	b_1 = d;
    print(a);
    print("\n");
    print(b_1);
    print("\n");
    print(c);
    print("\n");
    print(d);
    print("\n");
    return;
}

main()
{
	fn(a, b_1);
    return;
}

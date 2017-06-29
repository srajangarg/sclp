int prod;

void fact(int a);
void main();

fact(int a)
{
	if (a == 1)
		return ;
    prod = prod * a;
    print(prod);
    print("\n");
	fact(a-1);
    return;
}

main()
{
	int a;
    a = 10;
	prod = 1;
	fact(a);
    return;
}

void main();
int fac(int a, float b);

main() {
	int a, b, c, d;
	float e, f, g;

	a = 1; b = 2; c = 3; d = 4;
	e = 2.2; f = 3.3; g = 4.4;

	a = fac(4, 2.1);
	print(a);
	print("\n");
	a = fac(4, 5.1);
	print(a);
	print("\n");
	a = fac(10, 10.1);
	print(a);
	print("\n");
	a = fac(0, 0.1);
	print(a);
	print("\n");

	if ( !(a < b && e >= 2.0 || -d < 2) )
	{
		b = c+a;
		print(b);
		print("\n");
		c = -d - -a;
	}
	else
	{
		if (c == a)
		{
			print("here");
		}
		else
		{
			print(c);
			print("\n");
			c = -2 + -d/a;
			print(c);
			print("\n");
		}
	}

	a = 5;
	b = 1;

	while (a != b || 1.0 > 2.0)
	{
		print("a != b\n");
		print(a);
		print("\n");
		print(b);
		print("\n");

		a = a - 1;
		b = b + 1;
	}

	return;
}

fac(int a, float b)
{
	int k;
	if (a == 0)
		return 1;

	k = fac(a-1, b-1.0);

	if (b > 0.0 && a < 3)
	{
		print(b + 2.3);
		print("nottt\n");
	}
	else
	{
		print(b + 1.3);
		print("yesss\n");
	}

	print(k);
	print("\n");

	k = k * a;
	return k;
}
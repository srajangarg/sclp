
float r(float a);
float v(float a, float b);
void main();
float i2;

main()
{
	float x,z;
	x = 5.0;
	z = r(x);
	return;

}

r(float a)
{
	int i;
	float b;
	print("r called with args");
	print(a);
	print("\n");
	if(a <= 3.0){
		if(a > 1.0){
			i2 = r(a - 1.0);
		}
		else{
			i = 1;
			i2 = r(a - 1.0);
		}
	}
	else
	{
		b = a+10.0;
		i2 = v(a-1.0, (b > 10.0)?1.0:b );
	}
	if(2.0 < 3.0)
		return i2+1.0;
	return i2;
}


v(float a, float b)
{
	float x;

	print("v called with args ");
	print(a);
	print(", ");
	print(b);
	print("\n");
	if(a <= 1.0)
		return a;
	x  = v(a + (i2 != 1.0)?-1.0:1.0, b - (b >= 10.0)?0.0:-1.0);
	x = 2.0;
	return x;
}
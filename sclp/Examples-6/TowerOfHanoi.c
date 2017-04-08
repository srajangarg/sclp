int from_tower;
int to_tower;

void TOH(int n, int x,int y,int z);
void main();

main()
{
    int n;
    n = 5;
    TOH(n-1,100,200,300);
    return;
}
TOH(int n,int x,int  y,int z)
{
    if(n>0)
    {
        print("n = ");
        print(n);
        print(" x = ");
        print(x);
        print(" y = ");
        print(y);
        print(" z = ");
        print(z);
        print("\n");
        TOH(n-1,x,z,y);
        print("n = ");
        print(n);
        print(" x = ");
        print(x);
        print(" y = ");
        print(y);
        print(" z = ");
        print(z);
        print("\n");
        from_tower = x;
        to_tower = y;
        print("from_tower = ");
        print(from_tower);
        print(" to_tower = ");
        print(to_tower);
        print("\n");
        TOH(n-1,z,y,x);
        print("n = ");
        print(n);
        print(" x = ");
        print(x);
        print(" y = ");
        print(y);
        print(" z = ");
        print(z);
        print("\n");
    }
    return;
}

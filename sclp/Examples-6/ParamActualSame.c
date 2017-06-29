void main();
void fn(int a);

main()
{
    int a;
    a = 10;
    print(a);
    print("\n");
    fn(a);
    print(a);
    print("\n");
    return;
}

fn(int a)
{
    a = 11;
    return;
}

float catalan(float n);
float main();

catalan(float n)
{
    // Base case
    float res;
    float i;
    float a, b;
    i = 0.0;
    res = 0.0;
    if (n <= 1.0) return;
 
    // catalan(n) is sum of catalan(i)*catalan(n-i-1)
    while (i < n){
        a = catalan(i);
        b = catalan(n-i-1.0);
        res = res + a*b;
        i = i+1.0;
    }
 
    return;
}
 
// Driver program to test above function
main()
{
    // for (float i=0; i<10; i++)
    //     catalan(i) << " ";
    float x;
    x = catalan(5.0);
    print (x);
    return 0.0;
}
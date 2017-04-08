int factorial(int n);
int find_ncr(int n, int r);
int find_npr(int n, int r);
int main();

main()
{
   int n, r;
   int ncr, npr;
 
   n = 5;
   r = 3;
 
   ncr = find_ncr(n, r);
   npr = find_npr(n, r);

   print(ncr);
   print("\n");
   print(npr);
   print("\n");
 
   return 0;
}
 
find_ncr(int n, int r)
{
   int res1, res2, res3, result;

   print(res1);
   print("\n");
   print(res2);
   print("\n"); 
   print(res3);
   print("\n");  

   res1 = factorial(n);

   print(res1);
   print("\n");
   print(res2);
   print("\n"); 
   print(res3);
   print("\n");  

   res2 = factorial(r);

   print(res1);
   print("\n");
   print(res2);
   print("\n"); 
   print(res3);
   print("\n");  

   res3 = factorial(n-r);

   print(res1);
   print("\n");
   print(res2);
   print("\n"); 
   print(res3);
   print("\n");  

   result = res1/(res2*res3);

   print(res1);
   print("\n");
   print(res2);
   print("\n"); 
   print(res3);
   print("\n");

   print(result);
   print("\n");
 
   return result;
}
 
find_npr(int n, int r)
{
   int res1, res2, result;

   print(res1);
   print("\n");
   print(res2);
   print("\n");
   print(result);
   print("\n");

   res1 = factorial(n);

   print(res1);
   print("\n");
   print(res2);
   print("\n");
   print(result);
   print("\n");

   res2 = factorial(n-r);

   print(res1);
   print("\n");
   print(res2);
   print("\n");
   print(result);
   print("\n");

   result = res1/res2;

   print(res1);
   print("\n");
   print(res2);
   print("\n");
   print(result);
   print("\n");
 
   return result;
} 
 
factorial(int n)
{
   int c;
   int result;
   result = 1;
   c = 1;
   while (c <= n) {
      print(c);
      print("\n");
      print(result);
      print("\n");
      result = result*c;
      c = c+1;
   }
   print(result);
   print("\n");
   return (result);
}

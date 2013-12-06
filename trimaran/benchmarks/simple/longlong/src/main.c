struct x {int a,b;};

long long mul(long long t, long long x, long long y)
{
  long long z;
  z = x * y;
  z = z + t;
  return z;
}

struct x foo(int x, int y)
{
  struct x z;
  z.a = x + y;
  z.b = x * y;
  return z;
}

int main ()
{
  long long x, y, z;
  struct x a;
  long long t = 23;
  x = 0x000000000EADBEAFLL;
  y = 0x000000000FFFCAFELL;
  z = mul(t, x, y);
  a = foo(10, 20);
  printf("%0llX %d\n", z, a.a);
}

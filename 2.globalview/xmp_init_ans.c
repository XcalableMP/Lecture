#include <stdio.h>
#pragma xmp nodes p(2)
#pragma xmp template t(0:9)
#pragma xmp distribute t(block) onto p
int a[10]; 
#pragma xmp align a[i] with t(i)

int main(){
  int i;

#pragma xmp loop on t(i)
  for(i=0;i<10;i++)
    a[i] = i+1; 

#pragma xmp loop on t(i)
  for(i=0;i<10;i++)
    printf("%d\n", a[i]);

  return 0;
}

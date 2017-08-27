#include <stdio.h>
#include <xmp.h>
#pragma xmp nodes p[2]
#pragma xmp template t[10]
#pragma xmp distribute t[block] onto p
int a[10]; 
#pragma xmp align a[i] with t[i]

int main(){
  
#pragma xmp loop on t[i]
  for(int i=0;i<10;i++)
    a[i] = i+1; 

#pragma xmp loop on t[i]
  for(int i=0;i<10;i++)
    printf("[%d] %d\n", xmpc_node_num(), a[i]);

  return 0;
}

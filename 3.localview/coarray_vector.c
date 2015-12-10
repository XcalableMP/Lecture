#include <stdio.h>
#include <xmp.h>
int a[10]:[*], b[10]:[*], c[10][10]:[*];
int status;

int main(){
  int i, j, me = xmp_node_num();

  for(i=0;i<10;i++)
    a[i] = b[i] = i + 10 * (me-1);

  for(i=0;i<10;i++)
    for(j=0;j<10;j++)
      c[i][j] = (i * 10 + j) + 100 * (me-1);

  xmp_sync_all(&status);

  if(me == 1){
    a[0:3] = a[5:3]:[2];            // Get
    printf("My node is %d:\n", me);
    for(i=0;i<10;i++)
      printf("  a[%d] = %d\n", i, a[i]);

    b[0:5:2] = b[0:5:2]:[2];       // Get
    printf("\nMy node is %d:\n", me);
    for(i=0;i<10;i++)
      printf("  b[%d] = %d\n", i, b[i]);

    c[0:5][0:5]:[2] = c[0:5][0:5]; // Put
  }
  xmp_sync_all(&status);

  if(me == 2){
    printf("\nMy node is %d:\n", me);
    for(i=0;i<10;i++){
      for(j=0;j<10;j++){
	printf("  %3d",c[i][j]);
      }
      printf("\n");
    }
  }

  return 0;
}

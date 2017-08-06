#include <stdio.h>
#include <xmp.h>
int a[10]:[*], b[10]:[*], c[10][10]:[*];

int main(){
  int i, j, me = xmpc_this_image();

  for(i=0;i<10;i++)
    a[i] = b[i] = i + 10 * me;

  for(i=0;i<10;i++)
    for(j=0;j<10;j++)
      c[i][j] = (i * 10 + j) + 100 * me;

  xmp_sync_all(NULL);

  if(xmpc_this_image() == 0){
    a[0:3] = a[5:3]:[1];            // Get
    printf("My image is %d:\n", me);
    for(i=0;i<10;i++)
      printf("  a[%d] = %d\n", i, a[i]);

    b[0:5:2] = b[0:5:2]:[1];       // Get
    printf("\nMy image is %d:\n", me);
    for(i=0;i<10;i++)
      printf("  b[%d] = %d\n", i, b[i]);

    c[0:5][0:5]:[1] = c[0:5][0:5]; // Put
  }
  xmp_sync_all(NULL);

  if(xmpc_this_image() == 0){
    printf("\nMy image is %d:\n", me);
    for(i=0;i<10;i++){
      for(j=0;j<10;j++){
	printf("  %3d",c[i][j]);
      }
      printf("\n");
    }
  }

  return 0;
}

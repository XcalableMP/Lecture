#include <stdio.h>
#include <xmp.h>

int val:[*], tmp = 0;

int main(){
  int me = xmpc_this_image();
  val    = me + 1;

  printf("[START] My image is %d, val = %d, tmp = %d\n", me, val, tmp);

  xmp_sync_all(NULL);

  if(xmpc_this_image() == 0){
    tmp = val:[1];  // get
    val:[1] = val;  // put
  }
  xmp_sync_all(NULL);
  
  printf("[END]   My image is %d, val = %d, tmp = %d\n", me, val, tmp);
  
  return 0;
}

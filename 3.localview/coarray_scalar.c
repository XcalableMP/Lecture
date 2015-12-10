#include <stdio.h>
#include <xmp.h>

int val:[*], status, tmp = 0;

int main(){
  int me = xmp_node_num();
  val = me;

  printf("[START] My node is %d, val = %d, tmp = %d\n", me, val, tmp);

  xmp_sync_all(&status);

  if(me == 1){
    tmp = val:[2];  // get
    val:[2] = val;  // put
  }
  xmp_sync_all(&status);
  
  printf("[END]   My node is %d, val = %d, tmp = %d\n", me, val, tmp);
  
  return 0;
}

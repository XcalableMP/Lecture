program coarray_vector
  implicit none
  include "xmp_coarray.h"
  integer :: a(10)[*], b(10)[*], c(10,10)[*]
  integer :: i, j, me

  me = this_image()

  do i=1, 10
     b(i) = (i-1) + 10 * (me - 1)
     a(i) = b(i)
  end do

  do i=1, 10
     do j=1, 10
        c(j,i) = ((i-1) * 10 + (j-1)) + 100 * (me - 1)
     end do
  end do
  
  sync all

  if (me .eq. 1) then
     a(1:3) = a(6:8)[2] ! Get
     write (*, "(A10, I2)") "My node is", me
     do i=1, 10
        write(*,"(A5,I2,A5,I2)") "a(",i,") = ", a(i)
     end do
     
     b(1:10:2) = b(1:10:2)[2];  ! Get
     write(*,*) ""
     write (*, "(A10, I2)") "My node is", me
     do i=1, 10
        write(*,"(A5,I2,A5,I2)") "b(",i,") = ", b(i)
     end do

     c(1:5,1:5)[2] = c(1:5,1:5) ! Put
  end if

  sync all

  if (me .eq. 2) then
     write(*,*) ""
     write (*, "(A10, I2)") "My node is", me
     do i=1, 10
        write(*,*) c(:,i)
     end do
  end if
end program coarray_vector
program init
!$xmp nodes p(2)
!$xmp template t(10)
!$xmp distribute t(block) onto p
  integer :: a(10) 
  integer :: i

  do i=1,10
    a(i)=i 
  end do

  do i=1,10
    print *, a(i)
  end do

end program init

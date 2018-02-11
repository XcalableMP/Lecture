program matmul_ans
  implicit none
  include "xmp_coarray.h"
  integer,parameter :: N = 1024
  real :: A(N,N)[*], B(N,N)[*], C(N,N)[*]  ! c = a x b
  integer :: t1, t2, t_rate, t_max, diff
  integer :: seedsize
  integer,allocatable:: seed(:)

  if (num_images() .ne. 4 ) then
     write(*,*) "This program must be executed by 4 nodes."
  end if

  call random_seed(size=seedsize)
  allocate(seed(seedsize))
  call random_seed(get=seed)
  
  ! Serial Implementation
  if (this_image() .eq. 1 ) then
     call init_dmat()          ! matrix initialization
     call system_clock(t1)
     call mul_dmat(1, N, 1, N) ! matrix multiply
     call system_clock(t2, t_rate, t_max)
     if ( t2 < t1 ) then
        diff = (t_max - t1) + t2 + 1
     else
        diff = t2 - t1
     endif
     
     write(*,*) "Serial:"
     print "(A, F10.3)", "  time it took was:", diff/dble(t_rate)
     print "(A, F10.3)", "  verify          :", sum(C)
  end if
  ! End Serial Implementation

  ! Parallel Implementation
  if (this_image() .eq. 1 ) then
     call init_dmat()        ! matrix initialization
  end if

  sync all
  call system_clock(t1)

  call move_data()  ! transports data
  sync all

  if (this_image() .eq. 1 ) then
     call mul_dmat(1, N/2, 1, N/2)
  else if (this_image() .eq. 2 ) then
     call mul_dmat(1, N/2, N/2+1, N)
  else if (this_image() .eq. 3 ) then
     call mul_dmat(N/2+1, N, 1, N/2)
  else if (this_image() .eq. 4 ) then
     call mul_dmat(N/2+1, N, N/2+1, N)
  endif
  sync all

  call gather_data()
  ! END Parallel Implementation

  sync all
  call system_clock(t2, t_rate, t_max)
  if ( t2 < t1 ) then
     diff = (t_max - t1) + t2 + 1
  else
     diff = t2 - t1
  endif

  if (this_image() .eq. 1 ) then
     write(*,*) "Parallel:"
     print "(A, F10.3)", "  time it took was:", diff/dble(t_rate)
     print "(A, F10.3)", "  verify          :", sum(C)
  end if
  
contains  
  subroutine init_dmat()
    call random_seed(put=seed)
    call random_number(A)
    call random_number(B)
    A(:,:) = A(:,:) - 0.5  ! -0.5 <= A < 0.5
    B(:,:) = B(:,:) - 0.5  ! -0.5 <= B < 0.5
  end subroutine init_dmat

  subroutine mul_dmat(start_i, end_i, start_k, end_k)
    integer :: start_i, end_i, start_k, end_k
    integer :: i, j, k

    C(:,:) = 0.0

    do i=start_i, end_i
       do j=1, N
          do k=start_k, end_k
             C(k,i) = C(k,i) + A(j,i) * B(k,j)
          end do
       end do
    end do
  end subroutine mul_dmat
  
  subroutine gather_data()
    integer :: me
    me = this_image()
    if (me .eq. 1) then
       C(N/2+1:N,:N/2) = C(N/2+1:N,:N/2)[2]
       C(:N/2,N/2+1:N) = C(:N/2,N/2+1:N)[3]
       C(N/2+1:N,N/2+1:N) = C(N/2+1:N,N/2+1:N)[4]
    end if
  end subroutine gather_data

  subroutine move_data()
    if(this_image() .eq. 1) then
       ! Please implement THIS
    end if
  end subroutine move_data
end program matmul_ans

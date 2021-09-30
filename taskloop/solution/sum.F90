program taskloop
  implicit none

  integer, parameter :: nx = 102400
  integer, dimension(nx) :: veca, vecb
  integer :: i, sum

  vecA = [(i, i = 1, nx)]
  vecB = [(-i, i = 1, nx)]

  sum = 0.0;

  ! First sum 
  do i = 1, nx
    sum = sum + vecA(i)
  end do

  ! Second sum 
  do i = 1, nx
    sum = sum + vecB(i)
  end do

  write(*,*) 'Reduction sum', sum

end program

program taskloop
  implicit none

  integer, parameter :: nx = 102400
  integer, dimension(nx) :: veca, vecb
  integer :: i, sum

  vecA = [(i, i = 1, nx)]
  vecB = [(-i, i = 1, nx)]

  sum = 0.0;

  !$omp parallel private(i), reduction(+:sum) 
  !$omp single
  !$omp taskgroup

  !$omp task
  ! First sum 
  !$omp taskloop
  do i = 1, nx
    sum = sum + vecA(i)
  end do
  !$omp end taskloop
  !$omp end task

  !$omp task
  ! Second sum 
  !$omp taskloop 
  do i = 1, nx
    sum = sum + vecB(i)
  end do
  !$omp end taskloop
  !$omp end task

  !$omp end taskgroup
  !$omp end single
  !$omp end parallel

  write(*,*) 'Reduction sum', sum

end program

set terminal wxt
set parametric
set hidden3d
set mouse
set urange [0:2*pi]
set vrange [0:pi]
set xlabel 'X'
set ylabel 'Y'
set zlabel 'Z'
set view equal xyz
r = 1.25
splot 'data/original_sphere.dat' with linespoints pointtype 7 linecolor 'blue'

set term png
set output 'timers.png'
set xrange[1:4]
set yrange[0:1]
plot 'timers.out' w lp

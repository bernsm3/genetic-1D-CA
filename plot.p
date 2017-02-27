set style line 1 lt 2 lc rgb "red" lw 4

set palette gray negative
set termoption enhanced
set xlabel font 'Calibri,14'
set ylabel font 'Calibri,14'

set style line 3
set xlabel "Space"
set ylabel "Time"
set size ratio -1
set xrange [0:201]
set yrange [0:500]
unset tics
unset key
unset colorbox

plot infile matrix w image

set out
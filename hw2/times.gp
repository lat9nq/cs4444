set terminal latex
set output "times.tex"
set xlabel "Cores"
set ylabel "\\begin\{sideways\}Render time (seconds)\\end\{sideways\}"
unset key
plot "times.dat" using 1:3 with linespoints

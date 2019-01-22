set terminal png
#
set output "speed.png"
set size nosquare
plot "orbit.txt" u 2:11 w l title "vx","orbit.txt" u 2:12 w l title "vy","orbit.txt" u 2:13 w l title "vmag",0 w l ls 0 notitle
#
set output "distance.png"
set size nosquare
plot "orbit.txt" u 2:8 w l title "r", 6.371e+6 w l ls 0 title "R", "orbit.txt" u 2:14 w l title "a"
#
set output "orbit.png"
set size square
set xrange[-7e8:7e8]
set yrange[-7e8:7e8]
plot "earth.txt" w l notitle, "orbit.txt" u 3:4 w l notitle, "orbit.txt" u 9:10 w l notitle
#

import sys

f = open(sys.argv[1])

a = []
for line in f:
	s = line.strip() + " 0 0 255 0.2 0 x"
	a.append(s)

n = len(a)
f.close()
f = open(sys.argv[1][2:], "w")
f.write(str(n)+"\n")
f.write("\n".join(a))
f.write("\n")
f.close()

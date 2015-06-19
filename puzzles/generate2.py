f=open("temp","r")
a=f.readlines()
for j in range(0,10000):
	g=open(str(j),"w")
	for i in range(6*j,6*(j+1)):
		g.write(a[i])
	g.close()

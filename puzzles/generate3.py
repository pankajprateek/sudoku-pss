f=open("temp","r")
a=f.readlines()
for j in range(0,200):
	g=open(str(j),"w")
	for i in range(12*j,12*(j+1)):
		g.write(a[i])
	g.close()

import hdc_python as hdc
import numpy as np
c = hdc.Container(np.array([1,2,3],dtype="int64"), 3, 11)
print(c[0],c[1],c[2])
c[0] = 111
print(c)
print(c[0],c[1],c[2])

t = hdc.HDC()
t.addNode("aaa/bbb",c)
cc = t.getNode("aaa/bbb")
print("end of get")
print(cc)
print(cc[0],cc[1],cc[2])

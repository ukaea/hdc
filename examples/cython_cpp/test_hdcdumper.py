import cyhdcdumper
from pyhdc import HDC
print("-- create HDCDumper()")
d = cyhdcdumper.HDCDumper()
print("-- call HDCDumper.make_hdc_t()")
h = d.make_hdc_t()

print("-- convert it to HDC")
hh = HDC(h)

print("-- dump the result")
print(hh.dumps())

print("-- Done.")

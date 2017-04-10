import numpy as np
import sys
import os

# import the base HDC class
import pyhdc2
from pyhdc2 import HDC

hdc = HDC()

print('---')

# assert 'aa' not in hdc

# hdc['aa'] = 'test'
# hdc._hdc_obj.set_data_double('aa', 1.23)
hdc._hdc_obj.set_string('aa', "AA")
assert 'aa' in hdc

print('---')

aa = hdc['aa']
print(type(aa))
aa.dump()

print('---')

key = 'bb'
val = "1.23"
hdc[key] = val

print('--')

# print(hdc['bb'])

print("--dump---")
hdc.dump()

print('well done')
print('---------')

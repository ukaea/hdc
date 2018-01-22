from cyhdc import HDC
import numpy as np

empty = HDC()
assert empty.dumps().strip() == 'null'

s = HDC('Ahoj')
print(s.dumps())

tree = HDC()
tree['a'] = 'a'
tree['sub/sub/x'] = 'x'
assert 'a' in tree
assert 'sub' in tree
assert 'A' not in tree
assert 'SUB' not in tree
# assert tree['sub']['sub'] == tree['sub/sub']

print(tree.dumps())
print(tree['sub/sub'].dumps())

print("-- set data")
x = np.arange(30, dtype=np.float64)
h = HDC()
h.set_data(x)
print(h.dumps())

print("-- set data slice")
x2 = np.arange(2 * x.size, dtype=np.float64)
h = HDC()
h.set_data(x2[::2])
print(h.dumps())
print(h.dumps())

print("-- asarray")
y = np.asarray(h)
print(y)
print('-- modify x2[0] y[1] and dump HDC')
x2[0] = - 1 / 2
y[1] = - 1 / 4
print(h.dumps())

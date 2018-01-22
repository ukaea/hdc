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
x = np.arange(2, dtype=np.float64)
h = HDC()
h.set_data(x)
print(h.dumps())

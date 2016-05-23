# to be run from the same folder where libhdc_fortran is
import libhdc_python as hdc
import ctypes

hdc.hello__()
#h = hdc.HDC()
h = hdc.hdc()
h['group/data'] = np.random.rand(3, 4)

fmodule = ctypes.cdll.LoadLibrary('libhdc_fortran_module.so')

change_data = fmodule.change_data
change_data.restype = ctypes.c_void_p
change_data.argtypes = (hdc.hdc_t, )

# call fortran
change_data(h.c_ptr)
print(h['new_dataset'])

# create hdc in fortran
hdc_ptr_f = hdc_new_empty()
# to python object
g = hdc.from_c_ptr(hdc_ptr_f)

g = hdc.hdc()
h.add_child("aaa/bbb",g)
h.set_data_double("aaa/bbb", 55.5)
d = h.as_double("aaa/bbb")
print(d)
k = h.get_child("aaa")
k.set_data_int32("bbb",323232)
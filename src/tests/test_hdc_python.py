# to be run from the same folder where libhdc_fortran is
import libhdc_python as hdc

hdc.hello__()
h = hdc.hdc()
g = hdc.hdc()
h.add_child("aaa/bbb",g)
h.set_data_double("aaa/bbb", 55.5)
k = h.get_child("aaa")
k.set_data_int32("bbb",323232)

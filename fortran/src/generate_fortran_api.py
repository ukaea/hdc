#!/usr/bin/env python3
from jinja2 import Environment, FileSystemLoader
import datetime

env = Environment(
    loader=FileSystemLoader(''), trim_blocks=True, lstrip_blocks=True
)
now = datetime.datetime.now()

header = """
! This file was generated on {} {} by {}
! Please, edit the hdc_fortran.f90.template file instead and run the python script.
""".format(datetime.datetime.date(now),datetime.datetime.time(now),__file__)

types = [
    ("int8",    "HDC_INT8",   "integer(kind=c_int8_t)"),
    ("int16",   "HDC_INT16",  "integer(kind=c_int16_t)"),
    ("int32",   "HDC_INT32",  "integer(kind=c_int32_t)"),
    ("int64",   "HDC_INT64",  "integer(kind=c_int64_t)"),
    #("uint8",   "HDC_UINT8",  "integer(kind=c_int8_t)"),
    #("uint16",  "HDC_UINT16", "integer(kind=c_int16_t)"),
    #("uint32",  "HDC_UINT32", "integer(kind=c_int32_t)"),
    #("uint64",  "HDC_UINT64", "integer(kind=c_int64_t)"),
    ("float",   "HDC_FLOAT",  "real(kind=sp)"),
    ("double",  "HDC_DOUBLE", "real(kind=dp)"),
]
ranks = [
    ( 1,":"),
    ( 2,":,:"),
    ( 3,":,:,:"),
    ( 4,":,:,:,:"),
    ( 5,":,:,:,:,:"),
    ( 6,":,:,:,:,:,:"),
    ( 7,":,:,:,:,:,:,:"),
    #( 8,":,:,:,:,:,:,:,:"),
    #( 9,":,:,:,:,:,:,:,:,:"),
    #(10,":,:,:,:,:,:,:,:,:,:")
]





template = env.get_template('hdc_fortran.f90.template')
rendered = template.render(types=types, ranks=ranks, header=header)
#print(rendered)
with open("hdc_fortran.f90","w") as f:
    f.write(rendered)


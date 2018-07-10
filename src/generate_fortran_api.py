from jinja2 import Environment, FileSystemLoader
env = Environment(
    loader=FileSystemLoader(''), trim_blocks=True, lstrip_blocks=True
)


types = [
    ("int8",    "HDC_INT8"),
    #("int16",   "HDC_INT16"),
    #("int32",   "HDC_INT32"),
    #("int64",   "HDC_INT64"),
    #("uint8",   "HDC_UINT8"),
    #("uint16",  "HDC_UINT16"),
    #("uint32",  "HDC_UINT32"),
    #("uint64",  "HDC_UINT64"),
    #("float",   "HDC_FLOAT"),
    #("double",  "HDC_DOUBLE"),
]
ranks = [
    ( 1,":"),
    #( 2,":,:"),
    #( 3,":,:,:"),
    #( 4,":,:,:,:"),
    #( 5,":,:,:,:,:"),
    #( 6,":,:,:,:,:,:"),
    #( 7,":,:,:,:,:,:,:"),
    #( 8,":,:,:,:,:,:,:,:"),
    #( 9,":,:,:,:,:,:,:,:,:"),
    #(10,":,:,:,:,:,:,:,:,:,:")
]





template = env.get_template('hdc_fortran.f90.template')
rendered = template.render(types=types, ranks=ranks)
print(rendered)
with open("hdc_fortran.f90.rendered","w") as f:
    f.write(rendered)


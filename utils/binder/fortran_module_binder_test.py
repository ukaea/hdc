import jinja2


func = {
    'name': 'test_cpos',
    'module': 'hdc_fortran_test_module',
    'arguments': [
        {
            'name': 'equilibriumin',
            'type': 'type(hdc_t)',
            'intent': 'inout'
        },
        {
            'name': 'int_in',
            'type': 'integer',
            'intent': 'in'
        },
        {
            'name': 'distsourceout',
            'type': 'type(hdc_t)',
            'intent': 'out'
        },
        {
            'name': 'float_out',
            'type': 'real(8)',
            'intent': 'out'
        },
    ],
}


template = jinja2.Template(open('binder_template.f90.j2', 'r').read())
bind_src = template.render(func=func)

print(bind_src)

with open('hdc_fortran_test_module_bind.f90', 'w') as wrap_out:
    wrap_out.write(bind_src)

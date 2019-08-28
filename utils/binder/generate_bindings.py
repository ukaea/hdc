#! /usr/bin/env python3
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

templates = [
    ('Makefile.j2', 'Makefile'),
    ('binder_template.f90.j2', '{}_bind.f90'.format(func["module"])),
    ('python_interface.py.j2', '{}_interface.py'.format(func["module"]))
    ]

for file_in, file_out in templates:
    template = jinja2.Template(open(file_in, 'r').read())
    bind_src = template.render(func=func)

    with open(file_out, 'w') as wrap_out:
        wrap_out.write(bind_src)
        print("{} written.".format(file_out))

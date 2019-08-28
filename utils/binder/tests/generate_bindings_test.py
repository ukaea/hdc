#! /usr/bin/env python
import jinja2

funcs = []

funcs.append({
    'name': 'triple_int',
    'module': 'test_module_triple_int',
    'arguments': [
        {
            'name': 'int_in',
            'type': 'integer(kind=c_int64_t)',
            'intent': 'in'
        },
        {
            'name': 'int_out',
            'type': 'integer(kind=c_int64_t)',
            'intent': 'out'
        },
    ],
})

funcs.append({
    'name': 'add_double',
    'module': 'test_module_add_double',
    'arguments': [
        {
            'name': 'x1',
            'type': 'real(kind=c_double)',
            'intent': 'in'
        },
        {
            'name': 'x2',
            'type': 'real(kind=c_double)',
            'intent': 'in'
        },
        {
            'name': 'y',
            'type': 'real(kind=c_double)',
            'intent': 'out'
        },
    ],
})

funcs.append({
    'name': 'modify_string',
    'module': 'test_module_modify_string',
    'arguments': [
        {
            'name': 'str_in',
            'type': 'character(kind=c_char,len=100)',
            'intent': 'in'
        },
        {
            'name': 'str_out',
            'type': 'character(kind=c_char,len=201)',
            'intent': 'out'
        },
    ],
})

funcs.append({
    'name': 'merge_hdc_trees',
    'module': 'test_module_merge_hdc_trees',
    'arguments': [
        {
            'name': 'tree1',
            'type': 'type(hdc_t)',
            'intent': 'in'
        },
        {
            'name': 'tree2',
            'type': 'type(hdc_t)',
            'intent': 'in'
        },
        {
            'name': 'tree_out',
            'type': 'type(hdc_t)',
            'intent': 'out'
        },
    ],
})



for func in funcs:
    templates = [
        ('binder_template.f90.j2', '{}_bind.f90'.format(func["module"])),
        ('python_interface.py.j2', '{}_interface.py'.format(func["module"]))
        ]

    for file_in, file_out in templates:
        template = jinja2.Template(open(file_in, 'r').read())
        bind_src = template.render(func=func)

        with open(file_out, 'w') as wrap_out:
            wrap_out.write(bind_src)
            print("{} written.".format(file_out))

file_in, file_out = ("Makefile.j2", "Makefile")
template = jinja2.Template(open(file_in, 'r').read())
bind_src = template.render(funcs=funcs)

with open(file_out, 'w') as wrap_out:
    wrap_out.write(bind_src)
    print("{} written.".format(file_out))

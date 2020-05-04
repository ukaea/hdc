import json
import pkg_resources

import click
import jinja2


@click.command()
@click.argument('description_file', type=click.File("r"))
@click.option('--makefile', default='Makefile_binder', show_default=True, help='Make file name, leave empty for no make file generation')
def cli(description_file, makefile):
    """Contructs HDC bindings according to the description

    DESCRIPTION_FILE is a JSON file providing the interface description
    """
    description = json.load(description_file)

    bind_module_filename = description["module"] if description["module"] else description["function"]

    templates = [
        (pkg_resources.resource_string('pyhdc', 'templates/binder_template.f90.j2'), '{}_bind.f90'.format(bind_module_filename)),
        (pkg_resources.resource_string('pyhdc', 'templates/python_interface.py.j2'), '{}_interface.py'.format(bind_module_filename)),
        ]
    if makefile:
        templates.append((pkg_resources.resource_string('pyhdc', 'templates/Makefile.j2'), makefile))

    for file_in, file_out in templates:
        template = jinja2.Template(file_in.decode())
        bind_src = template.render(func=description)

        with open(file_out, 'w') as wrap_out:
            wrap_out.write(bind_src)
            click.echo("{} written.".format(file_out))

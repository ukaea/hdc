import json
import pkg_resources

import click
import jinja2


@click.command()
@click.argument('description_file', type=click.File("r"))
def cli(description_file):
    """Contructs HDC bindings according to the description

    DESCRIPTION_FILE is a JSON file providing the interface description
    """
    description = json.load(description_file)

    templates = [
        (pkg_resources.resource_string(__name__, 'templates/binder_template.f90.j2'), '{}_bind.f90'.format(description["module"])),
        (pkg_resources.resource_string(__name__, 'templates/python_interface.py.j2'), '{}_interface.py'.format(description["module"]))
        ]

    for file_in, file_out in templates:
        template = jinja2.Template(file_in.decode())
        bind_src = template.render(func=description)

        with open(file_out, 'w') as wrap_out:
            wrap_out.write(bind_src)
            click.echo("{} written.".format(file_out))

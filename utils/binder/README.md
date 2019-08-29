`binder` generates bindings (Python interface to FORTRAN function) via HDC containers.

The `binder` script inputs a JSON interface description file and outputs
Python and Fortran binding files using HDC containers and a make file
(`Makefile_binder` by default), which can be used to compile shared libraries.

Internally, `binder` puts all inputs into a single HDC container,
which is unpacked in the generated binding functions. Similarly,
all outputs are stored into an output container and unpacked
in the (Python) interface function.

Command line interface:

```
$ binder --help

Usage: binder [OPTIONS] DESCRIPTION_FILE

  Contructs HDC bindings according to the description

  DESCRIPTION_FILE is a JSON file providing the interface description

Options:
  --makefile TEXT  Make file name, leave empty for no make file generation
                   [default: Makefile_binder]
  --help           Show this message and exit.


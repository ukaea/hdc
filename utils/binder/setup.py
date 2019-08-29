from setuptools import setup

setup(
    name='binder',
    version='0.10.0',
    install_requires=[
        'Click',
        'jinja2',
        'pyhdc',
    ],
    entry_points='''
        [console_scripts]
        binder=binder:cli
    ''',
    packages=['binder'],
    package_data={'binder': ['templates/*.j2']},
)

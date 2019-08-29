from setuptools import setup

setup(
    name='binder',
    version='0.1.0',
    install_requires=[
        'Click',
        'jinja2',
    ],
    entry_points='''
        [console_scripts]
        binder=binder:cli
    ''',
    packages=['binder'],
    package_data={'binder': ['templates/*.j2']},
)

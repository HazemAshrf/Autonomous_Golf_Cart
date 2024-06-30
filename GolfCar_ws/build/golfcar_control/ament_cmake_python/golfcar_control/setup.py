from setuptools import find_packages
from setuptools import setup

setup(
    name='golfcar_control',
    version='0.0.0',
    packages=find_packages(
        include=('golfcar_control', 'golfcar_control.*')),
)

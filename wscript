#! /usr/bin/env python

import os
import platform
import json
from shutil import copyfile

VERSION='0.0.0'
APPNAME='rational_geometry'

top = '.'
out = 'bin'

def options(opt):
    opt.load('compiler_cxx')

def configure(conf):
    conf.load('compiler_cxx waf_unit_test')

    if platform.system() == 'Windows':
        conf.env.CXXFLAGS = ['/nologo', '/EHsc', '/MD']

local_resources_candidates = ['./local_resources.json', '~/.local_resources.json']

def build(bld):
    resource_file_name = ""

    for candidate in local_resources_candidates:
        candidate_abs = os.path.abspath(os.path.expanduser(candidate))
        if os.path.isfile(candidate_abs):
            resource_file_name = candidate_abs
            break

    if resource_file_name == "":
        print('Error: No path file given for resource files ("local_resources.json")\n')
        print('(See the "about this file" field in local_resources_template.json .')
        exit(1)

    # TODO: make into a do-while loop
    with open(resource_file_name) as resource_file:
        resources = json.load(resource_file)
        if 'redirect' in resources:
            actual_data_name = resources['redirect']
            with open(actual_data_name) as actual_resource_file:
                resources = json.load(actual_resource_file)

    # directories containing <> includes
    my_includes = [ resources['doctest'][-1]['include'] ]

    my_source = [
            'src/test.cpp']

    bld.program(
            source   = my_source,
            features = 'cxx cxxprogram',
            target   = 'rational_geometry_test',
            includes = my_includes)

# vim:set et ts=4 sts=4 sw=4 ft=python:


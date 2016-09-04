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
    opt.load('compiler_cxx waf_unit_test')

def configure(conf):
    # TODO figure out how to use waf_unit_test correctly. That is, get it to
    #      display the output from doctest.
    conf.load('compiler_cxx waf_unit_test')
    # conf.load('compiler_cxx')

    # TODO check for compiler in use, rather than OS.
    # TODO evaluate usefulness of these arguments
    if platform.system() == 'Windows':
        conf.env.CXXFLAGS = ['/nologo', '/EHsc', '/MD']

def build(bld):
    my_source = ['tests/test.cpp', 'tests/Point.test.cpp']
    
    bld.program(
            source   = my_source,
            features = 'cxx cxxprogram test',
            target   = 'rational_geometry_test')

    bld.options.all_tests = True

    from waflib.Tools import waf_unit_test
    bld.add_post_fun(waf_unit_test.summary)

# vim:set et ts=4 sts=4 sw=4 ft=python:


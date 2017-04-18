#! /usr/bin/env python

from shutil import copyfile
from subprocess import call
import os
import platform

VERSION='0.0.0'
APPNAME='rational_geometry'

top = '.'
out = 'bin'

def options(opt):
    opt.load('compiler_cxx')

def configure(conf):
    conf.env.MSVC_VERSIONS = ['msvc 14.0']
    conf.env.MSVC_TARGETS = ['x64']
    conf.env.CXXFLAGS = ['/nologo', '/EHsc', '/MD']
    conf.load('compiler_cxx msvc')

def post(ctx):
    print("\n -- Running Unit tests: --\n")
    call(['./bin/rational_geometry_test.exe'])
    print("\n -- End Unit tests --\n")

def build(bld):
    my_source = ['tests/test.cpp'
            , 'tests/unrepresentable_operation_error.test.cpp'
            , 'tests/common_factor.test.cpp'
            , 'tests/FixedRational.test.cpp'
            , 'tests/Point.test.cpp'
            , 'tests/Matrix.test.cpp'
            , 'tests/Direction.test.cpp']

    bld.program(
            source   = my_source,
            features = 'cxx cxxprogram',
            target   = 'rational_geometry_test')

    bld.add_post_fun(post)

# vim:set et ts=4 sts=4 sw=4 ft=python:


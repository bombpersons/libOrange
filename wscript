#! /usr/bin/env python
# encoding: utf-8

APPNAME = 'libOrange'
VERSION = '0.1'

top = '.'
out = 'build'

def options(ctx):
    ctx.load('compiler_cxx')
    ctx.add_option('--debug', action='store', default=False, help='Build with debug symbols')
    ctx.recurse('glad libOrange orangeTest')

def configure(ctx):
    ctx.load('compiler_cxx')
    ctx.env.append_value('CXXFLAGS', ['-std=c++11'])
    if ctx.options.debug == 'true':
        ctx.env.append_value('CXXFLAGS', ['-g', '-Wall'])
    else:
        ctx.env.append_value('CXXFLAGS', ['-O3'])

    ctx.recurse('glad libOrange orangeTest')

def build(ctx):
    ctx.recurse('glad libOrange orangeTest')

#!python


import os
from os import path
import stat
import shutil


MODULES = [ "base", "windowing" ]
build_dir = ""
current_module = ""


def makedirs(p):
    if not path.exists(p):
        os.makedirs(p)


# called by module.py
def process_idl(list):

    for i in list:

        input  = path.join(current_module, "idl", i)
        output = path.join(build_dir, "idl", i)

        should_build = 0

        try:
            itime = os.stat(input)[stat.ST_MTIME]
            otime = os.stat(output)[stat.ST_MTIME]
            should_build = itime > otime
        except OSError:
            should_build = 1

        if should_build:
            print i
            shutil.copyfile(input, output)


def build_module(module):
    global current_module
    
    print "Building module '" + module +  "'"
    current_module = module
    execfile(path.join(module, "module.py"))


def main():

    global build_dir

    # figure out if we should parse a command file
    # XXX todo

    # parse command line arguments and environmental variables
    if os.environ.has_key("NK_BUILD_DIR"):
        build_dir = os.environ["NK_BUILD_DIR"]
    if (len(build_dir) == 0):
        build_dir = "dist"

    # convert to absolute path
    build_dir = path.abspath(build_dir)

    # create the build directories
    makedirs(build_dir)
    makedirs(path.join(build_dir, "idl"))
    makedirs(path.join(build_dir, "include"))
    makedirs(path.join(build_dir, "lib"))
    makedirs(path.join(build_dir, "xpt"))
    makedirs(path.join(build_dir, "bin"))

    # compile each module
    for i in MODULES:
        build_module(i)


main()

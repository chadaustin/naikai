#!python


import os
from os import path
import stat
import shutil


MODULES = [ "base", "windowing" ]
build_dir = ""
current_module = ""
stage = 0  # stage 0 = export IDL
           # stage 1 = compile IDL
           # stage 2 = ??


def makedirs(p):
    if not path.exists(p):
        os.makedirs(p)


def should_build(i, o):
    try:
        itime = os.stat(i)[stat.ST_MTIME]
        otime = os.stat(o)[stat.ST_MTIME]
        return itime > otime
    except OSError:
        return 1

    return 0


# called by module.py
def process_idl(list):

    XPIDL_INCLUDES = "-I F:/mozilla/dist/idl -I " + path.join(build_dir, "idl")

    for i in list:

        input = path.join(current_module, "idl", i)
        output_idl = path.join(build_dir, "idl", i)
        output_xpt = path.join(build_dir, "xpt", i)
        output_h   = path.join(build_dir, "include", i)

        # remove extension if we must
        if output_xpt[-4:] == ".idl":
            output_xpt = output_xpt[:-4]
        if output_h[-4:] == ".idl":
            output_h = output_h[:-4]

        if stage == 0:  # export IDL
            if should_build(input, output_idl):
                shutil.copyfile(input, output)

        if stage == 1:  # compile IDL
            if should_build(input, output_xpt):
                os.system("xpidl " + XPIDL_INCLUDES + \
                          " -m typelib -o " + output_xpt + " " + input);
                os.system("xpidl " + XPIDL_INCLUDES + \
                          " -m header -o " + output_h + " " + input);


def build_module(module):
    global current_module
    
    print "Building module '" + module +  "'"
    current_module = module
    execfile(path.join(module, "module.py"))


def main():

    global build_dir, stage

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
    stage = 0
    for i in MODULES:
        build_module(i)
    stage = 1
    for i in MODULES:
        build_module(i)


main()

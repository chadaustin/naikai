#!python

# this only works in Windows, and on my system
# we'll convert to scons eventually


import os
from os import path
import stat
import shutil


MODULES = [ "base", "nrun", "windowing" ]
build_dir = ""
current_module = ""
stage = 0  # stage 0 = export IDL
           # stage 1 = compile IDL
           # stage 2 = compilation stage


class BuildError:
    def __init__(self, value):
        self.value = value
    def __str__(self):
        return `self.value`


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
                print i
                shutil.copyfile(input, output_idl)

        if stage == 1:  # compile IDL
            if should_build(input, output_xpt + '.xpt'):
                command = "xpidl " + XPIDL_INCLUDES + \
                          " -m typelib -o " + output_xpt + " " + input
                if os.system(command):
                    raise BuildError
            if should_build(input, output_h + '.h'):
                command = "xpidl " + XPIDL_INCLUDES + \
                          " -m header -o " + output_h + " " + input
                if os.system(command):
                    raise BuildError


# called by module.py
def process_src(list):

    if stage != 2:  # compile C++
        return

    obj_dir = path.join(build_dir, "obj")
                        
    for i in list:

        INCLUDES = "/IF:/mozilla/dist/include "          + \
                   "/IF:/mozilla/dist/include/nspr "     + \
                   "/I" + path.join(build_dir, "include")

        DEFINES = "/DXP_WIN"

        # convert .cpp to .obj
        obj = i
        if obj[-4:] == ".cpp":
            obj = obj[:-4] + ".obj"

        # remove directories
        obj = path.basename(obj)

        output_obj = path.join(obj_dir, obj)
        input_cpp = path.join(current_module, "src", i)
        command = "CL /nologo /c /O /G5 /GA /GX " + INCLUDES   + \
                  " " + DEFINES                                + \
                  " /Fo" + output_obj                          + \
                  " " + input_cpp
        os.system(command)


def build_module(module):
    global current_module
    
    print "  Module '" + module +  "'"
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
    makedirs(path.join(build_dir, "obj"))

    # compile each module
    print "Stage: export"
    stage = 0
    for i in MODULES:
        build_module(i)

    print "Stage: IDL"
    stage = 1
    for i in MODULES:
        build_module(i)

    print "Stage: Compilation"
    stage = 2
    for i in MODULES:
        build_module(i)

    print "Hardcoded Link Stage"
    stage = 3

    # link nrun
    os.system("link /nologo /out:" + path.join(build_dir, "bin/nrun.exe") + \
              " /release /subsystem:console "                             + \
              path.join(build_dir, "obj", "nrun.obj") + " xpcom.lib")

    # link windowing
    os.system("link /nologo /out:" + path.join(build_dir, "bin/windowing.dll") + \
              " /release /dll /subsystem:windows "                             + \
              path.join(build_dir, "obj", "nkWindow.obj") + " "                + \
              path.join(build_dir, "obj", "nkWindowingModule.obj") + " "       + \
              path.join(build_dir, "obj", "nkWindowingSystem.obj") + " "       + \
              "user32.lib xpcom.lib")
              

main()

import json
import os
from hashlib import sha1
from os import walk
from os.path import (basename, dirname, expanduser, isdir, isfile, join,
                     realpath, splitdrive, exists) 
from click.testing import CliRunner
from platformio import __version__, exception
from platformio.compat import WINDOWS, hashlib_encode_data
from platformio.project.config import ProjectConfig


Import("env")

PROJECTPLATFORM = "nodemcuv2"

def get_project_optional_dir(name, default=None):
    project_dir = get_project_dir()
    print ("project_dir: " + project_dir)
    config = ProjectConfig.get_instance(join(project_dir, "platformio.ini"))
    # print ("config: " + config)
    optional_dir = config.get("platformio", name)
    print ("optional_dir: " + optional_dir)

    if not optional_dir:
        return default

    if "$PROJECT_HASH" in optional_dir:
        optional_dir = optional_dir.replace(
            "$PROJECT_HASH", "%s-%s" %
            (basename(project_dir), sha1(
                hashlib_encode_data(project_dir)).hexdigest()[:10]))

    if optional_dir.startswith("~"):
        optional_dir = expanduser(optional_dir)

    print ((optional_dir))
    return (optional_dir)

def get_project_workspace_dir():
    return get_project_optional_dir("workspace_dir",
                                    join(get_project_dir(), ".pio"))

def get_project_libdeps_dir():
    print (get_project_optional_dir("libdeps_dir", join(get_project_workspace_dir(), "libdeps")))
    return get_project_optional_dir(
        "libdeps_dir", join(get_project_workspace_dir(), "libdeps"))

def get_project_dir():
    return os.getcwd()

BUILDSCRIPT = env.PioPlatform().get_build_script()
DIR = env.PioPlatform().get_dir()
ISEMBEDDED = env.PioPlatform().is_embedded()
LIBSTORAGES = env.PioPlatform().get_lib_storages()
PROJECTDIR = os.getcwd()
PROJECTLIBDEPS = join(get_project_libdeps_dir(), PROJECTPLATFORM)
PROJECTLIBDEPSPUBSUBCLIENT = join(PROJECTLIBDEPS, "PubSubClient_ID89")
PROJECTLIBDEPSFUAXMOESP = join(PROJECTLIBDEPS, "FauxmoESP_ID1303")

PROJECTLIBDEPSPUBSUBCLIENTPATCHINGDONE = join(PROJECTLIBDEPSPUBSUBCLIENT, "patching-done")
PROJECTLIBDEPSFUAXMOESPPATCHINGDONE = join(PROJECTLIBDEPSFUAXMOESP, "patching-done")

print (PROJECTLIBDEPSPUBSUBCLIENTPATCHINGDONE)
print (PROJECTLIBDEPSFUAXMOESPPATCHINGDONE)

if ((env.Execute("ls %s" % PROJECTLIBDEPSPUBSUBCLIENTPATCHINGDONE) == 0) and (env.Execute("ls %s" % PROJECTLIBDEPSFUAXMOESPPATCHINGDONE) == 0)):
    print ("Patches previously applied")
    env.Exit(0)
else:
    print ("Patches not applied")


original_file = join(PROJECTLIBDEPSPUBSUBCLIENT,"src","PubSubClient.h")
patched_file = join(PROJECTDIR,"patch","PubSubClient.h.patch")
print (original_file)
print (patched_file)
#assert isfile(original_file) and isfile(patched_file)
env.Execute("patch %s %s" % (original_file, patched_file))

original_file = join(PROJECTLIBDEPSPUBSUBCLIENT,"src","PubSubClient.cpp")
patched_file = join(PROJECTDIR,"patch","PubSubClient.cpp.patch")
print (original_file)
print (patched_file)
#assert isfile(original_file) and isfile(patched_file)
env.Execute("patch %s %s" % (original_file, patched_file))

original_file = join(PROJECTLIBDEPSFUAXMOESP,"src","fauxmoESP.h")
patched_file = join(PROJECTDIR,"patch","fauxmoESP.h.patch")
print (original_file)
print (patched_file)
#assert isfile(original_file) and isfile(patched_file)
env.Execute("patch %s %s" % (original_file, patched_file))

original_file = join(PROJECTLIBDEPSFUAXMOESP,"src","fauxmoESP.cpp")
patched_file = join(PROJECTDIR,"patch","fauxmoESP.cpp.patch")
print (original_file)
print (patched_file)
#assert isfile(original_file) and isfile(patched_file)
env.Execute("patch %s %s" % (original_file, patched_file))

def _touch(path):
    with open(path, "w") as fp:
        fp.write("")

env.Execute("touch %s" % (PROJECTLIBDEPSPUBSUBCLIENTPATCHINGDONE))
env.Execute("touch %s" % (PROJECTLIBDEPSFUAXMOESPPATCHINGDONE))
#env.Execute(lambda *args, **kwargs: _touch(PROJECTLIBDEPSPUBSUBCLIENTPATCHINGDONE))
#env.Execute(lambda *args, **kwargs: _touch(PROJECTLIBDEPSFUAXMOESPPATCHINGDONE))

print ("Patches: --- --- --- --- --- ---")

print (BUILDSCRIPT)
print (DIR)
print (ISEMBEDDED)
print (LIBSTORAGES)
print (PROJECTDIR)
print (os.environ)
print (PROJECTLIBDEPS)
print (PROJECTLIBDEPSPUBSUBCLIENTPATCHINGDONE)
print (PROJECTLIBDEPSFUAXMOESPPATCHINGDONE)
print (original_file)
print (patched_file)
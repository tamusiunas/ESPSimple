from os.path import join, isfile

Import ("env")
Import("projenv")

def _touch(path):
        with open(path, "w") as fp:
            fp.write("")

def applyPatch(originalFile, patchFile):
    print("originalFile: " + originalFile)
    print("patchFile: " + patchFile)
    env.Execute("bash -c \"patches/patch.bash '%s %s'\"" % (originalFile, patchFile))

libDepsDir = projenv["PROJECT_LIBDEPS_DIR"]
pioName = projenv ["PIOENV"]
patchDir = join(projenv ["PROJECT_DIR"],"patches")
libDepsPath = join(libDepsDir,pioName)
patchDoneFile = join(libDepsPath, ".patching-done")
hostOs = projenv ["HOST_OS"]
print("libDepsPath: " + libDepsPath)
print("patchDoneFile: " + patchDoneFile)

# print(env.Dump())
# print(projenv.Dump())

if not isfile(patchDoneFile):
    originalFile = join(libDepsPath, "PubSubClient", "src", "PubSubClient.h")
    patchFile = join(patchDir, "PubSubClient.h.patch")
    applyPatch(originalFile,patchFile)
    originalFile = join(libDepsPath, "PubSubClient", "src", "PubSubClient.cpp")
    patchFile = join(patchDir, "PubSubClient.cpp.patch")
    applyPatch(originalFile,patchFile)

    originalFile = join(libDepsPath, "FauxmoESP", "src", "fauxmoESP.h")
    patchFile = join(patchDir, "fauxmoESP.h.patch")
    applyPatch(originalFile,patchFile)
    originalFile = join(libDepsPath, "FauxmoESP", "src", "fauxmoESP.cpp")
    patchFile = join(patchDir, "fauxmoESP.cpp.patch")
    applyPatch(originalFile,patchFile)

    env.Execute(lambda *args, **kwargs: _touch(patchDoneFile))
else:
    print("Files were already patched")
    

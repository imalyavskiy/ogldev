# -*- coding: utf-8 -*-
#TODO copy socket.io necessary headers
#TODO create gn command
#TODO create ninja command
#TODO create fetch command (fetch.bat of depot_tools)
#TODO create gclient command(gclient.bat of depot tools)
#TODO get rid of cmd command - move it underground(as command.run())
#TODO make dry run
#TODO check if all the variables below are initialized before start doing anything
#TODO pass install target dir by argv
#TODO check all the pathes required before start
#TODO write the application long and the logs of instruments(commands) to the bit log file

log_file_name   = "set_up"

###########################################
# used as a folder that contains all the dependencies and participates in construction of the CWD for certain dependency - "install_dir+dependency_name+/"
install_dir     = ""         
# MS Visual Studio install dir
vstudio_dir     = "" 
# MS Visual Studio build evironment batches dir
vcvarsall_dir   = ""
# MS Visual Studio build tools dir
msbuild_dir      = ""
# CMake install dir
cmake_dir       = ""
##########################################
vstudio_ver     = ""
##########################################

import os
import command
import json
import sys
import datetime

subdir_free_glut    = "FreeGlut/"
subdir_glew         = "glew/"

#get paths for build tool
try:
    with open("build_config.json", 'r') as f:
        conf = json.loads(f.read())
    install_dir = conf["install_dir"]                               # used as a folder that contains all the dependencies
                                                                    # and participates in construction of the CWD for 
                                                                    # certain dependency - "install_dir+dependency_name+/"
    assert install_dir!="", "Set install_dir"
    if install_dir[-1] != '/': install_dir += '/'
    vstudio_dir = conf["vstudio_dir"]                               # MS Visual Studio install dir
    assert vstudio_dir!="", "Set vstudio_dir"
    if vstudio_dir[-1] != '/': vstudio_dir += '/'
    vcvarsall_dir   = vstudio_dir+"VC/Auxiliary/Build/"             # MS Visual Studio build evironment batches dir
    msbuild_dir      = vstudio_dir+"MSBuild/15.0/Bin/amd64/"        # MS Visual Studio build tools dir
    cmake_dir = conf["cmake_dir"]                                   # CMake install dir
    assert cmake_dir!="", "Set cmake_dir"
    if cmake_dir[-1] != '/': cmake_dir += '/'
    vstudio_ver = conf["vstudio_ver"]                               # MS Visual Studio version
    assert vstudio_ver!="", "Set vstudio_ver"
except FileNotFoundError as e:
    print(e)
    print("Please create this file using example_build_config.json in project folder")
    exit()
except Exception as e:
    print("Configuring process error!")
    print(e)
    exit()

# The list of active stages for certain dependencies
activities = \
{
    subdir_free_glut[:-1] : { 
          "build_Win64" : True,
          "build_Win32" : True,
          "copy_headers": True,
          "clean"       : True,
    },
    subdir_glew[:-1]: {
          "build_Win64" : True,
          "build_Win32" : True,
          "copy_headers": True,
          "clean"       : True,
    },
}

# The list of dependencies
# the structure is the follows:
#     [                                             # list of dependencies each dependency is a dictionary
#       { "name"            : <dependency_name>     # string
#         "stages"          : [                     # list of stages
#           { "name"        : <stage_name>,         # string
#             "steps"       : [                     # list of steps
#               { "command" : <command_function>,   # string
#                 "args"    : [                     # list of certain command arguments
#                 <arg_0>, ..., <arg_N>             # arguments are command function dependent
#                 ]
#               }
#             ]
#           }
#         ]
#       } 
#     ]

# the set of rules requred to set up all the dependencies
dependencies =\
[
    # freeglut
    { "name"          : subdir_free_glut[:-1],
      "stages"        : [
            # clean
            { "name"  : "clean",
              "active": activities[subdir_free_glut[:-1]]["clean"],
              "steps" :
                [
                    # cleaning up the local repository
                    { "command" : command.git,
                      "args"    : ["clean", "-fx", "-d"]},
                ]
            },
            # build Win64 artifacts(binaries and static libraries)
            { "name"  : "build_Win64",
              "active": activities[subdir_free_glut[:-1]]["build_Win64"],
              "steps" :
                [
                    # read environment variables
                    { "command" : command.read_env_vars,
                      "args"    : [ "\""+vcvarsall_dir+"vcvarsall.bat\"", "x64"]},
                    # changing the --variable environment variable in the way --action with value --value
                    { "command" : command.update_environment_variable,
                      "args"    : ["--variable=Path", "--action=append", "--value=\"C:/Program Files/CMake/bin/\""]},
                    # generating project files with CMake
                    { "command" : command.cmake,
                      "args"    : [ "-G \"Visual Studio 15 2017 Win64\""]},
                    # changing the --variable environment variable in the way --action with value --value
                    { "command" : command.update_environment_variable,
                      "args"    : ["--variable=Path", "--action=append", "--value="+msbuild_dir]},
                    # building certain project with msbuild and certain parameters
                    { "command" : command.msbuild,
                      "args"    : [ "freeglut.vcxproj"
                                  , "/t:Rebuild"
                                  , "/p:Configuration=Debug"
                                  , "/p:Platform=x64"
                                  ]},
                    # building certain project with msbuild and certain parameters
                    { "command" : command.msbuild,
                      "args"    : [ "freeglut.vcxproj"
                                  , "/t:Rebuild"
                                  , "/p:Configuration=Release"
                                  , "/p:Platform=x64"
                                  ]},
                    # building certain project with msbuild and certain parameters
                    { "command" : command.msbuild,
                      "args"    : [ "freeglut_static.vcxproj"
                                  , "/t:Rebuild"
                                  , "/p:Configuration=Debug"
                                  , "/p:Platform=x64"
                                  ]},
                    # building certain project with msbuild and certain parameters
                    { "command" : command.msbuild,
                      "args"    : [ "freeglut_static.vcxproj"
                                  , "/t:Rebuild"
                                  , "/p:Configuration=Release"
                                  , "/p:Platform=x64"
                                  ]},
                    # copy build artifacts
                    { "command" : command.copy,
                      "args"    : [ "--src=\"./bin/Debug\"", "--dst=\"./../../exports/freeglut/lib/Win64/Debug\"", "--filter=\"*.dll;*.ilk;*.pdb\""]},
                    { "command" : command.copy,
                      "args"    : [ "--src=\"./lib/Debug\"", "--dst=\"./../../exports/freeglut/lib/Win64/Debug\"", "--filter=\"*.lib;*.exp\""]},
                    { "command" : command.copy,
                      "args"    : [ "--src=\"./bin/Release\"", "--dst=\"./../../exports/freeglut/lib/Win64/Release\"", "--filter=\"*.dll;*.ilk;*.pdb\""]},
                    { "command" : command.copy,
                      "args"    : [ "--src=\"./lib/Release\"", "--dst=\"./../../exports/freeglut/lib/Win64/Release\"", "--filter=\"*.lib;*.exp\""]},
                ]
            },
            # clean
            { "name"  : "clean",
              "active": activities[subdir_free_glut[:-1]]["clean"],
              "steps" :
                [
                    # cleaning up the local repository
                    { "command" : command.git,
                      "args"    : ["clean", "-fx", "-d"]},
                ]
            },
            # build Win32 artifacts(binaries and static libraries)
            { "name"  : "build_Win32",
              "active": activities[subdir_free_glut[:-1]]["build_Win32"],
              "steps" :
                [
                    # read environment variables
                    { "command" : command.read_env_vars,
                      "args"    : [ "\""+vcvarsall_dir+"vcvarsall.bat\"", "x86"]},
                    # changing the --variable environment variable in the way --action with value --value
                    { "command" : command.update_environment_variable,
                      "args"    : ["--variable=Path", "--action=append", "--value=\"C:/Program Files/CMake/bin/\""]},
                    # generating project files with CMake
                    { "command" : command.cmake,
                      "args"    : [ "-G \"Visual Studio 15 2017\""]},
                    # changing the --variable environment variable in the way --action with value --value
                    { "command" : command.update_environment_variable,
                      "args"    : ["--variable=Path", "--action=append", "--value="+msbuild_dir]},
                    # building certain project with msbuild and certain parameters
                    { "command" : command.msbuild,
                      "args"    : [ "freeglut.vcxproj"
                                  , "/t:Rebuild"
                                  , "/p:Configuration=Debug"
                                  , "/p:Platform=x86"
                                  ]},
                    # building certain project with msbuild and certain parameters
                    { "command" : command.msbuild,
                      "args"    : [ "freeglut.vcxproj"
                                  , "/t:Rebuild"
                                  , "/p:Configuration=Release"
                                  , "/p:Platform=x86"
                                  ]},
                    # building certain project with msbuild and certain parameters
                    { "command" : command.msbuild,
                      "args"    : [ "freeglut_static.vcxproj"
                                  , "/t:Rebuild"
                                  , "/p:Configuration=Debug"
                                  , "/p:Platform=x86"
                                  ]},
                    # building certain project with msbuild and certain parameters
                    { "command" : command.msbuild,
                      "args"    : [ "freeglut_static.vcxproj"
                                  , "/t:Rebuild"
                                  , "/p:Configuration=Release"
                                  , "/p:Platform=x86"
                                  ]},
                    # copy build artifacts
                    { "command" : command.copy,
                      "args"    : [ "--src=\"./bin/Debug\"", "--dst=\"./../../exports/freeglut/lib/Win32/Debug\"", "--filter=\"*.dll;*.ilk;*.pdb\""]},
                    { "command" : command.copy,
                      "args"    : [ "--src=\"./lib/Debug\"", "--dst=\"./../../exports/freeglut/lib/Win32/Debug\"", "--filter=\"*.lib;*.exp\""]},
                    { "command" : command.copy,
                      "args"    : [ "--src=\"./bin/Release\"", "--dst=\"./../../exports/freeglut/lib/Win32/Release\"", "--filter=\"*.dll;*.ilk;*.pdb\""]},
                    { "command" : command.copy,
                      "args"    : [ "--src=\"./lib/Release\"", "--dst=\"./../../exports/freeglut/lib/Win32/Release\"", "--filter=\"*.lib;*.exp\""]},
                ]
            },
            # clean
            { "name"  : "clean",
              "active": activities[subdir_free_glut[:-1]]["clean"],
              "steps" :
                [
                    # cleaning up the local repository
                    { "command" : command.git,
                      "args"    : ["clean", "-fx", "-d"]},
                ]
            },
            # copy headers
            { "name"  : "copy_headers",
              "active": activities[subdir_free_glut[:-1]]["copy_headers"],
              "steps" :
                [
                    { "command" : command.copy,
                      "args"    : [ "--src=\"./include\"", "--dst=\"./../../exports/freeglut/inc/\"", "--filter=\"*.h;*.hpp\""]},
                ]
            },
        ]
    },
    #glue
    { "name"          : subdir_glew[:-1],
      "stages"        : [
            # clean
            { "name"  : "clean",
              "active": activities[subdir_glew[:-1]]["clean"],
              "steps" :
                [
                    # cleaning up the local repository
                    { "command" : command.git,
                      "args"    : ["clean", "-fx", "-d"]},
                ]
            },
            # build Win64 artifacts(binaries and static libraries)
            { "name"  : "build_Win64",
              "active": activities[subdir_glew[:-1]]["build_Win64"],
              "steps" :
                [
                    # read environment variables
                    { "command" : command.read_env_vars,
                      "args"    : [ "\""+vcvarsall_dir+"vcvarsall.bat\"", "x64"]},
                    # changing the --variable environment variable in the way --action with value --value
                    { "command" : command.update_environment_variable,
                      "args"    : ["--variable=Path", "--action=append", "--value="+msbuild_dir]},
                    # building certain project with msbuild and certain parameters
                    { "command" : command.msbuild,
                      "args"    : [ "./build/vc15/glew_shared.vcxproj"
                                  , "/t:Rebuild"
                                  , "/p:Configuration=Debug"
                                  , "/p:Platform=x64"
                                  ]},
                    # building certain project with msbuild and certain parameters
                    { "command" : command.msbuild,
                      "args"    : [ "./build/vc15/glew_shared.vcxproj"
                                  , "/t:Rebuild"
                                  , "/p:Configuration=Release"
                                  , "/p:Platform=x64"
                                  ]},
                    # building certain project with msbuild and certain parameters
                    { "command" : command.msbuild,
                      "args"    : [ "./build/vc15/glew_static.vcxproj"
                                  , "/t:Rebuild"
                                  , "/p:Configuration=Debug"
                                  , "/p:Platform=x64"
                                  ]},
                    # building certain project with msbuild and certain parameters
                    { "command" : command.msbuild,
                      "args"    : [ "./build/vc15/glew_static.vcxproj"
                                  , "/t:Rebuild"
                                  , "/p:Configuration=Release"
                                  , "/p:Platform=x64"
                                  ]},
                    # copy build artifacts
                    { "command" : command.copy,
                      "args"    : [ "--src=\"./bin/Debug/x64\"", "--dst=\"./../../exports/glew/lib/Win64/Debug\"", "--filter=\"*.dll;*.ilk;*.pdb\""]},
                    { "command" : command.copy,
                      "args"    : [ "--src=\"./lib/Debug/x64\"", "--dst=\"./../../exports/glew/lib/Win64/Debug\"", "--filter=\"*.lib;*.exp\""]},
                    { "command" : command.copy,
                      "args"    : [ "--src=\"./bin/Release/x64\"", "--dst=\"./../../exports/glew/lib/Win64/Release\"", "--filter=\"*.dll;*.ilk;*.pdb\""]},
                    { "command" : command.copy,
                      "args"    : [ "--src=\"./lib/Release/x64\"", "--dst=\"./../../exports/glew/lib/Win64/Release\"", "--filter=\"*.lib;*.exp\""]},
                ]
            },
            # clean
            { "name"  : "clean",
              "active": activities[subdir_glew[:-1]]["clean"],
              "steps" :
                [
                    # cleaning up the local repository
                    { "command" : command.git,
                      "args"    : ["clean", "-fx", "-d"]},
                ]
            },
            # build Win32 artifacts(binaries and static libraries)
            { "name"  : "build_Win32",
              "active": activities[subdir_glew[:-1]]["build_Win32"],
              "steps" :
                [
                    # read environment variables
                    { "command" : command.read_env_vars,
                      "args"    : [ "\""+vcvarsall_dir+"vcvarsall.bat\"", "x86"]},
                    # changing the --variable environment variable in the way --action with value --value
                    { "command" : command.update_environment_variable,
                      "args"    : ["--variable=Path", "--action=append", "--value=\"C:/Program Files/CMake/bin/\""]},
                    # building certain project with msbuild and certain parameters
                    { "command" : command.msbuild,
                      "args"    : [ "./build/vc15/glew_shared.vcxproj"
                                  , "/t:Rebuild"
                                  , "/p:Configuration=Debug"
                                  , "/p:Platform=x86"
                                  ]},
                    # building certain project with msbuild and certain parameters
                    { "command" : command.msbuild,
                      "args"    : [ "./build/vc15/glew_shared.vcxproj"
                                  , "/t:Rebuild"
                                  , "/p:Configuration=Release"
                                  , "/p:Platform=x86"
                                  ]},
                    # building certain project with msbuild and certain parameters
                    { "command" : command.msbuild,
                      "args"    : [ "./build/vc15/glew_static.vcxproj"
                                  , "/t:Rebuild"
                                  , "/p:Configuration=Debug"
                                  , "/p:Platform=x86"
                                  ]},
                    # building certain project with msbuild and certain parameters
                    { "command" : command.msbuild,
                      "args"    : [ "./build/vc15/glew_static.vcxproj"
                                  , "/t:Rebuild"
                                  , "/p:Configuration=Release"
                                  , "/p:;Platform=x86"
                                  ]},
                    # copy build artifacts
                    { "command" : command.copy,
                      "args"    : [ "--src=\"./bin/Debug/Win32\"", "--dst=\"./../../exports/glew/lib/Win32/Debug\"", "--filter=\"*.dll;*.ilk;*.pdb\""]},
                    { "command" : command.copy,
                      "args"    : [ "--src=\"./lib/Debug/Win32\"", "--dst=\"./../../exports/glew/lib/Win32/Debug\"", "--filter=\"*.lib;*.exp\""]},
                    { "command" : command.copy,
                      "args"    : [ "--src=\"./bin/Release/Win32\"", "--dst=\"./../../exports/glew/lib/Win32/Release\"", "--filter=\"*.dll;*.ilk;*.pdb\""]},
                    { "command" : command.copy,
                      "args"    : [ "--src=\"./lib/Release/Win32\"", "--dst=\"./../../exports/glew/lib/Win32/Release\"", "--filter=\"*.lib;*.exp\""]},
                ]
            },
            # clean
            { "name"  : "clean",
              "active": activities[subdir_glew[:-1]]["clean"],
              "steps" :
                [
                    # cleaning up the local repository
                    { "command" : command.git,
                      "args"    : ["clean", "-fx", "-d"]},
                ]
            },
            # copy headers
            { "name"  : "copy_headers",
              "active": activities[subdir_glew[:-1]]["copy_headers"],
              "steps" :
                [
                    { "command" : command.copy,
                      "args"    : [ "--src=\"./include\"", "--dst=\"./../../exports/glew/inc/\"", "--filter=\"*.h;*.hpp\""]},
                ]
            },
        ]
    },
]


class Logger(object):
    """logger class"""
    def __init__(self, log_file_dir = "", log_file_name = ""):
        self.log_file_name = log_file_name
        self.log_file_dir = log_file_dir
        self.log_file = None
        if len(self.log_file_dir) > 0 and len(self.log_file_name) > 0:
            now = datetime.datetime.now()
            # [path][name]_[day][month][year]\[[hour].[minute].[second]\].log
            self.log_file = open(
                 "{0}{1}_{2:02d}{3:02d}{4:04d}[{5:02d}h{6:02d}m{7:02d}s].log".format(self.log_file_dir, self.log_file_name, now.day, now.month, now.year, now.hour, now.minute, now.second), #filepath
                 "w",                                                                                                                                               #open mode
                 1                                                                                                                                                  #buffering policy - one line
                 )
        pass

    def __del__(self):
        self.log_file.close()
        pass

    def report(self, _header_text, _string_text, **kwargs):
        hide = False if kwargs.get("hide") is None or kwargs["hide"] == False else True

        if hide is False:
            print("{0} {1}".format(_header_text, _string_text))
        
        if self.log_file is not None:
            self.log_file.write("{0} {1}".format(_header_text, _string_text) + "\n" if not _string_text.endswith("\n") else "")

    def info(self, _string_text, **kwargs):
        self.report("[ INFO  ]", _string_text)
        pass

    def error(self, _string_text="", **kwargs):
        self.report("[FAILURE]", _string_text)
        pass

    def fatal(self, _string_text="", **kwargs):
        self.report("[FATAL]", _string_text)
        pass

    def success(self, _string_text="", **kwargs):
        self.report("[SUCCESS]", _string_text)
        pass
    
    def delimeter(self):
        self.report("", "--------------------------------------------------------------")


def check_required_paths(log):
    ''' Checks does requred paths are present in the file system '''
    if not os.path.isdir(vstudio_dir):
        log.fatal("The \""+vstudio_dir+"\" not exists")
        return False
    else:
        log.info("Presence of \""+vstudio_dir+"\"")
        log.success("Ok!")

    if not os.path.isdir(vcvarsall_dir):
        log.fatal("The \""+vcvarsall_dir+"\" not exists")
        return False
    else:
        log.info("Presence of \""+vcvarsall_dir+"\"")
        log.success("Ok!")

    if not os.path.isdir(msbuild_dir):
        log.fatal("The \""+msbuild_dir+"\" not exists")
        return False
    else:
        log.info("Presence of \""+msbuild_dir+"\"")
        log.success("Ok!")

    if not os.path.isdir(cmake_dir):
        log.fatal("The \""+cmake_dir+"\" not exists")
        return False
    else:
        log.info("Presence of \""+cmake_dir+"\"")
        log.success("Ok!")

    return True

def main():
    ''' This function runs through depenedencies/stages/steps and executes them '''
    log.info("Starting...")

    # Running through dependencies
    for cDep in range(0,len(dependencies)):
        if sorted(dependencies[cDep].keys()) != sorted(["name", "stages"]):
            log.error("Invalid dependency format")
            return False
        log.info("=== Processing dependency - \"{0}\"...".format(dependencies[cDep]["name"]))
        
        # Checking if the dependency directory exists and creating it if it does not 
        if not os.path.isdir(install_dir+"/"+dependencies[cDep]["name"]):
            os.makedirs(install_dir+"/"+dependencies[cDep]["name"])

        # Running through stages
        for cStage in range(0, len(dependencies[cDep]["stages"])):
            
            # Setting up the context. It is used to pass common information(e.g. environment variables) between steps.
            context = {"logger"         : log,
                       "install_dir"    : install_dir,
                       "dependency_dir" : install_dir+dependencies[cDep]["name"]+"/",
                       "cwd"            : "{0}/{1}".format(install_dir, dependencies[cDep]["name"])} 
            # Checking the validity of stage dict 
            if sorted(dependencies[cDep]["stages"][cStage].keys()) != sorted(["name", "active", "steps"]):
                log.error("Invalid stage format")
                return False

            # Logging out what stage we are processing
            log.info(">>> Processing stage - \"{0}\"...".format(dependencies[cDep]["stages"][cStage]["name"]))
            if dependencies[cDep]["stages"][cStage]["active"] is False:
                log.info("stage is inactive")
                continue

            # Running through steps
            for cStep in range(0, len(dependencies[cDep]["stages"][cStage]["steps"])):
                log.info("step[{0}/{1}]".format(cStep + 1, len(dependencies[cDep]["stages"][cStage]["steps"])))
                # Checking the validity of stage dict
                if sorted(dependencies[cDep]["stages"][cStage]["steps"][cStep].keys()) != sorted(["command", "args"]):
                    log.error("Invalid command format")
                    return False
                
                # Executing step by calling to its command with given arguments
                result = dependencies[cDep]["stages"][cStage]["steps"][cStep]["command"](
                    context,                                                        # context
                    dependencies[cDep]["stages"][cStage]["steps"][cStep]["args"])   # arguments

            #cleaning up the context
            context.clear() 
    return True

def create_install_dir():
    ''' Creates the dependecies install dir '''
    if not os.path.isdir(install_dir):
        print("The \"{0}\" does not exist...".format(install_dir))
        try:
            print("Creating directory \"{0}\"...".format(install_dir))
            os.makedirs(install_dir)
        except OSError:
            print("Oops! Cannot create \"{0}\"...".format(install_dir))
            print("\tcheck the path is accessible or may be there is unsufficient access rights")
            print("..! TERMINATING !..")
            exit()
    else:
        print("The install dir that is \"{0}\" already exist. Creating logger".format(install_dir))

if __name__ == "__main__":
    
    #praparations
    create_install_dir()

    # creating logger
    log = Logger(install_dir, log_file_name)
    
    # printing log delimeeter
    log.delimeter()

    if not check_required_paths(log):
        # not all paths exist
        exit()

    # printing log delimeeter
    log.delimeter()

    # logging switches 
    log.info("=== SWITCHES")
    activities_text = json.dumps(activities, sort_keys=False, indent=4).split("\n")
    for line in activities_text:
        log.info(line)
    
    # printing log delimeeter
    log.delimeter()

    # execution
    if not main():
        log.error()
        # something went wrong
        exit()
    
    log.success()
    
    # printing log delimeeter
    log.delimeter()

    #done
    exit()

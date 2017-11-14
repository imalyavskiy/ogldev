# -*- coding: utf-8 -*-

import subprocess
import os
import shutil
import re
import winreg
import sys
import time

class ProgressBar(object):
    """progress bar class"""
    def __init__(self, width=10):
        self.width = width
        self.pos = 0
        self.eraser = str()
        for pos in range(0, self.width + 2):
            self.eraser += "\b"

    def cleanup(self):
        pass
        print(self.eraser, end="", flush=True)

    def display(self):
        bar = "["
        for _pos in range(0, self.width):
            if _pos == self.pos:
               bar += "*"
            else:
               bar += "-"
        else:
            bar += "]"

        if self.pos + 1 < self.width:
           self.pos = self.pos + 1
        else:
           self.pos = 0

        self.cleanup()

        print(bar, end="", flush=True)

def run(log, target, args=[], **kwargs):
    result = str()
    pipe = subprocess.PIPE
    
    cmd_str = target
    for arg in args:
        cmd_str += " " + arg

    cwd_str = os.getcwd()
    cwd = kwargs.get("cwd")
    if cwd is not None:
        cwd_str = cwd
    
    env = kwargs["env"] if "env" in kwargs and kwargs["env"] is not None else os.environ

    log.info("Calling to \"{0}\"".format(cmd_str))
    log.info("        at \"{0}\"".format(cwd_str))

    proc = subprocess.Popen( cmd_str,
                                shell  = True,
                                stdin  = pipe,
                                stdout = pipe,
                                stderr = subprocess.STDOUT,
                                cwd    = cwd_str,
                                env    = env )

    bar     = ProgressBar()

    loop = True
    while loop:
        bytes = proc.stdout.readline()
        line = bytes.decode('cp1251')
        result += line

        ret_val = proc.poll()
        if len(line) == 0: 
            if ret_val is None:
                bar.display()
                time.sleep(1.000)
                continue
            else:
                loop = False

        log.report("\t", line, hide=True)
    else:
        bar.cleanup()

    log.info("Done(return code {0}).".format(ret_val))

    return True, result

def git(context, args, **kwargs):
    '''
    calls to git.exe
    '''
    log = context.get("logger")

    return run(log, "git", args, env = None if context.get("environment") is None else context["environment"], cwd=context["cwd"])[0]

def b2(context, args, **kwargs):
    '''
    calls to b2.exe
    '''
    log = context.get("logger")
    
    if not os.path.isfile(context["cwd"]+"/b2.exe"):
        return False, ""

    return run(log, "b2", args, env = None if context.get("environment") is None else context["environment"], cwd=context["cwd"])[0]

def bootstrap(context, args, **kwargs):
    '''
    calls to bootstrap.bat
    '''
    log = context.get("logger")

    if not os.path.isfile(context["cwd"]+"/bootstrap.bat"):
        return False, ""

    return run(log, "bootstrap.bat", args, env = None if context.get("environment") is None else context["environment"], cwd=context["cwd"])[0]

def copy(context, args, **kwargs):
    '''
    copies all files that satisfy the given filter from src to dst
    cals to move() with keep==True
    '''    
    kwargs["keep"]=True
    return move(context, args, **kwargs)

def move(context, args, **kwargs):
    '''
    moves all files that satisfy the given filter
    from src to dst
    if keep==True that files are kept in the src i.e. copy operation performed instead move
    '''
    result = str()
    keep = kwargs["keep"] if kwargs.get("keep") is not None else False

    log = context.get("logger")

    if kwargs.get("report") is None or kwargs["report"] is True: 
        log.info("... Coping files ...")

    # checking and parsing parameters
    parameters = parse_args(log, args)
    if parameters is None:
        return False
    
    keys = parameters.keys()
    if not ("src" in keys and "dst" in keys):
        return False

    # stripping quoted string    
    if parameters["src"][0] == "\"" and parameters["src"][-1] == "\"":
        parameters["src"] = parameters["src"][1:-1]
        parameters["src"] += "/" if not (parameters["src"].endswith("/") or parameters["src"].endswith("\\")) else ""
    if parameters["dst"][0] == "\"" and parameters["dst"][-1] == "\"":
        parameters["dst"] = parameters["dst"][1:-1]
        parameters["dst"] += "/" if not (parameters["dst"].endswith("/") or parameters["dst"].endswith("\\")) else ""
    if parameters.get("filter") is not None:
        if parameters["filter"][0] == "\"" and parameters["filter"][-1] == "\"":
            parameters["filter"] = parameters["filter"][1:-1]
    
    filters =[]
    if parameters.get("filter") is not None and len(parameters["filter"])>0:
        filters = parameters["filter"].split(";")
    
    for cFilter in range(0, len(filters)):
        filters[cFilter] = filters[cFilter].replace("*","^[A-Za-z0-9_\-]*")
        filters[cFilter]+="$"

    source_dir = context["dependency_dir"]+parameters["src"]
    target_dir = context["dependency_dir"]+parameters["dst"]

    if not os.path.isdir(target_dir):
        os.makedirs(target_dir)

    dir_items = os.listdir(source_dir)
    for dir_item in dir_items:
        if os.path.isfile(source_dir+dir_item):
            if len(filters) > 0:
                item_copied = False
                for filter in filters:
                    res = re.match(filter, dir_item)
                    if res is not None:
                        log.report("\t"," ... {0} {1} from {2} to {3} ... ".format("Moving" if keep is False else "Coping", dir_item, source_dir, target_dir), hide=True)
                        shutil.move(source_dir+dir_item, target_dir+dir_item) if keep is False \
                            else shutil.copy(source_dir+dir_item, target_dir+dir_item)
                        item_copied = True
                        break
                if item_copied is False:
                    log.report("\t"," ... {0} {1} ... ".format("Filtering out", dir_item), hide=True)
            else:
                log.report("\t"," ... {0} {1} from {2} to {3} ... ".format("Moving" if keep is False else "Coping", dir_item, source_dir, target_dir), hide=True)
                shutil.move(source_dir+dir_item, target_dir+dir_item) if keep is False \
                    else shutil.copy(source_dir+dir_item, target_dir+dir_item)
                pass
            pass
        elif os.path.isdir(source_dir+dir_item):
            newparameters = parameters.copy()
            newparameters["src"]+=dir_item+"/"
            newparameters["dst"]+=dir_item+"/"
            newargs = []
            for param in newparameters:
                newargs.append("--{0}={1}".format(param, "\""+newparameters[param]+"\""))
            kwargs["report"] = False
            res_bool = move(context, newargs, **kwargs)
            pass
    
    if not keep:
        shutil.rmtree(source_dir, ignore_errors=True, onerror=print("Error: Cannot remove source directory"))

    return True

def read_env_vars(context, args, **kwargs):
    '''
    runs the provided batch script 
    reads all the environment variables 
    stores them in the stage's context
    '''
    log = context.get("logger")

    log.info("... Reading environment variables ...")

    if args is None or len(args) == 0:
        context["environment"] = os.environ.copy()
        return True
    else:
        arguments = "{0}".format(args[0])
        for arg in range(1, len(args)):
            arguments+=" {0}".format(args[arg])

    # if context is not dict
    if not isinstance(context, (dict)):
        return False

    if context.get("environment") is not None:
        return False

    initial = None if kwargs.get("initial") is None else kwargs["initial"]

    # create a tag so we can tell in the output when the proc is done
    tag = "__DONE_RUNNING_COMMAND_B8E5CB44_F797_447D_9901_EDA32D03BFEE__"

    # construct a cmd.exe command to do accomplish this
    cmd = '/s /c "{arguments} && echo "{tag}" && set"'.format(**vars())

    # launch the process
    result = run(log, "cmd.exe", [cmd], environment_variables=initial)
    if result[0] is False:
        return False

    # parse the output sent to stdout
    lines = result[1].split("\r\n")
    
    environment_variables={}
    #remove all lines prior tag inclusive
    loop = True
    while loop:
        if tag in lines[0]:
            loop = False
        lines.remove(lines[0])
    
    for line in lines:
        pair = line.split("=")
        if len(pair) != 2:
            continue
        environment_variables[pair[0]]=pair[1]

    if context.get("environment") is None:
        context["environment"] = environment_variables
    
    return True

def perl(context, args, **kwargs):
    '''
    calls to perl.exe
    '''
    log = context.get("logger")
    
    return run(log, "perl", args, env = None if context.get("environment") is None else context["environment"], cwd=context["cwd"])[0]

def cmd(context, args, **kwargs):
    '''
    calls to cmd.exe
    '''
    log = context.get("logger")

    for arg in range(0, len(args)):
        if args[arg].endswith(".bat") and not os.path.isabs(args[arg]):
            args[arg] = context["dependency_dir"]+args[arg]
    
    return run(log, "cmd", args, env = None if context.get("environment") is None else context["environment"], cwd=context["cwd"])[0]

def parse_args(log, args):
    
    def strip_arg(arg):
        arg = arg[2:]
        res = arg.split("=")
        return res[0], res[1]

    parameters = {}
    for arg in args:
        match = re.match("^--filter=", arg)
        if match is not None:
            key, val = strip_arg(arg)
            parameters[key] = val
            continue
        match = re.match("^--[A-Za-z_0-9\-]*=[A-Za-z_0-9/#\"\-\:\s\.\(\)\$\{\}]*$", arg)
        if match is not None and match.group() == arg:
            key, val = strip_arg(arg)
            parameters[key] = val
        else:
            log.report("\t", "[update_environment_variable] The \"{0}\" parameter is invalid".format(arg)+"\n", hide=True)
            return None
    return parameters

def update_environment_variable(context, args, **kwargs):
    '''
    looks stage's context for a viariable given with --variable=<variable>
    takes the value given with the --value=<value>
    executes the action given by --action=<action> with the value
    '''
    # strinpping logger
    log = context.get("logger")

    # reporting the command short description
    log.info("... Updating environment variables ...")

    # checking context exists
    if context.get("environment") is None:
        return False

    # checking and parsing parameters
    parameters = parse_args(log, args)
    if parameters is None:
        return False
    if sorted(parameters.keys()) != sorted(["variable", "action", "value"]):
        result+="[update_environment_variable] invalid parameters set"+"\n"
        return False
    # stripping quoted string    
    if parameters["value"][0] == "\"" and parameters["value"][-1] == "\"":
        parameters["value"] = parameters["value"][1:-1]
    
    # checking if target environment variable present in the list
    
    variable_key = None
    for key in context["environment"]:
        if key.upper() ==  parameters["variable"].upper():
            variable_key = key
            break
    if variable_key is None:
        return False
    
    # check if the value already exist
    value = parameters["value"] if not parameters["value"].endswith("/") and not parameters["value"].endswith("\\") \
                                else parameters["value"][:-1]

    if value in context["environment"][variable_key] \
        or value.replace("/", "\\") in context["environment"][variable_key] \
            or value.replace("\\", "/") in context["environment"][variable_key]:
        return True

    # executing certain action
    if parameters["action"] == "prepend":
        context["environment"][variable_key] = value + ";" + context["environment"][variable_key]
    elif parameters["action"] == "append":
        if not context["environment"][variable_key].endswith(";"):
            context["environment"][variable_key] +=";"
        context["environment"][variable_key] += parameters["value"]
        if not context["environment"][variable_key].endswith(";"):
            context["environment"][variable_key] +=";"
    
    return True

def edit_file_remove(context, parameters, in_file, out_file):
    if parameters["what"][0] == "\"" and parameters["what"][-1] == "\"":
        parameters["what"] = parameters["what"][1:-1]

    while True:
        line = in_file.readline()
        if line == "":
            break
        elif parameters["what"] in line:
            continue
        else:
            out_file.write(line)
    pass

def edit_file_insert(context, parameters, in_file, out_file):
    if parameters["where"][0] == "\"" and parameters["where"][-1] == "\"":
        parameters["where"] = parameters["where"][1:-1]
    if parameters["what"][0] == "\"" and parameters["what"][-1] == "\"":
        parameters["what"] = parameters["what"][1:-1]

    while True:
        line = in_file.readline()
        if line == "":
            break
        elif parameters["where"] in line:
            out_file.write(line)
            out_file.write(parameters["what"]+"\n")
        else:
            out_file.write(line)
    pass

def edit_file(context, args, **kwargs):
    '''
    opens given file in --file=<file> for read
    looks for the string given with --string=<text>
    performs action gived with --action=<action>
    writes chages to temporary file
    replaces source file with teporary one
    '''

    log = context.get("logger")

    log.info("... Editing file ...")
    parameters = parse_args(log, args)
    
    if parameters is None:
        return False

    if not("file" in parameters.keys() and "action" in parameters.keys()):
        return False

    if parameters["action"] == "remove":
        if "what" not in parameters.keys():
            return False
    elif parameters["action"] == "insert":
        if not("where" in parameters.keys() and "what" in parameters.keys()):
            return False

    in_file = open(context["dependency_dir"]+parameters["file"], "r")
    if in_file is None:
        return False

    out_file = open(context["dependency_dir"]+parameters["file"]+".tmp", "w")
    if out_file is None:
        return False

    if parameters["action"] == "remove":
        edit_file_remove(context, parameters, in_file, out_file)
    elif parameters["action"] == "insert":
        edit_file_insert(context, parameters, in_file, out_file)

    in_file.close()
    out_file.close()

    os.remove(context["dependency_dir"]+parameters["file"])
    os.rename(context["dependency_dir"]+parameters["file"]+".tmp", context["dependency_dir"]+parameters["file"])

    return True

def cmake(context, args, **kwargs):
    '''
    calls to cmake.exe
    '''
    log = context.get("logger")

    for arg in range(0, len(args)):
        if args[arg].endswith(".bat") and not os.path.isabs(args[arg]):
            args[arg] = context["dependency_dir"]+args[arg]
    
    return run(log, "cmake", args, env = None if context.get("environment") is None else context["environment"], cwd=context["cwd"])[0]

def msbuild(context, args, **kwargs):
    '''
    calls to msbuild.exe
    '''
    log = context.get("logger")

    for arg in range(0, len(args)):
        if args[arg].endswith(".bat") and not os.path.isabs(args[arg]):
            args[arg] = context["dependency_dir"]+args[arg]
    
    res_bool, res_txt = run(log, "msbuild", args, env = None if context.get("environment") is None else context["environment"], cwd=context["cwd"])
    if res_txt.endswith("\n"):
        res_txt = res_txt[:-1]
    res_txt = res_txt.replace("\r", "");
    result_lines = res_txt.split("\n");
    
    if "\'msbuild\' is not recognized as an internal or external command," in result_lines[0]:
        return False

    return True

def create_fixed_environment_variable(context, args, **kwargs):
    '''
    creates system wide environment variable given by --variable=<variable> in the stage's context
    sets the value given by the --value=<value>
    '''
    # stripping logger
    log = context.get("logger")

    parameters = parse_args(log, args)
    if parameters is None:
        return False
    if sorted(parameters.keys()) != sorted(["variable", "value"]):
        result+="[create_environment_variable] invalid parameters set"+"\n"
        return False

    # reporting the command short description
    log.info("... Creating fixed(system wide) environment variable ...")

    # stripping quoted string    
    if parameters["value"][0] == "\"" and parameters["value"][-1] == "\"":
        parameters["value"] = parameters["value"][1:-1]

    # forming a cmd line
    params = "setx {0} {1}".format(parameters["variable"], parameters["value"])

    res_flag, res_txt = run(log, "cmd.exe", ["/c", "\""+params + " -m" + "\""])
    res_txt = res_txt.replace("\r", "")
    res_txt = res_txt.replace("\n", "")
    if res_txt == "ERROR: Access to the registry path is denied.":
        log.info("Cannot set system wide variable - \"{0}\"".format(parameters["variable"]))
        res_flag, res_txt = run(log, "cmd.exe", ["/c", "\""+params+"\""])
        res_txt = res_txt.replace("\r", "")
        res_txt = res_txt.replace("\n", "")
        if res_txt == "ERROR: Access to the registry path is denied.":
            log.error("Cannot set user variable - \"{0}\"".format(parameters["variable"]))
            log.info("Please set the \"{0}\" variable to \"{1}\" manually".format(parameters["variable"], parameters["value"]))
        else:
            log.success("User variable \"{0}\" is set".format(parameters["variable"]))
    else:
        log.success("System wide environment variable \"{0}\" is set".format(parameters["variable"]))

    pass


def create_environment_variable(context, args, **kwargs):
    '''
    creates environment variable given by --variable=<variable> in the stage's context
    sets the value given by the --value=<value>
    '''

    # stripping logger
    log = context.get("logger")

    # checking and parsing parameters
    parameters = parse_args(log, args)
    if parameters is None:
        return False
    if sorted(parameters.keys()) != sorted(["variable", "value"]):
        result+="[create_environment_variable] invalid parameters set"+"\n"
        return False

    # reporting the command short description
    log.info("... Creating temporary environment variable ...")

    # checking context exists
    if context.get("environment") is None:
        return False

    # stripping quoted string    
    if parameters["value"][0] == "\"" and parameters["value"][-1] == "\"":
        parameters["value"] = parameters["value"][1:-1]
        
    # checking if target environment variable present in the list
    variable_key = None
    for key in context["environment"]:
        if key.upper() ==  parameters["variable"].upper():
            variable_key = key
            break
        
    # set given variable's value
    context["environment"][parameters["variable"] if variable_key is None else variable_key]=parameters["value"]
    
    return True


def cd(context, args, **kwargs):
    '''
    Changes the current working directory entry in the stage's context
    '''
    if len(args) == 0:
        log.error("Cannot change directory")
        return False
    if len(args) > 1:
        log.error("Cannot multiply change directory for single time")

    if os.path.isdir(context["cwd"]+args[0]):
        context["cwd"]+=args[0]
    return True
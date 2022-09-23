import subprocess
import os
import glob
import argparse

def str_to_red(str):
    return "\033[91m{}\033[00m".format(str)

def str_to_green(str):
    return "\033[92m{}\033[00m".format(str)

def str_to_yellow(str):
    return "\x1b[33m{}\033[00m".format(str)

class Tester:
    def __init__(self, module, unit):
        self.module_target = module
        self.unit_target = unit
        self.tests_dir = "tests"
        self.utils_dir = "utils"
        self.bins_dir = "bin"
        self.modules = []
        self.utils = self.get_tester_utils()
        self.webserv_objs = self.get_webserv_objs();
        self.test()
 
    def delete_tester_bins(self):
        bins = os.listdir(self.bins_dir)
        for bin in bins:
            os.remove(f"{self.bins_dir}/{bin}")

    def make_webserv(self):
        os.chdir("..")
        process = subprocess.run(["make"], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        os.chdir("tester")
        return process.returncode == 0

    def get_webserv_objs(self):
        dir_to_exclude = []
        files = glob.glob('../build/**/*.o', recursive=True)
        files_paths = [_ for _ in files if _.split("\\")[0] not in dir_to_exclude]
        main_index = files_paths.index("../build/main.o")
        del files_paths[main_index]
        return files_paths

    def get_tester_utils(self):
        utils_files = os.listdir(self.utils_dir)
        utils = []
        for util in utils_files:
            [file_name, file_extension] = os.path.splitext(util)
            if file_extension == ".cpp":
                utils.append(f"utils/{util}")
        return utils

    def compile_unit(self, unit, module, utils, webserv_objs, output_name):
        output_path = f"bin/{output_name}"
        unit_path = f"{self.tests_dir}/{module}/{unit}.cpp"
        exec = ["clang++", "-o", output_path, unit_path]
        exec.extend(utils)
        exec.extend(webserv_objs)
        compile_process = subprocess.run(exec, stdout=subprocess.DEVNULL)
        compiled = True if compile_process.returncode == 0 else False
        return compiled

    def test_unit(self, module, unit):
        unit_path = f"{self.bins_dir}/{module}_{unit}"
        exec = [unit_path]
        try:
            process = subprocess.run(exec, stdout=subprocess.DEVNULL)
        except:
            print(str_to_red("Invalid Unit"))
            return
        valid = True if process.returncode == 0 else False
        return valid

    def compile_and_test_unit(self, module, unit):
        unit_compile_name = f"{module}_{unit}"
        compiled = self.compile_unit(unit, module, self.utils, self.webserv_objs, unit_compile_name)
        compiled_str = str_to_green("COMPILED     ") if compiled else str_to_red("NOT COMPILED ")
        print(compiled_str, end="")
        test = False
        if compiled:
            test = self.test_unit(module, unit)
        tested_str = str_to_green("OK ") if test else str_to_red("KO ")
        print(tested_str, unit)
        print()


    def test_module(self, module):
        try:
            module_units = os.listdir(f"{self.tests_dir}/{module}")
        except:
            print(str_to_red("Cannot find module"))
            return
        print(f"{str_to_yellow(module)}")
        for unit in module_units:
            [unit_file_name, file_extension] = os.path.splitext(unit)
            if file_extension != ".cpp":
                continue
            unit_compile_name = f"{module}_{unit_file_name}"
            compiled = self.compile_unit(unit_file_name, module, self.utils, self.webserv_objs, unit_compile_name)
            compiled_str = str_to_green("COMPILED     ") if compiled else str_to_red("NOT COMPILED ")
            print(compiled_str, end="")
            test = False
            if compiled:
                test = self.test_unit(module, unit_file_name)
            tested_str = str_to_green("OK ") if test else str_to_red("KO ")
            print(tested_str, unit)
        print()

    def test_all(self):
        modules_files = os.listdir(self.tests_dir)
        for module in modules_files:
            self.test_module(module)

    def test(self):
        self.delete_tester_bins()
        print(str_to_yellow("Making Webserv... "), end="")
        made = self.make_webserv()
        if made == False:
            print(str_to_red("Cannot make webserv"))
            return
        print(str_to_green("OK"))
        print()

        if self.module_target != None and self.unit_target != None:
            self.compile_and_test_unit(self.module_target, self.unit_target)
        elif self.module_target != None:
            self.test_module(self.module_target)
        else:
            self.test_all()

if __name__=="__main__":
    args_parser = argparse.ArgumentParser(description="Webserv tester")
    args_parser.add_argument('-m', metavar='--module', type=str, help='Module name to test')
    args_parser.add_argument('-u', metavar='--unit', type=str, help='Unit of a module to test')
    args = args_parser.parse_args()

    Tester(args.m, args.u)

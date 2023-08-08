# 保存命令执行结果到指定文件
import lldb
import os
import argparse  # https://docs.python.org/3/library/argparse.html


def __lldb_init_module(debugger: lldb.SBDebugger, internal_dict: dict):
    debugger.HandleCommand("command script add -f dump.command_dump dump")


def command_dump(debugger: lldb.SBDebugger, command: str, result: lldb.SBCommandReturnObject, internal_dict: dict):
    output_dir: str = debugger.GetSelectedTarget().GetEnvironment().Get("COMMAND_OUTPUT_DIR")

    parts = command.strip().split("--", 1)  # 从左侧开始按照 "--" 进行一次切割
    dump_args = parts[0].strip()  # dump 命令的参数，不允许使用 --，避免和实际命令冲突
    dump_command = parts[1].strip()  # dump 命令后接的实际命令
    parser = argparse.ArgumentParser(prog='dump', description="save command result to file.")
    parser.add_argument("-f", help="--file", type=str, required=False, default=f"{dump_command}.log")
    parser.add_argument("-a", help="--append", action='store_true', required=False, default=False)
    args = parser.parse_args(dump_args.split(" ") if dump_args else None)

    # 支持绝对路径、相对路径和指定目录
    path = args.f if os.path.isabs(args.f) else f"{os.path.join(output_dir, args.f) if output_dir else args.f}"
    file = open(path, f"{'a' if args.a else 'w'}")
    debugger.SetOutputFileHandle(file, False)  # 输出到文件
    debugger.HandleCommand(dump_command)
    debugger.SetOutputFileHandle(None, False)  # 还原到标准输出，否则影响控制台后续命令的展示

#! /usr/bin/python3

from sys import argv, stderr, exit
from pathlib import Path

import subprocess
import shlex

from typing import List, Any


def cmd_run_echoed(cmd: List[str], echo: bool = True, **kwargs: Any) -> int:
    """Runs a command and prints it.

    Args:
        cmd: List[str]: Command in list form
        echo: bool: Echo command
        **kwargs: Same as subprocess.call

    Returns:
        int: Return code of the command
    """
    if echo:
        print(f"[CMD] {' '.join(map(shlex.quote, cmd))}")
    return subprocess.call(cmd, **kwargs)


def main():
    program = argv.pop(0)
    try:
        executable = argv.pop(0)
    except IndexError:
        print(f"Usage: {program} <executable>", file=stderr)
        exit(1)

    exit_code = cmd_run_echoed([executable])
    if exit_code != 0:
        print(f"Executable exited with non zero exit code: {exit_code}",
              file=stderr)
        exit(exit_code)

    name = Path(executable).name
    dot_file = f"graphs/{name}.dot"
    pdf_file = f"graphs/{name}.pdf"
    exit_code = cmd_run_echoed(["dot", "-Tpdf", "-o", pdf_file, dot_file])
    if exit_code != 0:
        print(f"Graphviz exited with non zero exit code: {exit_code}",
              file=stderr)
        exit(exit_code)


if __name__ == "__main__":
    main()

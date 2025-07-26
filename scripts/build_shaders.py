#!/usr/bin/env python3

# Small python script to compile all shaders associated with the project

import os
import shutil
import subprocess
import argparse

SUFFIX_TO_PROFILE = {
    "vert": "vs_6_0",
    "frag": "ps_6_0"
}

def compile_shader(filename, input, output):
    basename = os.path.basename(filename)
    name_parts = basename.split("_")

    if len(name_parts) < 2:
        print(f"[!] Skipping {basename}: unexpected filename format")
        return

    suffix_with_ext = name_parts[-1]
    shader_type = suffix_with_ext.split(".")[0]

    profile = SUFFIX_TO_PROFILE.get(shader_type)

    if not profile:
        print(f"[!] Skipping {basename}: unknown shader type '{shader_type}'")
        return

    filename_no_ext = os.path.splitext(filename)[0]
    output_filename = filename_no_ext.replace("_", ".") + ".spv"
    output_path = os.path.join(output, output_filename)

    command = [
        "slangc",
        "-target", "spirv",
        "-profile", profile,
        "-entry", "main",
        "-o", output_path,
        "-matrix-layout-column-major",
        os.path.join(input, filename)
    ]

    print(f"[+] Compiling {basename} -> {output_path}")
    result = subprocess.run(command, capture_output=True, text=True)

    if result.returncode != 0:
        print(f"[!] Error compiling {basename}:\n{result.stderr}")
    else:
        print(f"[+] Compiled: {output_path}")

def main():
    parser = argparse.ArgumentParser(description="Compile Slang shaders to SPIR-V")
    parser.add_argument("--input", required=True, help="Input directory containing .slang shaders")
    parser.add_argument("--output", required=True, help="Output directory for .spv files")
    args = parser.parse_args()

    os.makedirs(args.output, exist_ok=True)

    if not shutil.which("slangc"):
        print("[!] slangc not found in PATH. Please install or update PATH.")
        return

    for file in os.listdir(args.input):
        if file.endswith(".slang"):
            compile_shader(file, args.input, args.output)

if __name__ == "__main__":
    main()


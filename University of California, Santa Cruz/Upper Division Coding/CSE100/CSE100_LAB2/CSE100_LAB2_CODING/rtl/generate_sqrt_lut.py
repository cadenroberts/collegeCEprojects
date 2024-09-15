#!/usr/bin/env python3
# Copyright (c) 2024 Ethan Sifferman.
# All rights reserved. Distribution Prohibited.

import sys
import math

def generate_sqrt_memh(filename):
    with open(filename, 'w') as file:
        file.write(f"// Generated SQRT Lookup Table\n")
        file.write(f"// 8-bit input, 8-bit output\n")
        file.write(f"\n")
        for i in range(256):
            sqrt_value = round(math.sqrt(i))
            rounded_value = int(round(sqrt_value))
            hex_value = hex(rounded_value)[2:]
            file.write(f"{hex_value}\n")

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print(f"Usage: {sys.argv[0]} <filename>")
    else:
        filename = sys.argv[1]
        generate_sqrt_memh(filename)

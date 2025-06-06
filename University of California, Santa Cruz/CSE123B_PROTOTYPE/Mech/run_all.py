#!/usr/bin/env python3
import subprocess
import argparse
import time
import sys
from pathlib import Path

def run_build():
    print(">>> Building project…")
    subprocess.run(["idf.py", "build"], check=True)

def flash_and_log(port, duration, log_path):
    print(f">>> Flashing & monitoring on {port} for {duration}s → {log_path}")
    with open(log_path, "w") as lf:
        # idf.py -p PORT flash monitor
        proc = subprocess.Popen(
            ["idf.py", "-p", port, "flash", "monitor"],
            stdout=lf, stderr=lf
        )
        try:
            time.sleep(duration)
        finally:
            proc.terminate()
            proc.wait()

def convert_log(log_path, json_path):
    print(f">>> Converting {log_path} → {json_path}")
    # adjust this if convert_log.py takes arguments: here we pass input/output
    subprocess.run(["python", "convert_log.py", str(log_path), str(json_path)], check=True)

def main():
    p = argparse.ArgumentParser(
        description="Build, flash+monitor (to log.txt), then convert log → JSON"
    )
    p.add_argument("-p","--port",    default="/dev/ttyACM0",
                   help="Serial port for idf.py monitor")
    p.add_argument("-d","--duration",type=int, default=30,
                   help="Seconds to capture serial output")
    p.add_argument("--log",   default="log.txt",
                   help="Filename for raw log")
    p.add_argument("--json",  default="log.json",
                   help="Filename for output JSON")
    args = p.parse_args()

    # ensure convert_log.py is on PATH or in CWD
    if not Path("convert_log.py").exists():
        print("Error: convert_log.py not found in current directory.", file=sys.stderr)
        sys.exit(1)

    run_build()
    flash_and_log(args.port, args.duration, args.log)
    convert_log(args.log, args.json)
    print("All done!")
    
if __name__ == "__main__":
    main()

#How to run

''' 
./run_all.py \
  --port /dev/ttyACM0 \
  --duration 60 \
  --log log.txt \
  --json log.json
'''
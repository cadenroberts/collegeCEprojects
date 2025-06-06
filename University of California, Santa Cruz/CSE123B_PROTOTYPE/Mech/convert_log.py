import re
import json
import sys
from pathlib import Path

def main():
    # Get filenames from command-line args (with defaults)
    in_file  = Path(sys.argv[1]) if len(sys.argv) > 1 else Path("log.txt")
    out_file = Path(sys.argv[2]) if len(sys.argv) > 2 else Path("output_seconds.json")

    #print(f"[DEBUG] Input file: {in_file}")
    #print(f"[DEBUG] Output file: {out_file}")

    # Initialize the data structure
    data = {
        'pinky': [],    # Sensor 0
        'ring': [],     # Sensor 1
        'middle': [],   # Sensor 2
        'pointer': [],  # Sensor 3
        'thumb': [],    # Sensor 4
        'timestamp_s': []  # Time in seconds
    }

    names = ['pinky', 'ring', 'middle', 'pointer', 'thumb'] # Array with all names of fingers
    cycle = 0 # Counter for cycles

    #debugging assistance
    #total_lines = 0
    #matched_lines = 0

    if not in_file.exists():
        print(f"Error: input file '{in_file}' not found.", file=sys.stderr)
        sys.exit(1)

    #pattern = re.compile(r'\((\d+)\) FLEX: Sensor (\d+) \| Flex: ([\d.]+)%')
    
    # Read and parse the log file
    with open(in_file, 'r') as f:
        for line in f:
            m = re.search(r'\((\d+)\) FLEX: Sensor (\d+) \| Flex: ([\d.]+)%', line)
            if not m:
                continue
            ts, sensor_idx, flex = m.groups()
            sensor_idx = int(sensor_idx)
            flex = float(flex)

            # Only advance timestamp on sensor 0 entries
            if sensor_idx == 0:
                data['timestamp_s'].append(cycle * 0.5)
                cycle += 1

            # Append flex value
            data[names[sensor_idx]].append(flex)

    #print(f"[DEBUG] Total lines read: {total_lines}")
    #print(f"[DEBUG] Lines matched:    {matched_lines}")
    #print(f"[DEBUG] Cycles counted:   {cycle}")
    
    # Write out JSON
    with open(out_file, 'w') as f:
        json.dump(data, f, indent=2)

    print(f"Wrote {out_file} with {len(data['timestamp_s'])} timestamps")

if __name__ == "__main__":
    main()

#Usage

#Default (log.txt → output_seconds.json)
#python convert_log.py

# Custom filenames
#python convert_log.py custom_log.txt custom_output.json

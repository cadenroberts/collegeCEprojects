#!/bin/bash
for file in *; do
    if [ -f "$file" ]; then
        echo "File: $file"
        awk 'NR % 2 == 0 {print FILENAME": "$0}' "$file"
    fi
done


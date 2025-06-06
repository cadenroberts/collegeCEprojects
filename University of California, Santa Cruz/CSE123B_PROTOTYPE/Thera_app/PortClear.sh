#!/bin/bash
for port in {3000..3010}; do   lsof -ti :$port | xargs -r kill -9; done

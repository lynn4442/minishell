#!/bin/bash

# Check if a commit message is provided
if [ -z "$1" ]; then
    echo "Usage: $0 \"commit message\""
    exit 1
fi

# Clean up the build files
make fclean

# Git commands
git add .
git commit -m "$1"
git push

echo "✅ Changes pushed successfully!"

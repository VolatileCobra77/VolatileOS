#!/bin/bash

VM_NAME="Volatile OS"

# --- pre-launch stuff ---
./build.sh

# --- launch VM ---
VBoxManage startvm "$VM_NAME" --type gui
#!/bin/sh

INSTALL_DIR=~/.local/bin

mkdir -p $INSTALL_DIR
cp bin/xor* $INSTALL_DIR

echo "Installed to '$INSTALL_DIR', please add it to your \$PATH"

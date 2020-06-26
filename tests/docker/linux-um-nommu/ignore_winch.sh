#!/bin/sh

trap 'echo "ignoring SIGWINCH (FIXME)"' WINCH

"$@"

#!/usr/bin/env python
from collections import defaultdict
import sys
import time
import curses
import math

"""
Continously shows the histogram of a stream of strings.

Examples:

Histogram of random numbers:
while true; do echo $RANDOM | awk '{print $1 % 10}'; done | python histo.py

Histogram of all syscalls in the system:
sysdig "evt.type!='switch' and evt.dir!='<'" --print=' %evt.type' | python histo.py
"""

def main(stdscr):
    bins = defaultdict(int)
    period = 0.5 # print every 500ms
    last = 0

    def print_histo(stdscr):
        stdscr.clear()
        sbins = sorted(bins, key=lambda k: int(bins[k]), reverse=True)
        for i, k in enumerate(sbins):
            c = bins[k]
            pk = k[:20]
            log_c = max(1, int(math.log(c)))
            try:
                stdscr.addstr(i, 0, '{}'.format(i+1))
                stdscr.addstr(i, 5, '{}'.format(pk))
                stdscr.addstr(i, 22, '{}'.format(c))
                stdscr.addstr(i, 32, '{}'.format('#' * log_c))
            except:
                pass
        stdscr.refresh()

    stdscr = curses.initscr()
    stdscr.clear()

    while True:
        try:
            line = sys.stdin.readline()
        except IOError:
            continue
        except KeyboardInterrupt:
            sys.exit(0)
        event = line.strip()
        bins[event] += 1
        now = time.time()
        if (now - last) >= period:
            print_histo(stdscr)
            last = time.time()

curses.wrapper(main)

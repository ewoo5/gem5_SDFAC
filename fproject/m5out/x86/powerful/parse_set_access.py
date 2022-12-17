#!/usr/bin/python

import sys
import os
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt

def parse():

  if (len(sys.argv) != 4):
    print("Usage: parse_set_access.py <input file> <output file> <set>")
    return

  input_fd = open(sys.argv[1], "rb")
  output_fd = open(sys.argv[2], "w")

  target_set = int(sys.argv[3])
  target_bins = 50

  # get last tick and determine how to split time into 20 bins
  input_fd.seek(-2, os.SEEK_END)
  while(input_fd.read(1) != b'\n'):
    input_fd.seek(-2, os.SEEK_CUR)
  input_fd.seek(-2, os.SEEK_CUR)
  while(input_fd.read(1) != b'\n'):
    input_fd.seek(-2, os.SEEK_CUR)
  last_line = input_fd.readline().decode()
  print(last_line)
  line_list = last_line.split()
  max_tick = int(line_list[3]) + 1
  bin_interval = max_tick//target_bins
  input_fd.close()

  input_fd = open(sys.argv[1], "r")

  # parse through file and add into timeline histogram whenever target_set is accessed
  iteration = 0
  hit_bin = []
  miss_bin = []
  for i in range(target_bins):
    hit_bin.append(0)
    miss_bin.append(0)
  line = input_fd.readline() # first line is garbage
  Lines = input_fd.readlines()
  for line in Lines:
    data_list = line.split()
    """
    Example file line:
    Object: system.cpu0.dcache.tags          curTick: 0      Set: 0         Miss
    [0]     [1]                              [2]     [3]     [4]  [5]       [6]
    """
    if(len(data_list) == 7):
        if(data_list[1].find("cpu1") != -1):
          if (int(data_list[5]) == target_set):
            time_bin = int(data_list[3])//bin_interval
            if (data_list[6] == "Miss"):
              miss_bin[time_bin] = miss_bin[time_bin] + 1
            else:
              hit_bin[time_bin] = hit_bin[time_bin]  + 1
            output_fd.write(line)
            iteration = iteration + 1
            if (iteration % 100000 == 0):
              print("iteration: %d, tick: %ld" % (iteration, int(data_list[3])))

  print("Closing files...")

  input_fd.close()
  output_fd.close()

  print("Processing graphs...")
  # display histogram
  print("Creating subplots")
  fig, ax = plt.subplots()
  print("Setting axes")
  #hit_y = list(hit_bin.values())
  hit_y = hit_bin
  print(hit_y)
  #x = list(hit_bin.keys())
  #print(x)
  #miss_y = list(miss_bin.values())
  miss_y = miss_bin
  print(miss_y)
  ax.plot(hit_y, label="Hit", color='blue')
  ax.tick_params(axis='y', labelcolor='blue')
  ax2 = ax.twinx()
  ax2.plot(miss_y, label="Miss", color='red')
  ax2.tick_params(axis='y', labelcolor='red')
  plt.legend()
  plt.savefig("figure.png")
  return

def __main__():
  parse()

if(__name__ == "__main__"):
  __main__()
#!/usr/bin/python

import os, sys
import matplotlib.pyplot as plt

if __name__ == "__main__":
	fp = open(sys.argv[1], "r");
	storage = []
	final_special = []
	for line in fp:
		index = line.find("Mean")
		if index != -1:
			string = line.split(":")
			storage.append(float(string[1]))

	if sys.argv[1] == "ram_access_measurement3.txt":
		for i in range(0, 16):
			j = i
			var = 0
			while j < len(storage):
				var = var + storage[j]
				j = j + 16
			final_special.append(round(var/5, 2))
		plt.plot(final_special)
		plt.show()
					     			

			

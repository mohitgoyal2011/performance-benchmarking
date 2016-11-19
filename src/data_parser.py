import os,sys
import statistics
import commands
import time

performance_measurement_overhead = 34
BUILD_FOLDER = "../build/"
SECONDS_PER_CYCLE = 0.556
def measure_ram_access():
	for i in [1024, 2048, 16*1024, 32*1024, 64*1024, 128*1024, 256*1024, 512*1024, 1024*1024, 1024*1024*2, 1024*1024*3, 1024*1024*4, 1024*1024*8, 1024*1024*64, 1024*1024*128, 1024*1024*256]:
		clock_cycles = []
		for j in range(0,10):
			output = commands.getoutput(BUILD_FOLDER + 'ram_access_measurement ' + str(i))
			if len(output.split(' ')) >= 3:
				clock_cycles.append(float(output.split(' ')[2]))
		print "------------------------------------------------------------"
		print "RAM Access Time",i
		print "RAM Latency(Mean):",statistics.mean(clock_cycles)
		print "RAM Latency(Variance):",statistics.pvariance(clock_cycles)
		print "RAM Latency Overhead(Standard Deviation):",statistics.pstdev(clock_cycles)
		print "------------------------------------------------------------"

def measure_performance_measure_overhead():
	output = commands.getoutput(BUILD_FOLDER + 'performance_measurement_overhead' )
	print output
	output = commands.getoutput(BUILD_FOLDER + 'loop_overhead_measurement' )
	print output


def measure_system_call():
	clock_cycles = []
	for i in range(0,10000):
		output = commands.getoutput(BUILD_FOLDER + 'system_call_measurement' )
		if len(output.split(' ')) >= 3:
			clock_cycles.append(int(output.split(' ')[2]))
	print "System Call Overhead(Mean):",statistics.mean(clock_cycles)
	print "System Call Overhead(Variance):",statistics.pvariance(clock_cycles)
	print "System Call Overhead(Standard Deviation):",statistics.pstdev(clock_cycles)
	
def measure_procedure_call():
	for i in range(0,8):
		output = commands.getoutput(BUILD_FOLDER + 'procedure_call' + str(i) + '_measurement' )
		print "Arguments " + str(i)
		print output
		time.sleep(1)

def measure_task_creation():
	output = commands.getoutput(BUILD_FOLDER + 'task_creation_fork_measurement' )
	print output
	output = commands.getoutput(BUILD_FOLDER + 'task_creation_pthread_measurement' )
	print output

def measure_context_switch():
	output = commands.getoutput(BUILD_FOLDER + 'context_switch_fork_measurement' )
	print output
	output = commands.getoutput(BUILD_FOLDER + 'context_switch_pthread_measurement' )
	print output

def measure_ram_bandwidth():
	bandwidth = []
	for i in range(0,10):
		output = commands.getoutput(BUILD_FOLDER + 'memory_read_bandwidth_measure' )
		clock_cycles = int(output.strip())
		result = 1.0/(clock_cycles*SECONDS_PER_CYCLE*pow(10,-9))
		bandwidth.append(result)
	print "Memory Read Bandwidth(Mean):",statistics.mean(bandwidth),"GB/s"
	print "Memory Read Bandwidth(Variance):",statistics.pvariance(bandwidth)
	print "Memory Read Bandwidth(Standard Deviation):",statistics.pstdev(bandwidth),"GB/s"
	print "------------------------------------------------------------"
	bandwidth = []
	for i in range(0,10):
		output = commands.getoutput(BUILD_FOLDER + 'memory_write_bandwidth_measure' )
		clock_cycles = int(output.strip())
		result = 1.0/(clock_cycles*SECONDS_PER_CYCLE*pow(10,-9))
		bandwidth.append(result)
	print "Memory Write Bandwidth(Mean):",statistics.mean(bandwidth),"GB/s"
	print "Memory Write Bandwidth(Variance):",statistics.pvariance(bandwidth)
	print "Memory Write Bandwidth(Standard Deviation):",statistics.pstdev(bandwidth),"GB/s"

def measure_page_fault():
	output = commands.getoutput(BUILD_FOLDER + 'page_fault_measurement' )
	print output

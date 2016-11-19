import data_parser as dp
import sys

if len(sys.argv) >1 :
	a = int(sys.argv[1])
	for i in range(0,5):
		print "Iteration " + str(i)
		print " * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *"
		if a == 1:
			dp.measure_performance_measure_overhead()
		if a == 2:
			dp.measure_procedure_call()
		if a == 3:
			dp.measure_system_call()
		if a == 4:
			dp.measure_task_creation()
		if a == 5:	
			dp.measure_context_switch()
		if a == 6:
			dp.measure_ram_access()
		if a == 7:
			dp.measure_ram_bandwidth()
		if a == 8:
			dp.measure_page_fault()
		print " * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *"


									Cache-Simulator
This is a simulator which simulates the cache with the LRU (least recently) used algorithm. It simulates the cache in all 128 cache configurations one after another. It will output all the configuration in an output.txt file. The output will have the following style:
<cache size>  <block size>  <cache placement type>  <write policy>  <hit rate>  <total bytes from memory to cache>  <total bytes from cache to memory>  <number of blocks within each block set>.

To run the simulator, P_3.exe  <tracefile>
where: 
•	<tracefile> This is the name of the text file which contains all the memory access traces.

The simulator runs in the following configuration below:

Cache Size (in bytes):
1.	1024
2.	4096
3.	65536
4.	131072

Block Size (in bytes):
1.	4
2.	8
3.	16
4.	128

Cache Placement Type:
1.	Direct Mapped (DM)
2.	2-way set associative (2W)
3.	4-way set associative (4W)
4.	Fully associative (FA)

Write Policy:
1.	Write back (WB)
2.	Write through (WT)

An Example of the trace file:
•	read 0x04000004
•	write 0x02000007

The first is a read and the second is a write.



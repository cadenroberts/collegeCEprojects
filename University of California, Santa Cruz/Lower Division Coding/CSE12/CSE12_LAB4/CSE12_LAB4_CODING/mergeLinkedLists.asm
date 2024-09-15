#---------------------------------------------------------------------------------------------#
mergeLinkedLists:
#Function to merge the 2 sorted lists together and the resultant list is also sorted
#Inputs: a0 = address of head of 1st linked list; so if a0=0x10040000 points to empty list, then mem[0x10040000]=0
#	a1 = = address of head of 2nd linked list;
#Output: the first linked contains the combined elements of 1st and second lists sorted
	mv t4, a0
	lw t6, (a1)
	lw a1, (t6)
	mv s1, ra
	
	combine:
	jal insertSorted
	mv a0, t4
	lw t6, 4(t6)
	beqz t6, done
	lw a1, (t6)
	j combine

	done:
	mv ra, s1
	ret

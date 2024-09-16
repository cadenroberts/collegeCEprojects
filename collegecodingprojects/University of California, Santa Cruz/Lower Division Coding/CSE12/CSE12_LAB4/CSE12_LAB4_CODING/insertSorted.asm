#---------------------------------------------------------------------------------------------#
insertSorted:
# Function to insert a new node into the given linkedlist in ascending order
# Inputs: a1 = value of the node key
#	a0 = address of head of linked list; so if a0=0x10040000 points to empty list, then mem[0x10040000]=0
# Function to insert a new node into the given linked list in ascending order
# Inputs: a1 = value of the node key
#         a0 = address of head of linked list; if a0 = 0x10040000 points to an empty list, then mem[0x10040000] = 0
# Outputs: a0 = pointer to the final node in the current sorted linked list
    # Allocate space for the new node
    mv t3, ra
    mv t1, a0
    li a7, 9            # ecall number for sbrk
    li a0, 8            # Adjust the size based on your node structure
    ecall

    # Store the node value
    sw a1, 0(a0)

    # Initialize the next pointer to NULL
    sw zero, 4(a0)
    mv ra, t3
    
    lw t2, (t1)
    beqz t2, emptyy
    
    mv t0, t1
    lw t1, (t1)
    
    lw t3, (t1)
    ble a1, t3, frontt
    
    checkk:
    lw t3, (t1)
    ble a1, t3, heree
    lw t3, 4(t1)
    beqz t3, endd
    mv t0, t1
    lw t1, 4(t1)
    j checkk

    emptyy:
    sw a0, (t1)
    ret
    
    heree:
    sw a0, 4(t0)
    sw t1, 4(a0)
    ret
    
    frontt:
    sw a0, (t0)
    sw t1, 4(a0)
    ret
    
    endd:
    sw a0, 4(t1)
    ret

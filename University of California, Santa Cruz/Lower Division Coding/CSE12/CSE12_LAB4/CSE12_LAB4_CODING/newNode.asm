newNode:
    # Allocate memory for a new node
    li a7, 9            # ecall number for sbrk
    li a0, 8            # Adjust the size based on your node structure
    ecall

    # Store the node value
    sw a1, 0(a0)

    # Initialize the next pointer to NULL
    sw zero, 4(a0)

    # Return the pointer to the allocated node
    ret


print_list:
nexxt:
    lw t1, 0(a1)   # Load the value at the current node
    li a7, 1       # System call number for printing an integer
    addi a0, t1, 0 # Set a0 to the value to be printed
    ecall          # Print the integer

    li a7, 11      # System call number for printing a space
    li a0, 0x2C    # ASCII value for space character
    ecall          # Print the space

    lw a1, 4(a1)   # Move to the next node in the linked list

    addi t2, zero, 1
    bgeu a1, t2, nexxt  # If the next pointer is not NULL, continue the loop

    
    ret            # Return from the function
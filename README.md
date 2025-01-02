Overview
In this project, you will be implementing a (partial) RISCV interpreter in C.

Since there are multiple components to this project, we suggest that you read through the entire writeup before beginning because the Main Program section below provides tips on debugging your code.

We provide several starter files, which you should also familiarize yourself with before beginning the project. Your job is to implement everything marked as // TODO:.

While it is certainly possible to use the native C compiler on your machine, we highly suggest that you make sure that your implementation works in either the VM, or SSH on the UGCLinux machines.

What to Submit
A complete implementation of linkedlist.c
A complete implementation of hashtable.c
A complete implementation of riscv.c
Compiling
You can find starter files here Download here. We recommend taking a look at the header files (*.h) and the source files (*.c).

A Makefile is provided to help with compiling your code. As you progress through each section of this project, there will be a make target that you can use to compile the main function. This will generate an executable that you can run to test your code.

The available options for make are as follows:

make linkedlist
make hashtable
make riscv_interpreter
By default, make will create the riscv_interpreter executable to run.

Simulating Memory
Since you will be supporting memory instructions, you need to have a way to represent memory.

A naive attempt would allocate a large array with 2^31 elements, and directly index into the array for each memory request. However, this poses certain limitations:

the majority of the array will be unused, leading to waste
it may not always be possible to allocate 2^31 bytes of memory for a single process (OS limitation)
Thus, we require you to simulate memory in a specific way, as specified below, which addresses both of the above limitations.

Associative Linked List
You may already be familiar with the notion of an associative linked list. It is one way to represent a dictionary, or a set of key-value mappings. Each node contains a key, a value, and a pointer to the next node. In this case, we are working with mappings from a 32-bit integer (the key, representing a memory address) to another 32-bit integer (the value at the given address).

The associative linked list can be visualized as follows:

associative_linked_list.png

For this part of the project, it will be valuable to walk through the ArrayList Lab and familiarize yourself with pointers and structs.

Begin by implementing an associative linked list in linkedlist.c. The specification of each function can be found in linkedlist.h.

linkedlist_t *ll_init()
void ll_add(linkedlist_t *list, int key, int value)
int ll_get(linkedlist_t *list, int key)
int ll_size(linkedlist_t *list)
Run make linkedlist to create a linkedlist executable to run. This will run the code found in linkedlist_main.c.

While an associative linked list can certainly simulate memory, it has performance limitations. You may have noticed that as you insert more key-value pairs into the linked list, the time it takes to find the matching key grows linearly. That is, the data structure provides an O(n) lookup. Let's see if we can use a different data structure to achieve a better performance.

Hash Table
You will now simulate memory in this project using a hash table, which provides much better performance than the associative linked list from the previous section. Once again, you are working with mappings from a 32-bit integer (the key, representing a memory address) to another 32-bit integer (the value at the given address).

To implement your hashtable, you MUST use your linked list implementation. That is, you should represent the buckets in your hash table using associative linked lists, which you previously implemented. It can be visualized as follows:

hashtable.png

Implement the following functions in hashtable.c. The specification of each function can be found in hashtable.h.

hashtable_t *ht_init(int num_buckets)
void ht_add(hashtable_t *table, int key, int value)
int ht_get(hashtable_t *table, int key)
int ht_size(hashtable_t *table)
You do not need to worry about the resizing case of a traditional hashtable. We intentionally use a fixed number of buckets, and will initialize your hashtable with a high number of buckets when grading the performance of your hashtable. Similarly, feel free to use a high number of buckets to simulate memory in your riscv interpreter, but do keep in mind memory constraints of the system (i.e. you don't need 2^31 buckets).

Run  make hashtable to create a hashtable executable to run. This will run the code found in hashtable_main.c.

Interpreter
For this part of the project, you should implement the init() and step() functions in riscv.c. The specification for these functions can be found in riscv.h.

String Parsing
You will need to convert a string corresponding to a RISCV instruction to something that you can subsequently work with. Hence, you will have to do some string splitting. You may already be familiar with the tools available for string splitting; if not, it is worth reading up on them

You have probably noticed that RISCV instructions follow a certain format depending on the type of operation. Before you begin implementing anything, we strongly suggest that you take a look at the get_op_type function provided in riscv.c. We recommend that you use this, as it will likely make things simpler for you.

Spaces should not affect the correctness of your interpreter. For example:

addi x5,x0,12 should be handled the same way as addi   x5,  x0 ,   12
sw x12,4(x0) should be handled the same way as sw   x12 , 4  ( x0  )
Your interpreter should support both decimal and hexadecimal immediate values. This means that the following are equivalent:

addi x4, x0, 17 should be equivalent to addi x4, x0, 0x11
sw x5, 16(x0) should be equivalent to sw x5, 0x10(x0)
Operations
Your interpreter must support the following operations in addition to empty strings (i.e. null terminator):

R-type	ADD, SUB, AND, OR, XOR, NOR, SLT, SLL, SRA
I-type	ADDI, ANDI, ORI, XORI, SLTI
Memory-type **	LW, LB, SW, SB
U-type	LUI
** Note: "Memory-type" is not a formal RISCV instruction type; we group the instructions this way to make parsing easier for you.

Don’t overthink these; most of them can be done quite simply. However, take note of edge cases and check that your interpreter behaves as specified in the RISCV reference manual.

Memory is little-endian. We recommend implementing memory as byte-addressed, which will allow each memory address (0x00000000, 0x00000001, ...) to exist as keys in your simulated memory, mapping to a value of a single byte. Other address modes are welcome (such as word-addressed), but discouraged.

Strings passed into your interpreter will either be empty strings or valid RISCV instructions. Valid means that the operation will be in the list of supported operations above, registers will be valid x[0-31], immediates will be a maximum of 20 bits in either decimal or hexadecimal notation, at least one space follows the operation, and commas are used as the separator between tokens (there can still be any number of spaces).

Formally, all valid instructions satisfy the regex format (though, the set of tokens may not match the format of the operation):
[a-z]+ +x[0-9]+ *, *((x[0-9]+ *, *(x[0-9]+|(0x[0-9a-f]+|-?[0-9]+)))|(0x[0-9a-f]+|-?[0-9]+)( *\( *x[0-9]+ *\))?)

The Main Program
Run  make riscv_interpreter to create a riscv_interpreter executable to run. This will run the code found in riscv_interpreter.c.

To simulate instructions, type them in after running ./riscv_interpreter. When you are done, press Ctrl+D to show the final values in the registers after evaluation. To automate this process, feel free to use redirection from a file as shown in this Tutorial 3 of this Unix/Linux guide.  

Debugging. The provided riscv_interpreter.c has a -d flag that can be passed on the command line to enable debugging. Debugging mode will print out each instruction that is executed, along with the instruction's line number. i.e. (DEBUG:<line number>)> <instruction>

Your debugging output should look something like this:

./riscv_interpreter -d < test.txt
(DEBUG:1)> addi x1,x0,12
(DEBUG:2)> addi x2, x0, 0x11
r[0] = 0x0
r[1] = 0xc
r[2] = 0x11
r[3] = 0x0
r[4] = 0x0
...
Starting values. It is possible to initialize a register with a 32-bit value for testing. The special comment directive ## start[<r>] = <value> can be placed anywhere in your test program to set the value in register <r> to <value>. For example, ## start[4] = 0x123 will set the value of register 4 to 0x123.

Try writing some sample programs to verify the correctness of your RISCV Interpreter! :)

Notes & Hints
Read the docs. Refer to the RISCV manual often.

What to do first. We suggest that you implement all required functions in the same order as the sections appear in this writeup. We recommend attempting to simulate the memory using an associative linked list first, before attempting to simulate the memory with a hash table.

Modifications. You only need to modify linkedlist.c, hashtable.c, riscv.c. Modification of any other files is not permitted. For example, you may not add another struct definition to riscv.h.

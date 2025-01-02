#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "linkedlist.h"
#include "hashtable.h"
#include "riscv.h"

/************** BEGIN HELPER FUNCTIONS PROVIDED FOR CONVENIENCE ***************/
const int R_TYPE = 0;
const int I_TYPE = 1;
const int MEM_TYPE = 2;
const int U_TYPE = 3;
const int UNKNOWN_TYPE = 4;

/**
 * Return the type of instruction for the given operation
 * Available options are R_TYPE, I_TYPE, MEM_TYPE, UNKNOWN_TYPE
 */
static int get_op_type(char *op) // array of char -> string in c++
{
    const char *r_type_op[] = {"add", "sub", "and", "or", "xor", "nor", "slt", "sll", "sra"};
    const char *i_type_op[] = {"addi", "andi", "ori", "xori", "slti"};
    const char *mem_type_op[] = {"lw", "lb", "sw", "sb"};
    const char *u_type_op[] = {"lui"};
    for (int i = 0; i < (int)(sizeof(r_type_op) / sizeof(char *)); i++)
    {
        if (strcmp(r_type_op[i], op) == 0)
        {
            return R_TYPE;
        }
    }
    for (int i = 0; i < (int)(sizeof(i_type_op) / sizeof(char *)); i++)
    {
        if (strcmp(i_type_op[i], op) == 0)
        {
            return I_TYPE;
        }
    }
    for (int i = 0; i < (int)(sizeof(mem_type_op) / sizeof(char *)); i++)
    {
        if (strcmp(mem_type_op[i], op) == 0)
        {
            return MEM_TYPE;
        }
    }
    for (int i = 0; i < (int)(sizeof(u_type_op) / sizeof(char *)); i++)
    {
        if (strcmp(u_type_op[i], op) == 0)
        {
            return U_TYPE;
        }
    }
    return UNKNOWN_TYPE;
}
/*************** END HELPER FUNCTIONS PROVIDED FOR CONVENIENCE ****************/

registers_t *registers;
// TODO: create any additional variables to store the state of the interpreter
hashtable_t *myTable; // create a hashtable to store the data in the memory

void init(registers_t *starting_registers)
{
    registers = starting_registers;
    // TODO: initialize any additional variables needed for state
    myTable = ht_init(8192);
    registers->r[0] = 0; // x0 = 0
}

enum operation
{
    ADD,
    SUB,
    AND,
    OR,
    XOR,
    NOR,
    SLT,
    SLL,
    SRA, // R-type instructions
    ADDI,
    ANDI,
    ORI,
    XORI,
    SLTI, // I-type instructions
    LW,
    LB,
    SW,
    SB,     // Mem instructions
    LUI,    // LUI instruction
    UNKNOWN // Unknown type
};

// Array of instruction strings
const char *operation_names[] = {
    "add", "sub", "and", "or", "xor", "nor", "slt", "sll", "sra", // R-type
    "addi", "andi", "ori", "xori", "slti",                        // I-type
    "lw", "lb", "sw", "sb",                                       // D-type
    "lui"                                                         // LUI
};

// TODO: create any necessary helper functions
// Function to map instruction name to enum
enum operation op_type(const char *op)
{
    for (int i = 0; i < 19; i++)
    {
        if (strcmp(op, operation_names[i]) == 0)
        {
            return (enum operation)i; // Return the corresponding enum based on the index
        }
    }
    return UNKNOWN; // If no match is found, return UNKNOWN
}

char *strsep(char **stringp, const char *delim)
{
    if (!stringp || !*stringp)
        return NULL;

    char *start = *stringp;
    char *end = strpbrk(start, delim);

    if (end)
    {
        *end = '\0';        // Replace delimiter with null terminator
        *stringp = end + 1; // Move pointer to the next character after delimiter
    }
    else
    {
        *stringp = NULL; // No more tokens
    }

    return start;
}

int converter(char *s)
{
    // case when s is x0, x1, x2, ..., x31
    if (s[0] == 'x')
        return atoi(s + 1);
    // case when s is a hexadecimal number
    if (s[1] == 'x' || s[0] == '0') // case 0x - hexadecimal number
        return (int)strtol(s, NULL, 0);
    return atoi(s);
}

void get_RIM_Type_Parameter(char *instruction, int *rd, int *rs1, int *rs2) // get parameter for R, I, and Mem type
{
    char *temp = strsep(&instruction, " ");
    *rd = converter(temp);
    temp = strsep(&instruction, " ");
    *rs1 = converter(temp);
    temp = strsep(&instruction, " ");
    *rs2 = converter(temp);
}

void Execute_R_Type(char *op, char *instruction, registers_t *registers)
{
    // ADD, SUB, AND, OR, XOR, NOR, SLT, SLL, SRA
    int rd, rs1, rs2;
    get_RIM_Type_Parameter(instruction, &rd, &rs1, &rs2);
    // fprintf(stdout, rd, rs1, rs2);
    switch (op_type(op))
    {
    case ADD:
        registers->r[rd] = registers->r[rs1] + registers->r[rs2];
        break;
    case SUB:
        registers->r[rd] = registers->r[rs1] - registers->r[rs2];
        break;
    case AND:
        registers->r[rd] = registers->r[rs1] & registers->r[rs2];
        break;
    case OR:
        registers->r[rd] = registers->r[rs1] | registers->r[rs2];
        break;
    case XOR:
        registers->r[rd] = registers->r[rs1] ^ registers->r[rs2];
        break;
    case NOR:
        registers->r[rd] = ~(registers->r[rs1] | registers->r[rs2]);
        break;
    case SLT:
        registers->r[rd] = (registers->r[rs1] < registers->r[rs2]) ? 1 : 0;
        break;
    case SLL:
        registers->r[rd] = registers->r[rs1] << registers->r[rs2];
        break;
    case SRA:
        registers->r[rd] = registers->r[rs1] >> registers->r[rs2];
        break;
    default:
        break;
    }
}

// I type and Mem Type need to check more
void Execute_I_Type(char *op, char *instruction, registers_t *registers)
{
    //	ADDI, ANDI, ORI, XORI, SLTI
    int rd, rs1, imm;
    get_RIM_Type_Parameter(instruction, &rd, &rs1, &imm);
    // sign extend imm
    if (imm & 0x800) // if msb is 1
        imm |= 0xFFFFF000;

    switch (op_type(op))
    {
    case ADDI:
        registers->r[rd] = registers->r[rs1] + imm;
        break;
    case ANDI:
        registers->r[rd] = registers->r[rs1] & imm;
        break;
    case ORI:
        registers->r[rd] = registers->r[rs1] | imm;
        break;
    case XORI:
        registers->r[rd] = registers->r[rs1] ^ imm;
        break;
    case SLTI:
        registers->r[rd] = (registers->r[rs1] < imm) ? 1 : 0;
        break;
    default:
        break;
    }
}

void Execute_MEM_Type(char *op, char *instruction, registers_t *registers)
{
    // LW, LB, SW, SB
    int rd, imm, rs1;
    get_RIM_Type_Parameter(instruction, &rd, &imm, &rs1);
    switch (op_type(op))
    {
    case LW:
        registers->r[rd] = 0;
        for (int t = 3; t >= 0; t--)
            registers->r[rd] = (registers->r[rd] << 8) | ht_get(myTable, registers->r[rs1] + imm + t);
        break;
    case LB:
        registers->r[rd] = ht_get(myTable, registers->r[rs1] + imm);
        if (registers->r[rd] & 0x80)        // if msb is 1
            registers->r[rd] |= 0xFFFFFF00; // sign extend
        break;
    case SW:
        for (int t = 0; t < 4; t++)
            ht_add(myTable, registers->r[rs1] + imm + t, (registers->r[rd] >> (8 * t)) & 0xFF);
        break;
    case SB:
        ht_add(myTable, registers->r[rs1] + imm, registers->r[rd] & 0xFF);
        break;
    default:
        break;
    }
}

void get_U_Type_Parameter(char *instruction, int *rd, int *imm) // get parameter for U Type
{
    char *temp = strsep(&instruction, " ");
    *rd = converter(temp);
    temp = strsep(&instruction, " ");
    *imm = converter(temp);
}

void Execute_U_Type(char *op, char *instruction, registers_t *registers)
{
    // 	LUI
    int rd, imm;
    get_U_Type_Parameter(instruction, &rd, &imm);
    registers->r[rd] = imm << 12;
}

void standardlization(char *instruction)
{
    // replace "(,)" by " "
    for (int i = 0; instruction[i]; i++)
    {
        if (instruction[i] == ',' || instruction[i] == '(' || instruction[i] == ')')
            instruction[i] = ' ';
    }
    // remove all invalid space
    int oldP = 0, newP = 0, check = 0;
    while (instruction[oldP] != '\0')
    {
        if (instruction[oldP] != ' ')
        {
            if (check == 1 && newP > 0)
                instruction[newP++] = ' ';
            instruction[newP++] = instruction[oldP];
            check = 0;
        }
        else
            check = 1;
        oldP++;
    }
    instruction[newP] = '\0';
}

void step(char *instruction)
{
    // Extracts and returns the substring before the first space character,
    // by replacing the space character with a null-terminator.
    // `instruction` now points to the next character after the space
    // See `man strsep` for how this library function works
    char *op = strsep(&instruction, " ");
    // Uses the provided helper function to determine the type of instruction
    int op_type = get_op_type(op);
    // Skip this instruction if it is not in our supported set of instructions
    if (op_type == UNKNOWN_TYPE)
    {
        return;
    }

    // TODO: write logic for evaluating instruction on current interpreter state
    standardlization(instruction);
    if (op_type == R_TYPE)
        Execute_R_Type(op, instruction, registers);
    if (op_type == I_TYPE)
        Execute_I_Type(op, instruction, registers);
    if (op_type == MEM_TYPE)
        Execute_MEM_Type(op, instruction, registers);
    if (op_type == U_TYPE)
        Execute_U_Type(op, instruction, registers);
}

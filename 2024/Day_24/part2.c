//######################################################################################################################

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

//######################################################################################################################
// type defines

typedef enum Type Type;
enum Type
{
    AND=0,
    XOR,
    OR
};

typedef struct Gate Gate;
struct Gate
{
    Type type;
    int8_t done;
    uint32_t input[2];
    uint32_t output;
};

typedef struct Register Register;
struct Register
{
    uint32_t id;
    int8_t value;
};

//######################################################################################################################

static int32_t get_registers(char* file_name, Register* registers, int32_t registers_size)
{
    FILE* file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Failed to open file!\n");
        return -1;
    }

    char ids[4] = {0};
    int32_t value = 0;
    int32_t registers_count = 0;
    while (fscanf(file, "%3s: %d\n", ids, &value) == 2) {
        Register* reg = &registers[registers_count];
        reg->id = *(uint32_t*)ids;
        reg->value = value;
        registers_count++;
        if (registers_count == registers_size) {
            printf("Register array full!\n");
            fclose(file);
            return -1;
        }
    }

    fclose(file);
    return registers_count;
}

static int32_t get_gates(char* file_name, Gate* gates, int32_t gates_size)
{
    FILE* file = fopen(file_name, "r");
    if (file == NULL) {
        printf("Failed to open file!\n");
        return -1;
    }

    char buffer[64] = {0};
    while (fgets(buffer, 64, file)) {
        if (buffer[0] == '\n')
            break;
    }

    char id_input_0[4] = {0};
    char id_input_1[4] = {0};
    char id_output[4] = {0};
    char gate_type[5] = {0};
    int32_t gate_count = 0;
    while (fscanf(file, "%3s %s %3s -> %3s\n", id_input_0, gate_type, id_input_1, id_output) == 4) {
        Gate* gate = &gates[gate_count];
        if (gate_type[0] == 'A') {
            gate->type = AND;
        } else if (gate_type[0] == 'O') {
            gate->type = OR;
        } else if (gate_type[0] == 'X') {
            gate->type = XOR;
        }
        gate->input[0] = *(uint32_t*)id_input_0;
        gate->input[1] = *(uint32_t*)id_input_1;
        gate->output = *(uint32_t*)id_output;
        gate->done = 0;
        gate_count++;
        if (gate_count == gates_size) {
            printf("Gate array full!\n");
            fclose(file);
            return -1;
        }
    }

    fclose(file);
    return gate_count;
}

static int32_t get_regval(int32_t id, Register* registers, int32_t registers_count)
{
    for (int32_t i=0; i<registers_count; i++) {
        Register* reg = &registers[i];
        if (reg->id != id)
            continue;
        return reg->value;
    }
    return -1;
}

static int8_t gate_xor(int8_t input_0, int8_t input_1)
{
    return input_0 ^ input_1;
}

static int8_t gate_and(int8_t input_0, int8_t input_1)
{
    return input_0 & input_1;
}

static int8_t gate_or(int8_t input_0, int8_t input_1)
{
    return input_0 | input_1;
}

int8_t(*gate_func[])(int8_t, int8_t) = {
    gate_and,
    gate_xor,
    gate_or,
};

static int32_t process_gates(Register* registers, int32_t registers_count, int32_t registers_size,
    Gate* gates, int32_t gates_count)
{
    int8_t done = 0;
    while (done == 0) {
        done = 1;
        for (int32_t i=0; i<gates_count; i++) {
            Gate* gate = &gates[i];
            if (gate->done == 1)
                continue;
            done = 0;
            int8_t input_0 = get_regval(gate->input[0], registers, registers_count);
            int8_t input_1 = get_regval(gate->input[1], registers, registers_count);
            if (input_0 == -1 || input_1 == -1)
                continue;
            int8_t output = gate_func[gate->type](input_0, input_1);
            registers[registers_count].id = gate->output;
            registers[registers_count].value = output;
            registers_count++;
            if (registers_count == registers_size) {
                printf("Register array full!\n");
                return 0;
            }
            gate->done = 1;
        }
    }
    return registers_count;
}

static Register* get_reg(uint32_t id, Register* registers, int32_t registers_count)
{
    for (int32_t i=0; i<registers_count; i++) {
        Register* reg = &registers[i];
        if (reg->id != id)
            continue;
        return reg;
    }
    return NULL;
}

static void run_gates(Register* registers, int32_t registers_count, Gate* gates, int32_t gates_count)
{
    int8_t done = 0;
    while (done == 0) {
        done = 1;
        for (int32_t i=0; i<gates_count; i++) {
            Gate* gate = &gates[i];
            if (gate->done == 1)
                continue;
            done = 0;
            int8_t input_0 = get_regval(gate->input[0], registers, registers_count);
            int8_t input_1 = get_regval(gate->input[1], registers, registers_count);
            if (input_0 == -1 || input_1 == -1)
                continue;
            int8_t output = gate_func[gate->type](input_0, input_1);
            Register* reg = get_reg(gate->output, registers, registers_count);
            reg->value = output;
            gate->done = 1;
        }
    }
}

static Gate* get_gate(uint32_t id, Gate* gates, int32_t gates_count)
{
    for (int32_t i=0; i<gates_count; i++) {
        Gate* gate = &gates[i];
        if (gate->output == id)
            return gate;
    }
    return NULL;
}

static Gate* get_gate_input(uint32_t id, Gate* gates, int32_t gates_count)
{
    for (int32_t i=0; i<gates_count; i++) {
        Gate* gate = &gates[i];
        if (gate->input[0] == id || gate->input[1] == id)
            return gate;
    }
    return NULL;
}

static int32_t get_gate_input_count(uint32_t id, Gate* gates, int32_t gates_count)
{
    int32_t count = 0;
    for (int32_t i=0; i<gates_count; i++) {
        Gate* gate = &gates[i];
        if (gate->input[0] == id || gate->input[1] == id)
            count++;
    }
    return count;
}

static void sort_regs(Register** regs, int32_t regs_count)
{
    int8_t sorted = 0;
    while (sorted == 0) {
        sorted = 1;
        for (int32_t i=0; i<regs_count-1; i++) {
            Register* reg_0 = regs[i];
            Register* reg_1 = regs[i + 1];
            int32_t regval_0 = ((reg_0->id & 0x0000FF00)<<8) | ((reg_0->id & 0x00FF0000)>>8);
            int32_t regval_1 = ((reg_1->id & 0x0000FF00)<<8) | ((reg_1->id & 0x00FF0000)>>8);
            if (regval_0 <= regval_1)
                continue;
            sorted = 0;
            Register* temp = reg_0;
            reg_0 = reg_1;
            reg_1 = temp;
            regs[i] = reg_0;
            regs[i + 1] = reg_1;
        }
    }
}

// static int32_t find_faulty_output(Register* reg, Gate* gates, int32_t gates_count, Register** faulty, int32_t faulty_count)
// {
//     Gate* gate = get_gate(reg->id, gates, gates_count);
//     if (gate->type != XOR) {
//         char* reg_id = (char*)&reg->id;
//         if (!memcmp(reg_id, "z45", 3))
//             return faulty_count;
//         faulty[faulty_count] = reg;
//         faulty_count++;
//         printf("Faulty: %s\n", reg_id);
//     }
//     return faulty_count;
// }

// static int32_t find_faulty_and(Register* regs, int32_t regs_count, Gate* gates, int32_t gates_count, Register** faulty, int32_t faulty_count)
// {
//     for (int32_t i=0; i<gates_count; i++) {
//         Gate* gate = &gates[i];
//         if (gate->type != AND)
//             continue;

//         Gate* gate_input = get_gate_input(gate->output, gates, gates_count);
//         int32_t input_count = get_gate_input_count(gate->output, gates, gates_count);
//         if (input_count > 1) {
//             faulty[faulty_count] = get_reg(gate->output, regs, regs_count);
//             printf("Faulty: %s\n", faulty[faulty_count]);
//             faulty_count++;
//             continue;
//         } else if (gate_input == NULL) {
//             continue;
//         } else if (gate_input->type == OR) {
//             continue;
//         }

//         faulty[faulty_count] = get_reg(gate->output, regs, regs_count);
//         printf("Faulty: %s\n", faulty[faulty_count]);
//         faulty_count++;
//     }
//     return faulty_count;
// }

// static int32_t find_faulty_reg(Register* regs, int32_t regs_count, Gate* gates, int32_t gates_count, Register** faulty, int32_t faulty_count)
// {
//     for (int32_t i=0; i<regs_count; i++) {
//         Register* reg = &regs[i];
//         if (((char*)&reg->id)[0] == 'z')
//             continue;
//         Gate* gate_input = get_gate_input(reg->id, gates, gates_count);
//         if (gate_input != NULL)
//             continue;
//         faulty[faulty_count] = reg;
//         printf("Faulty: %s\n", faulty[faulty_count]->id);
//         faulty_count++;
//     }
//     return faulty_count;
// }

// static int32_t find_faulty_or(Register* regs, int32_t regs_count, Gate* gates, int32_t gates_count, Register** faulty, int32_t faulty_count)
// {
//     for (int32_t i=0; i<gates_count; i++) {
//         Gate* gate = &gates[i];
//         if (gate->type != OR)
//             continue;

//         Gate* gate_input = get_gate_input(gate->output, gates, gates_count);
//         int32_t input_count = get_gate_input_count(gate->output, gates, gates_count);
//         if (input_count == 1) {
//             char* reg_id = (char*)&gate->output;
//             if (reg_id[0] == 'z')
//                 continue;
//             faulty[faulty_count] = get_reg(gate->output, regs, regs_count);
//             printf("Faulty: %s\n", faulty[faulty_count]);
//             faulty_count++;
//         }
//     }
//     return faulty_count;
// }

static char* get_id(uint32_t* id)
{
    return (char*)id;
}

static void visalize_first(Register* current, Register* reg, int32_t regs_count, Gate* gates, int32_t gates_count)
{
    char* input_x;
    char* input_y;
    char* output;
    char* carry;

    // get input x
    input_x = get_id(&current->id);

    // get gates
    Gate* xor;
    Gate* and;
    for (int32_t i=0; i<gates_count; i++) {
        Gate* gate = &gates[i];
        if (gate->input[0] != current->id && gate->input[1] != current->id)
            continue;
        if (gate->type == XOR)
            xor = gate;
        else
            and = gate;
    }

    Register* output_reg = get_reg(xor->output, reg, regs_count);
    output = get_id(&xor->output);
    
    // get input y
    uint32_t input_y_id = xor->input[0];
    if (xor->input[0] == current->id)
        input_y_id = xor->input[1];
    input_y = get_id(&input_y_id);

    // get carry
    uint32_t carry_id = and->output;
    carry = get_id(&carry_id);

    printf("%s──┬─────┐       \n", input_x);
    printf("     │    XOR──%s \n", output);
    printf("%s────┬───┘       \n", input_y);
    printf("     │ │          \n");
    printf("     │ └───┐      \n");
    printf("     │    AND──%s \n", carry);
    printf("     └─────┘      \n");
}

static void visalize_second(Register* current, Register* reg, int32_t regs_count, Gate* gates, int32_t gates_count)
{
    char* input_x;
    char* input_y;
    char* output;
    char* carry_in;
    char* carry_out;

    // get input x
    input_x = get_id(&current->id);

    // get gates
    Gate* xor = NULL;
    Gate* and = NULL;
    for (int32_t i=0; i<gates_count; i++) {
        Gate* gate = &gates[i];
        if (gate->input[0] != current->id && gate->input[1] != current->id)
            continue;
        if (gate->type == XOR)
            xor = gate;
        else
            and = gate;
    }
    Gate* xor2 = NULL;
    Gate* and2 = NULL;
    for (int32_t i=0; i<gates_count; i++) {
        Gate* gate = &gates[i];
        if (gate->input[0] != xor->output && gate->input[1] != xor->output)
            continue;
        if (gate->type == XOR)
            xor2 = gate;
        else if (gate->type == AND)
            and2 = gate;
    }

    if(xor2 == NULL) {
        printf("XOR2 NULL\n");
    }
    if(and2 == NULL) {
        printf("AND2 NULL\n");
    }

    if (get_gate_input(and2->output, gates, gates_count) != get_gate_input(and->output, gates, gates_count)) {
        printf("Found: %s, %s\n", get_id(&and2->output), get_id(&and->output));
    }
    
    // get output
    output = get_id(&xor2->output);
    if (output[0] != 'z') {
        printf("Found: %s\n", output);
    }
    
    // get input y
    uint32_t input_y_id = xor->input[0];
    if (xor->input[0] == current->id)
        input_y_id = xor->input[1];
    input_y = get_id(&input_y_id);

    // get carry in
    uint32_t carry_in_in = xor2->input[0];
    if (xor2->input[0] == xor->output)
        carry_in_in = xor2->input[1];
    carry_in = get_id(&carry_in_in);

    // get carry out
    Gate* or = get_gate_input(and->output, gates, gates_count);
    if (or == NULL) {
        printf("Found: %s\n", get_id(&and->output));
        carry_out = "###";
    } else if    (get_id(&or->output)[0] == 'z') {
        printf("Found: %s\n", get_id(&or->output));
        carry_out = get_id(&or->output);
    } else {
        carry_out = get_id(&or->output);
    }
    
    printf("────────────────────────────────────────────────────────────────────────────────────────────\n");
    printf("\n%s──┬─────┐                      \n", input_x);
    printf("     │    XOR[%s]┬────┐          \n", get_id(&xor->output));
    printf("%s────┬───┘      │   XOR──────%s  \n", input_y, output);
    printf("     │ │          │    │              \n");
    printf("%s──────┬─────────────┘               \n", carry_in);
    printf("     │ │ │        │              \n");
    printf("     │ │ │        │          \n");
    printf("     │ │ │       AND[%s]──┐      \n", get_id(&and2->output));
    printf("     │ │ └────────┘        OR──%s \n", carry_out);
    printf("     │ └──────────┐        │          \n");
    printf("     │           AND[%s]──┘      \n", get_id(&and->output));
    printf("     └────────────┘              \n");
}

static int64_t get_answer(Register* registers, int32_t registers_count, Gate* gates, int32_t gates_count)
{
    // get all registers
    Register* xregs[64];
    Register* yregs[64];
    Register* zregs[64];
    int32_t xregs_count = 0;
    int32_t yregs_count = 0;
    int32_t zregs_count = 0;
    for (int32_t i=0; i<registers_count; i++) {
        Register* reg = &registers[i];
        char* reg_id = (char*)&reg->id;
        if (reg_id[0] == 'x') {
            xregs[xregs_count] = reg;
            xregs_count++;
        } else if (reg_id[0] == 'y') {
            yregs[yregs_count] = reg;
            yregs_count++;
        } else if (reg_id[0] == 'z') {
            zregs[zregs_count] = reg;
            zregs_count++;
        }

    }

    sort_regs(xregs, xregs_count);
    sort_regs(yregs, yregs_count);
    sort_regs(zregs, zregs_count);

    // Register* faulty[2000];
    // int32_t faulty_count = 0;
    // for (int32_t i=0; i<zregs_count; i++) {
    //     Register* output = zregs[i];
    //     faulty_count = find_faulty_output(output, gates, gates_count, faulty, faulty_count);
    // }
    // faulty_count = find_faulty_and(registers, registers_count, gates, gates_count, faulty, faulty_count);
    // faulty_count = find_faulty_or(registers, registers_count, gates, gates_count, faulty, faulty_count);
    // faulty_count = find_faulty_reg(registers, registers_count, gates, gates_count, faulty, faulty_count);
    // printf("Faulty total: %d\n", faulty_count);

    visalize_first(xregs[0], registers, registers_count, gates, gates_count);
    for (int32_t i=1; i<xregs_count; i++) {
        visalize_second(xregs[i], registers, registers_count, gates, gates_count);
    }


    
    return 0;
}

int solve_puzzle(char* file_name)
{
    Register registers[2000];
    int32_t registers_size = sizeof(registers) / sizeof(Register);
    int32_t registers_count = get_registers(file_name, registers, registers_size);
    if (registers_count == -1) {
        printf("Failed to get registers!\n");
        return 1;
    }

    printf("Registers: %d\n", registers_count);

    Gate gates[2000];
    int32_t gates_size = sizeof(gates) / sizeof(Gate);
    int32_t gates_count = get_gates(file_name, gates, gates_size);
    if (registers_count == -1) {
        printf("Failed to get gates!\n");
        return 1;
    }

    printf("Gates: %d\n", gates_count);

    registers_count = process_gates(registers, registers_count, registers_size, gates, gates_count);
    int64_t answer = get_answer(registers, registers_count, gates, gates_count);

    printf("Answer: %ld\n", answer);
}

//######################################################################################################################

int main(int argc, char* argv[])
{
    if (argc == 1) {
        printf("Please add the file name with the exeutable!\n");
        return 1;
    }
    char* file_name = argv[1];
    solve_puzzle(file_name);
    return 0;
}
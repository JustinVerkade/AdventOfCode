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

static int64_t get_answer(Register* registers, int32_t registers_count)
{
    Register zregs[2000];
    int32_t zregs_size = sizeof(zregs) / sizeof(Register);
    int32_t zregs_count = 0;
    for (int32_t i=0; i<registers_count; i++) {
        Register* reg = &registers[i];
        char* reg_id = (char*)&reg->id;
        if (reg_id[0] != 'z')
            continue;
        zregs[zregs_count].id = reg->id;
        zregs[zregs_count].value = reg->value;
        zregs_count++;
        if (zregs_count == zregs_size) {
            printf("Z register array full!\n");
            return 0;
        }
    }

    int8_t sorted = 0;
    while (sorted == 0) {
        sorted = 1;
        for (int32_t i=0; i<zregs_count-1; i++) {
            Register reg_0 = zregs[i];
            Register reg_1 = zregs[i + 1];
            int32_t regval_0 = ((reg_0.id & 0x0000FF00)<<8) | ((reg_0.id & 0x00FF0000)>>8);
            int32_t regval_1 = ((reg_1.id & 0x0000FF00)<<8) | ((reg_1.id & 0x00FF0000)>>8);
            if (regval_0 <= regval_1)
                continue;
            sorted = 0;
            Register temp = reg_0;
            reg_0 = reg_1;
            reg_1 = temp;
            zregs[i] = reg_0;
            zregs[i + 1] = reg_1;
        }
    }

    int64_t answer = 0x0000000000000000;
    for (int32_t i=0; i<zregs_count; i++) {
        Register* zreg = &zregs[i];
        printf("Reg %s: %d\n", (char*)&zreg->id, zreg->value);
        answer |= ((int64_t)(zreg->value == 1)<<i);
    }

    return answer;
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
    int64_t answer = get_answer(registers, registers_count);

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
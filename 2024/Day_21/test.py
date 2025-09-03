command_0 = "<v<A>>^AvA^A<vA<AA>>^AAvA<^A>AAvA^A<vA>^AA<A>A<v<A>A>^AAAvA<^A>A"
command_1 = "v<<A>>^AvA^Av<<A>>^AAv<A<A>>^AAvAA<^A>Av<A>^AA<A>Av<A<A>>^AAAvA<^A>A"

keypad = {
    (0, 0):'7', (1, 0):'8', (2, 0):'9',
    (0, 1):'4', (1, 1):'5', (2, 1):'6',
    (0, 2):'1', (1, 2):'2', (2, 2):'3',
    (1, 3):'0', (2, 3):'A'}

control = {
    (1, 0):'^', (2, 0):'A',
    (0, 1):'<', (1, 1):'v', (2, 1):'>'}

def simulateKeypad(commands:str):
    pos_x = 2
    pos_y = 3
    output = ""
    for cmd in commands:
        if cmd == '^':
            pos_y -= 1
        elif cmd == 'v':
            pos_y += 1
        elif cmd == '<':
            pos_x -= 1
        elif cmd == '>':
            pos_x += 1
        elif cmd == 'A':
            output += keypad[(pos_x, pos_y)]
    return output

def simulateControls(commands:str):
    pos_x = 2
    pos_y = 0
    output = ""
    for cmd in commands:
        if cmd == '^':
            pos_y -= 1
        elif cmd == 'v':
            pos_y += 1
        elif cmd == '<':
            pos_x -= 1
        elif cmd == '>':
            pos_x += 1
        elif cmd == 'A':
            output += control[(pos_x, pos_y)]
    return output

string = simulateControls("v<<A>>^Av<A<A>>^AAvAA<^A>Av<<A>>^AAvA^Av<A>^AA<A>Av<A<A>>^AAAvA<^A>A")
print(string)
string = simulateControls(string)
print(string)
print(simulateKeypad(string))
import json

count_red = 0
def sumObject(obj):
    global count_red
    if type(obj) is int:
        return obj
    
    if type(obj) is list:
        return sum(map(sumObject, obj))
    
    if type(obj) is dict:
        vals = obj.values()
        
        # remove these two lines for part one
        if "red" in vals:
            count_red += 1
            return 0
        
        return sum(map(sumObject, vals))
    
    else:
        return 0


with open("src\\data.txt") as f:
    obj = json.loads(f.read())
    print(sumObject(obj))
    print(count_red)
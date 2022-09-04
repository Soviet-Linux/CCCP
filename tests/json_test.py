import json

o = json.load(open(input("what file do you want to test ? ")))

print(json.dumps(o,indent=4))
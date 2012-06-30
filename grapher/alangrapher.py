from sys import argv
from subprocess import check_call, call, Popen, PIPE

script, filename = argv

process = Popen(["alan", "-dump", "ci", filename], stdout=PIPE)

output = process.communicate(None)[0]
lines = ''.join(output).split("\n")

i = iter(lines)
line = i.next()

l = 0
while line.find("ADV:") == -1 :
    line = i.next()

try :
    while 1 :
        while line.find("INS:") == -1 :
            line = i.next()

        while line.find("id:") == -1 :
            line = i.next()
        idline = line
        line = i.next()

        # Found an instance. Is it a direct decendant of "location"?
        # We can't handle indirect decendants yet
        while line.find("parentId:") == -1 :
            line = i.next()
        if line.find("location") == -1 :
            continue;

        # Yes, that was a location. What's it's name?
        # 8th field and then remove the part after colon
        id = idline.split()[7].split(":")[0]

        # Is there an exit?
        line = i.next()
        while 1 :
            while line.find("DIRECTION") == -1 and line.find("INS:") == -1 :
                line = i.next()
            if line.find("INS:") != -1 :
                # No, found next instancs, so start over
                break

            # Yes, so find the direction
            # 9th field and then remove the part after colon
            direction = line.split()[11].split(":")[0]

            # So, to where does it lead?
            line = i.next()
            target = line
            line = i.next()

            if line[0] != '.' :
                print target
                print line
                target += line.split()[0]
                print target
            target = target.split()[11].split(":")[0]
            
            print id + ":" + direction + ":" + target

except StopIteration as x :
    pass


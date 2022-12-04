#open file

f = open('dict.txt', 'r')
#read file
data = f.read()
#split file into lines
lines = data.split("\n")
lines.sort()
lines.sort(key=len, reverse=False)
#write to file
f = open('dict.txt', 'w')
f.write("\n".join(lines))
f.close()

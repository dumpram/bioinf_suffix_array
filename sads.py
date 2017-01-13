import sys

##getting the name of the file
if len(sys.argv)<>2:
    txtfile = raw_input('File not given. Please provide a file: ')
else:
    txtfile = sys.argv[1]

##reading the file
f = open(txtfile, 'r')
header = f.readline().strip()
if header[0]=='>' or header[0]==';':
    ##fasta format
    s = f.readline()
    while s[0]==';':
        s = f.readline().strip()
else:
    ##regular string
    s = header
s += ''.join(f.read().split())
f.close()

## s - input string
## sa - output suffix array
## t - array od L- and S- types
lens = len(s)
s+='$'  #end of the string


## Step 1) Scan s once to classify all characters as L-/S- type into t
## L-type => 0,  S-type => 1
t = [0]*(lens+1)
t[lens] = 1   #the last character is S-type
for i in range(lens-1, -1, -1):
    if s[i] < s[i+1]:
        t[i] = 1
    elif s[i] > s[i+1]:
        t[i] = 0
    else:
        t[i] = t[i+1]
        
##Step 2) Scan t once to find d-critical substrings into p1
p1 = []
d = 2
for i in range(1,len(t)):
    ##if d-critical append to p1 
    if (t[i] == 1 and t[i-1] == 0) or ((i-d) in p1 and not (t[i+1] == 1 and t[i] == 0)
        and not any(x in p1 for x in range(i-d+1,i-1))):   
        p1.append(i)

##Step 3) Bucket sort d-critical substrings using p1 and b
b = []
    

print s
print str(t)
print p1
    


        




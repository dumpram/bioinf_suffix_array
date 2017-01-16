"""
This is the implementation of Nong-Zhang-Chan SA-DS algorithm.

This algorithm calculates suffix array from character sequence.
"""

__version__ = '0.1'
__author__ = 'Jasna Jankovic'

import sys


def sads(s):
    ## s - the input string
    ## sa - the output suffix array of s
    ## t - array of L- and S- types
    
    ## Step 1) Scan s once to classify all the characters as L-/S-type into t
    ## L-type => 0,  S-type => 1
    lens = len(s)-1
    alphDict = dict()                      
    alphDict[s[lens]] = 1
    #all characters set to 0 (L-type)
    t = [0]*(lens+1)
    #the last character is S-type
    t[lens] = 1   
    for i in range(lens-1, -1, -1):
        if s[i] < s[i+1]:
            t[i] = 1
        elif s[i] == s[i+1]:
            t[i] = t[i+1]
        if s[i] in alphDict:               
            alphDict[s[i]] += 1
        else:
            alphDict[s[i]] = 1
        
    ##Step 2) Scan t once to find all the d-critical substrings in s into p1
    p1 = []
    i = 1
    while i < len(t):
        ##if d-critical => append to p1 
        if (t[i] == 1 and t[i-1] == 0) or ((i-d) in p1 and not (t[i+1] == 1
            and t[i] == 0) and not any(x in p1 for x in range(i-d+1,i-1))):   
            p1.append(i)
            i += 1
        i += 1

    ##Step 3) Bucket sort all the d-critical substrings
    ##        using p1, bStart and bEnd       
    alphSorted = sorted(alphDict.keys())

    ##d-critical substrings
    subs = []
    for x in p1:
        sub = ""
        for j in range(d+2):
            sub += str(s[min(x+j, lens)])
        sub += str(t[min(x+d+1, lens)])
        subs.append(sub)

    subsAlph = sorted(set(subs))
    pairs = list((p1[i], subs[i]) for i in range(len(p1)))

    p2 = {}
    p3 = sorted(pairs, key = lambda pair: pair[1][d+1:d+3])
    for i in range(d, -1, -1):
        p2 = p3
        p3 = sorted(p2, key = lambda pair: pair[1][i:d+3])

    ##Step 4) Name each d-critical substring in S by its bucket
    ##        index to get a new shortened string S1
    ##asigning number of bucket to every substring pointer (p1 element)     
    p4 = []
    p4.append((p3[0][0], 0))
    j = 0
    for i in range(1, len(p3)):
        if p3[i][1] <> p3[i-1][1]:
            j += 1
        p4.append((p3[i][0], j))
    ##sorting substring pointers (p1 elements)
    s1 = sorted(p4, key = lambda pair: pair[0])
    ##s1 is list of buckets for every substring pointer
    s1 = list(x[1] for x in s1)

    sa1 = [0]*len(s1)
    ##Step 5) If size(s1) = number of buckets
    if len(s1) == len(subsAlph):
        ##directly cumpute sa1 from s1
        for i in range(len(sa1)):
            sa1[s1[i]] = i
    else:
        ##fire a recursive call
        sa1 = sads(s1)

    ##Step 6) Induce sa from sa1
    sa = [-1] * (lens+1)
    bEnd = dict()
    bEnd[alphSorted[0]] = 0
    bStart = dict()
    bStart[alphSorted[0]] = 0
    for i in range(1, len(alphSorted)):
        bEnd[alphSorted[i]] = alphDict[alphSorted[i]] + bEnd[alphSorted[i-1]]
        bStart[alphSorted[i]] = alphDict[alphSorted[i-1]] + bStart[alphSorted[i-1]]

    ##Induce sa algorithm: Step 1
    for i in range(len(sa1)-1,-1, -1):
        if t[p1[sa1[i]]] == 1 and t[p1[sa1[i]]-1] == 0:
            currS = s[p1[sa1[i]]]
            sa[bEnd[currS]] = p1[sa1[i]]
            bEnd[currS] -= 1

    ##Induce sa algorithm: Step 2
    for i in range(len(sa)):
        if sa[i] >= 0:
            if t[sa[i]-1] == 0:
                currS = s[sa[i]-1]
                sa[bStart[currS]] = sa[i]-1
                bStart[currS] += 1
         
    ##Induce sa algorithm: Step 3
    bEnd = dict()
    bEnd[alphSorted[0]] = 0
    for i in range(1, len(alphSorted)):
        bEnd[alphSorted[i]] = alphDict[alphSorted[i]] + bEnd[alphSorted[i-1]]

    for i in range(len(sa)-1, -1, -1):
        if sa[i] >=0:
            if sa[i] <> 0 and t[sa[i]-1] == 1:
                currS = s[sa[i]-1]
                sa[bEnd[currS]] = sa[i]-1
                bEnd[currS] -= 1

    return sa


############################################################
##getting the name of the file
if len(sys.argv)<>4:
    print("Number of arguments must be 3!")
    sys.exit(0)
    
inFile = sys.argv[2]
outFile = sys.argv[3]
d = int(sys.argv[1])

if d<2:
    print("d must be at least 2!")
    sys.exit(1)

##reading the file
f = open(inFile, 'r')
header = f.readline().strip()
if header[0] == '>' or header[0] == ';':
    ##fasta format
    s = f.readline()
    while s[0] == ';':
        s = f.readline().strip()
else:
    ##regular string
    s = header
s += ''.join(f.read().split())
f.close()

#end of the string
s += '$'  
sa = []

##starting the algorithm
sa = sads(s)

##writing sa in the file
out = open(outFile, 'w')
for x in sa:
    out.write(str(x)+'\n')
out.close()


    


        




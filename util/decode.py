#!/usr/bin/env python

import sys

def read_dict(f) :
  d = {}
  k = 1
  for a in open(f) :
    d[k] = a.strip()
    k = k + 1
  return d
  
d_e = read_dict(sys.argv[1])
d_o = read_dict(sys.argv[2])

for l in open(sys.argv[3]):
  l = l.strip()
  if l :
    o,e=map(lambda x:int(x),l.strip().split())
    print("\t".join(map(str,[d_o[o],d_e[e]])))
  else :
    print 
  


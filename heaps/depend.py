from sys import argv
from subprocess import check_output

source = argv[1]
out = check_output(['cc', '-MM', source])
_, tail = out.split(':')
print "build/"+source.replace('.cpp', '.o'), "deps/"+source.replace('.cpp', '.d'), ':', tail
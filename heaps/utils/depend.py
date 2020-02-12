from sys import argv
from subprocess import check_output

source = argv[2]
out = check_output(argv[1:] + ['-MM'])
_, tail = out.split(':')
print "build/"+source.replace('.cpp', '.o'), "deps/"+source.replace('.cpp', '.d'), ':', tail
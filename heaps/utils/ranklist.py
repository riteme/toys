import os
from os import path
from sys import argv
from hashlib import sha1
from subprocess import call
from prettytable import PrettyTable
from tempfile import mkdtemp

arg = argv[1]

assert path.exists(arg)

if arg.endswith('.tar'):
    tmp_dir = mkdtemp(prefix='tarx-', dir='/tmp')
    call(['tar', '-xf', arg, '-C', tmp_dir])
    result_dir = path.join(tmp_dir, 'results')
else:
    result_dir = arg

files = filter(lambda x: x.endswith('.txt'), os.listdir(result_dir))
entries = []

for filename in files:
    name, ext = filename.split('.')
    if ext != 'txt':
        continue

    count = 0
    total = 0

    hexdigest = sha1()
    with open(path.join(result_dir, filename), 'r') as fp:
        fp.readline()  # first line is ignored
        for line in fp:
            if line.strip() != '':
                data = line.split()
                _hash = data[1]
                time = float(data[3])

                hexdigest.update(_hash)
                total += time
                count += 1

    if count > 0:
        entries.append((name, total / count, hexdigest.hexdigest()))

entries = sorted(entries, key=lambda x: x[1])

if len(entries) > 0:
    tb = PrettyTable()
    tb.field_names = ['RANK', 'NAME', 'TIME', 'RATIO', 'HASH']
    tb.border = False
    tb.align['NAME'] = 'l'
    tb.align['TIME'] = 'l'
    tb.align['RATIO'] = 'l'
    tb.align['HASH'] = 'l'

    min_time = entries[0][1]
    for idx, (name, time, _hash) in enumerate(entries, start=1):
        ratio = time / min_time
        tb.add_row([idx, name, '%.1fms' % time, '%.2fx' % ratio, _hash[:7]])

    print tb
else:
    print '(no records)'
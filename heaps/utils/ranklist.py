from os import listdir, path
from sys import argv
from prettytable import PrettyTable

result_dir = argv[1]
files = filter(lambda x: x.endswith('.txt'), listdir(result_dir))
entries = []

for filename in files:
    name, _ = filename.split('.')

    count = 0
    total = 0
    with open(path.join(result_dir, filename), 'r') as fp:
        fp.readline()  # first line is ignored
        for line in fp:
            if line.strip() != '':
                time = float(line.split()[2])
                total += time
                count += 1

    if count > 0:
        entries.append((name, total / count))

entries = sorted(entries, key=lambda x: x[1])

if len(entries) > 0:
    tb = PrettyTable()
    tb.field_names = ['RANK', 'NAME', 'TIME', 'RATIO']
    tb.border = False
    tb.align['NAME'] = 'l'
    tb.align['TIME'] = 'l'
    tb.align['RATIO'] = 'l'

    min_time = entries[0][1]
    for idx, (name, time) in enumerate(entries, start=1):
        ratio = time / min_time
        tb.add_row([idx, name, '%.1fms' % time, '%.2fx' % ratio])

    print tb
else:
    print '(no records)'
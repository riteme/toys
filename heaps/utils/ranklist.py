from os import listdir, path
from sys import argv

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

    assert count > 0, 'No records!'
    entries.append((name, total / count))

entries = sorted(entries, key=lambda x: x[1])
min_time = entries[0][1]
for name, time in entries:
    print '%20s\t%6.1fms\t%.2lfx' % (name, time, time / min_time)
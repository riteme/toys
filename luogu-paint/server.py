#!/usr/bin/env python

import sys
import time
import logging as log
import traceback
f = open('paint.log', 'w')
f.truncate()
f.close()
log.basicConfig(filename='paint.log', level=log.INFO,
    format='%(asctime)s - %(filename)s[line:%(lineno)d] - %(levelname)s: %(message)s'
)

users = [
    "__client_id=73bac05af1959c536cc1660b7632d4b2ade85af9; UM_distinctid=167fdff59e21051-0f1b457ba9aefb-19291c0a-1fa400-167fdff59e330f; _uid=20010; CNZZDATA5476811=cnzz_eid%3D898271236-1546151986-%26ntime%3D1546162795"
    ,"__client_id=a882b43d8bca2ffa62f6d8a2ba0de97a5ee3d452; UM_distinctid=167b57379c37b7-0df532b6c56a8a-162a1c0b-100200-167b57379c42a4; _uid=47856; CNZZDATA5476811=cnzz_eid%3D812717008-1544934122-%26ntime%3D1546164337"
    ,"UM_distinctid=167b57379c37b7-0df532b6c56a8a-162a1c0b-100200-167b57379c42a4; CNZZDATA5476811=cnzz_eid%3D812717008-1544934122-%26ntime%3D1544934122; __client_id=a936317905b8535f084ba31a5447483ff710757b; _uid=85918"
    ,"UM_distinctid=166c587726221a-06a75cbcba80f2-7613374d-1fa400-166c58772633f5; __client_id=7afd2a4c51ea736c635811535a312c27695a78aa; CNZZDATA5476811=cnzz_eid%3D73098803-1540908674-%26ntime%3D1546167865; _uid=45610"
    ,"__client_id=21da84d8a70341b6014b99d2c8c988ffc0c5ea00; UM_distinctid=167fce9ac7557c-0bd292d25463be-3f674604-19fa51-167fce9ac771e2; _uid=51074; CNZZDATA5476811=cnzz_eid%3D275005417-1546131612-%26ntime%3D1546169738"
    ,"UM_distinctid=1665ed29d400-0b98809a3a1d39-7e107977-1fa400-1665ed29d413eb; __client_id=1162eff404c00d0c69c89dba7b294defa025b31a; _uid=47136; CNZZDATA5476811=cnzz_eid%3D1582708982-1522252920-%26ntime%3D1546169738"
    ,"UM_distinctid=165b76dd0883e5-0cd47bbd4fb37d-5701631-144000-165b76dd08af2; _uid=30066; __client_id=2d760801eb0a94feb0df61c7c2eee4cd426b595e; CNZZDATA5476811=cnzz_eid%3D150289353-1536382573-%26ntime%3D1546175144"
    # ,"UM_distinctid=167b57379c37b7-0df532b6c56a8a-162a1c0b-100200-167b57379c42a4; CNZZDATA5476811=cnzz_eid%3D812717008-1544934122-%26ntime%3D1544934122; __client_id=a936317905b8535f084ba31a5447483ff710757b; _uid=85918"
]
taken = [False] * len(users)
cur = 0
def provider():
    global cur
    while True:
        if not taken[cur]:
            taken[cur] = True
            return users[cur], cur
        time.sleep(0.1)
        cur += 1
        if cur == len(users):
            cur = 0

import utils
from multiprocessing import Pool
utils.OFFSET_X = 162
utils.OFFSET_Y = 64
with open('data') as reader:
    data = reader.read()
    T = filter(lambda x : x.strip() != '', data.split('\n'))
    for i in xrange(len(T)):
        T[i] = map(int, filter(lambda c : c != '', T[i].split(' ')))
WIDTH = len(T)
HEIGHT = len(T[0])
log.info('Image size: %sx%s' % (WIDTH, HEIGHT))

from itertools import islice
p = Pool(processes = len(users))
BLOCK_SIZE = 16
try:
    while True:
        D = utils.slice(utils.fetch(), WIDTH, HEIGHT)
        log.info('Fetched canvas.')
        cnt = 0
        for idx in p.imap_unordered(utils.draw, utils.diff(D, T, provider)):
            taken[idx] = False
            cnt += 1
        # for idx in p.imap_unordered(utils.draw, islice(utils.diff(D, T, provider), 0, BLOCK_SIZE)):
        #     taken[idx] = False
        log.info('Rendered %s pixel(s).' % cnt)
        time.sleep(15)
except:
    exc_type, exc_value, exc_traceback = sys.exc_info()
    log.error('[%s] %s' % (exc_type.__name__, exc_value))
    log.error('Python traceback:\n' + ''.join(traceback.format_tb(exc_traceback))[:-1])
    log.fatal('PAINT SERVICE EXITED.')

#!/usr/bin/env python

import sys
import time
import logging as log
import traceback
now = time.time
f = open('paint.log', 'w')
f.truncate()
f.close()
log.basicConfig(filename="paint.log", level=log.INFO,
    format='%(asctime)s - %(filename)s[line:%(lineno)d] - %(levelname)s: %(message)s'
)

users = [
    "__client_id=a882b43d8bca2ffa62f6d8a2ba0de97a5ee3d452; UM_distinctid=167b57379c37b7-0df532b6c56a8a-162a1c0b-100200-167b57379c42a4; _uid=47856; CNZZDATA5476811=cnzz_eid%3D812717008-1544934122-%26ntime%3D1546164337"
    ,"UM_distinctid=167b57379c37b7-0df532b6c56a8a-162a1c0b-100200-167b57379c42a4; CNZZDATA5476811=cnzz_eid%3D812717008-1544934122-%26ntime%3D1544934122; __client_id=a936317905b8535f084ba31a5447483ff710757b; _uid=85918"
    ,"UM_distinctid=166c587726221a-06a75cbcba80f2-7613374d-1fa400-166c58772633f5; __client_id=7afd2a4c51ea736c635811535a312c27695a78aa; CNZZDATA5476811=cnzz_eid%3D73098803-1540908674-%26ntime%3D1546167865; _uid=45610"
    ,"__client_id=21da84d8a70341b6014b99d2c8c988ffc0c5ea00; UM_distinctid=167fce9ac7557c-0bd292d25463be-3f674604-19fa51-167fce9ac771e2; _uid=51074; CNZZDATA5476811=cnzz_eid%3D275005417-1546131612-%26ntime%3D1546169738"
    ,"UM_distinctid=1665ed29d400-0b98809a3a1d39-7e107977-1fa400-1665ed29d413eb; __client_id=1162eff404c00d0c69c89dba7b294defa025b31a; _uid=47136; CNZZDATA5476811=cnzz_eid%3D1582708982-1522252920-%26ntime%3D1546169738"
    ,"UM_distinctid=165b76dd0883e5-0cd47bbd4fb37d-5701631-144000-165b76dd08af2; _uid=30066; __client_id=2d760801eb0a94feb0df61c7c2eee4cd426b595e; CNZZDATA5476811=cnzz_eid%3D150289353-1536382573-%26ntime%3D1546175144"
    ,"__client_id=cfcb283c3f41aa379a70fbc14e6eb6f062d8b993; UM_distinctid=1680395a8c6508-03303c01d9c7a-19291c0a-1fa400-1680395a8c79d2; CNZZDATA5476811=cnzz_eid%3D1210818396-1546245527-%26ntime%3D1546245527; _uid=20010"
    ,"UM_distinctid=168c503f175aa5-0f1556a42ce7ad-3a614f0b-144000-166c503f176230; _uid=130970; __client_id=26d2c9119d8707574e27d3fdc8e6f909fb0574fc; CNZZDATA5476811=cnzz_eid%3D1654759349-1540901299-%26ntime%3D1546245527"
    ,"__client_id=d0d8bc38ef613a7ad7bb749b85fbdf1de170b3e6; UM_distinctid=16804556a52b3-0e09dd517802c2-18211c0a-100200-16804556a5318a; CNZZDATA5476811=cnzz_eid%3D312990270-1546257904-%26ntime%3D1546257904; _uid=126658"
    ,"__client_id=688c05bcc25505bc612597dfd60567ba14763a57; UM_distinctid=168045bc5b316e-00f71c96293c3d-3f674604-144000-168045bc5b5315; CNZZDATA5476811=cnzz_eid%3D1757695589-1546257904-%26ntime%3D1546257904; _uid=120376"
    ,"__client_id=e2b6652e33d1280e25bb097fdeea25bcbcfd4a74; UM_distinctid=16804d08d4a340-09966c8dd45368-47425224-100200-16804d08d4b221; CNZZDATA5476811=cnzz_eid%3D931480051-1546268714-%26ntime%3D1546268714; _uid=130233"
    ,"__client_id=df71d87e172dfede1a4011af01a6b6d1607e4e24; UM_distinctid=16804c52ae933d-075c5f7e32dfa2-18211c0a-100200-16804c52aea19b; CNZZDATA5476811=cnzz_eid%3D4034381-1546263307-%26ntime%3D1546263307; _uid=119610"
]
last = [now() - 30] * len(users)
taken = [False] * len(users)
cur = 0
def provider():
    global cur
    i = cur
    while True:
        i += 1
        if i == len(users):
            i = 0
        if now() - last[i] >= 30.0 and not taken[i]:
            last[i] = now()
            taken[i] = True
            return users[i], i
        time.sleep(0.1)

import utils
from multiprocessing import Pool
utils.OFFSET_X = 160
utils.OFFSET_Y = 28
with open('data') as reader:
    data = reader.read()
    T = filter(lambda x : x.strip() != '', data.split('\n'))
    for i in xrange(len(T)):
        T[i] = map(int, filter(lambda c : c != '', T[i].split(' ')))
WIDTH = len(T)
HEIGHT = len(T[0])
log.info('Image size: %sx%s' % (WIDTH, HEIGHT))

if __name__ == "__main__":
    from itertools import islice
    p = Pool(processes = len(users) / 3)
    BLOCK_SIZE = 16
    try:
        while True:
            fetched = utils.fetch()
            if not fetched:
                log.fatal('Failed to fetch canvas.')
                exit(-1)
            D = utils.slice(utils.fetch(), WIDTH, HEIGHT)
            log.info('Fetched canvas.')
            cnt = 0
            for idx in p.imap_unordered(utils.draw, utils.diff(D, T, provider)):
                taken[idx] = False
                cnt += 1
            # for idx in p.imap_unordered(utils.draw, islice(utils.diff(D, T, provider), 0, BLOCK_SIZE)):
            #     taken[idx] = False
            log.info('Rendered %s pixel(s).' % cnt)
            if cnt:
                time.sleep(0.1)
            else:
                time.sleep(60)
    except:
        exc_type, exc_value, exc_traceback = sys.exc_info()
        log.error('[%s] %s' % (exc_type.__name__, exc_value))
        log.error('Python traceback:\n' + ''.join(traceback.format_tb(exc_traceback))[:-1])
        log.fatal('PAINT SERVICE EXITED.')
        exit(-1)


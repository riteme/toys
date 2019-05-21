import json
import logging as log
from time import sleep
from random import shuffle
from requests import get, post

USER_AGENT = 'Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/71.0.3578.80 Safari/537.36'
PAINT_DATA_URL = 'https://www.luogu.org/paintBoard/board'
PAINT_POST_URL = 'https://www.luogu.org/paintBoard/paint'
OFFSET_X = 0
OFFSET_Y = 0
TIMEOUT = 3
REST_TIME = 10
RETRY_TIME = 10
MAX_RETRY = 3

def parse(s):
    data = filter(lambda x : x.strip() != '', s.split('\n'))
    for i in xrange(len(data)):
        data[i] = map(
            lambda c : int(c, base = 36), filter(lambda c : c != ' ', data[i]))
    return data

def fetch():
    r = get(PAINT_DATA_URL, headers={'user-agent': USER_AGENT}, timeout=TIMEOUT)
    if r.status_code != 200:
        return None
    return parse(r.content)

def draw(args, no_block=False):
    pixel, user, userid = args
    x, y, c = pixel
    ok = False
    for i in xrange(MAX_RETRY):
        r = post(PAINT_POST_URL, headers={
            'user-agent': USER_AGENT,
            'cookie': user
        }, data={'x': x, 'y': y, 'color': c}, timeout=TIMEOUT)
        try:
            ret = json.loads(r.content)
        except:
            ret = {'status': 404}
        if r.status_code == 200 and ret['status'] == 200:
            ok = True
            break
        log.warn(u'[%s] %s' % (userid, ret['data']))
        sleep(RETRY_TIME)
    if ok:
        log.info('[%s] OK. (%s, %s, %s)' % (userid, x, y, c))
    else:
        log.error('[%s] Failed.' % userid)
    if not no_block:
        sleep(REST_TIME)
    return userid

def slice(data, w, h):
    x, y = OFFSET_X, OFFSET_Y
    ret = data[x:x + w]
    for i in xrange(len(ret)):
        ret[i] = ret[i][y:y + h]
    return ret

def diff(pre, nxt, user_provider):
    xl = range(len(pre))
    yl = range(len(pre[0]))
    shuffle(xl)
    shuffle(yl)
    for x in xl:
        for y in yl:
            if pre[x][y] != -1 and nxt[x][y] != -1 and pre[x][y] != nxt[x][y]:
                user, userid = user_provider()
                yield (OFFSET_X + x, OFFSET_Y + y, nxt[x][y]), user, userid


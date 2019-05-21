#!/usr/bin/env python

from PIL import Image
import json
from colorsys import rgb_to_hsv
import sys

imagePath = sys.argv[1]

colors = [
    (160,106,70),
    (164,182,108),
    (186,139,175),
    (123,174,193),
    (133,194,186),
    (250,203,135),
    (220,151,86),
    (173,71,67),
    (57,57,57),
    (40,40,40),
    (22,22,22)
]

if '-f' in sys.argv:
    FORBIDDEN_COLOR = True
mem = {}
def get_color(pixel):
    if pixel not in mem:
        mem[pixel] = min_color_diff(pixel, colors)[1]
    return mem[pixel]


def to_hsv(color):
    return rgb_to_hsv(*[x / 255.0 for x in color])


def color_dist(c1, c2):
    r1, g1, b1 = c1
    r2, g2, b2 = c2
    h1, s1, v1 = to_hsv(c1)
    h2, s2, v2 = to_hsv(c2)
    return (r1-r2)**2+(g1-g2)**2+(b1-b2)**2+(v1-v2)**2
    #return (v1-v2)**2+(s1-s2)**2
    #return sum(abs(a - b) for a, b in zip(to_hsv(c1), to_hsv(c2)))
    #return sum(abs(a - b) for a, b in zip(c1, c2))


def min_color_diff(color_to_match, colors):
    return min(
        (color_dist(color_to_match, test), test)
        for test in colors)


def main():
    im = Image.open(imagePath)
    data = []
    w, h = im.size
    print("width", w, "height", h)

    preview = Image.new("RGB", (w, h), 0xffffff)
    n = w*h
    cnt = 0
    t = 1
    for i in range(0, w, 1):
        for j in range(0, h, 1):
            color = get_color(im.getpixel((i, j)))
            preview.putpixel((i, j), color)
            cnt += 1
            if 100*cnt >= n*t:
                print("%s%%" % (t))
                t += 1

    preview.save("preview.bmp", "bmp")


if __name__ == '__main__':
    main()

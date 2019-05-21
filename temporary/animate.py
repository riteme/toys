stage = []
mem = []
frames = []
GRAPHICS = ""

def reset(w, h):
    global stage
    global mem
    global frames
    global GRAPHICS

    stage = []
    mem = []
    frames = []
    GRAPHICS = "Graphics[%%s, PlotRange -> {{0.6, %s - 0.6}, {0.6, %s - 0.6}}]" % (w, h)

def push(x):
    global stage

    stage.append(x)

def pop():
    global stage

    stage.pop()

def save():
    global mem
    global stage

    mem.append(stage)
    stage = []

def submit():
    global frames
    global stage

    frames.append("Join[%s, {%s}]" % ("b" + str(len(mem)), ", ".join(stage)))
    stage = []

def export():
    global frames
    global mem

    ret = ["b0 := {}"]
    for i in xrange(len(mem)):
        ret.append(
            "b%s := Join[b%s, {%s}]" % (i + 1, i, ", ".join(mem[i]))
        )

    data = [GRAPHICS % x for x in frames]
    ret.append("L = {%s}" % ", ".join(data))
    return "\n".join(ret)

import turtle
t = turtle.Pen()

fwd = 2
size = 1
colors = ['orange','red','pink','purple','green','blue','yellow']
color_i = 0
t.pensize(size)
t.speed(0)
for i in range(0,len(colors)*2):
    t.color(colors[i % len(colors)])
    for j in range(0,6):
        for k in range(0,5):
            if (k % 2 == 0):
                t.up()
            else:
                t.down()
            t.forward(fwd)
        t.left(60)
        #fwd = fwd + 3
        fwd = fwd * 1.05
        size = size * 1.02
        t.pensize(size)
    #fwd = fwd + 10

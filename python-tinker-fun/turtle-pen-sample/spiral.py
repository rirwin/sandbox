import turtle
t = turtle.Pen()

fwd = 10

colors = ['red','green','blue']
color_i = 0

for i in range(0,10):
    t.color(colors[i % 3])
    for j in range(0,6):
        t.forward(fwd)
        t.left(60)
        #fwd = fwd + 3
        fwd = fwd * 1.05
    #fwd = fwd + 10

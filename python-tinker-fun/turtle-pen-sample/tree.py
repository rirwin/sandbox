import turtle

max_depth = 11
turn_scalar = 0.1
length_scalar = 1.3
turtle.width(3)
turtle.speed(0)
def tree(n,l):
    if n==0:
        return
    turtle.down()
    turtle.width(6*float(n)/max_depth)
    turtle.color(float(n)/max_depth,1-float(n)/max_depth,0.2)
    turtle.forward(l)
    turtle.left(20+turn_scalar*n)
    tree(n-1,l/length_scalar)
    turtle.right(40+2*turn_scalar*n)
    tree(n-1,l/length_scalar)
    turtle.left(20+turn_scalar*n)
    turtle.up()
    turtle.backward(l)


turtle.left(90)
turtle.up()
turtle.backward(250)
turtle.down()
tree(max_depth,150)
#turtle.exitonclick()

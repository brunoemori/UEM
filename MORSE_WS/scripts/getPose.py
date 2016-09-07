from pymorse import Morse

def printPos(pose):
    poseSensor = pose.get()
    print("I'm currently at X = %.2f, Y = %.2f, Z = %.2f" % (poseSensor['x'], poseSensor['y'], poseSensor['z']))

def main():
    with Morse() as morse:
        printPos(morse.robot1.pose)
            
main()

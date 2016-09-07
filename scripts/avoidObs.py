from pymorse import Morse

def isCloseRight(sick_stream):
    listRange = sick_stream.get()
    rangeValues = listRange['range_list']
    rightSensorLen = int(len(rangeValues) / 2)
    for i in range(0, rightSensorLen):
        if rangeValues[i] < 5:
            return True
        else:
            return False

def isCloseLeft(sick_stream):
    listRange = sick_stream.get()
    rangeValues = listRange['range_list']
    leftSensorLen = int(len(rangeValues) / 2)
    for i in range(len(rangeValues) - 1, leftSensorLen , -1):
        if rangeValues[i] < 5:
            return True
        else:
            return False

   

def getDir(sick_stream):
    listRange = sick_stream.get()
    rangeValues = listRange['range_list']
    distRay = listRange[0]
    dirPos = 0
    for i in rangeValues:
        if distRay < listRange[i]:
            distRay = listRange[i]
            dirPos = i
    return i

def main():
    with Morse() as morse:
        motion = morse.robot1.motion
        robotSick = morse.robot1.sick

        while True:
            sensorRight = isCloseRight(robotSick)
            sensorLeft = isCloseLeft(robotSick)
            if (sensorRight and sensorLeft):
                v_w = {"v": 0.5, "w": 1}
            elif sensorRight:
                v_w = {"v": 0.5, "w": 1}
            elif sensorLeft:
                v_w = {"v": 0.5, "w": -1}
            else:
                v_w = {"v": 2, "w": 0}
            motion.publish(v_w)
main()

from pymorse import Morse

def getSickRangeList(sick_stream):
    listRange = sick_stream.get()
    rangeValues = listRange['range_list']
    i = 0
    for rangeValue in rangeValues:
        print("%i ray %.2f" % (i, rangeValue))
        i = i + 1

def main():
    with Morse() as morse:
        getSickRangeList(morse.robot1.sick)

main()

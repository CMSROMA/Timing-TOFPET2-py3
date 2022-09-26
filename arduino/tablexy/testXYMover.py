from xyMover import XYMover
import time

print ("++++ Testing XYMover +++++")

aMover=XYMover(8820)#motor_0
#aMover=XYMover(8821)#motor_1
print (aMover.estimatedPosition())

#print ("++++ Moving to 25,25 +++++")
print (aMover.moveAbsoluteXY(25,25))
print (aMover.estimatedPosition())
print ("++++ Done +++++")

#print ("++++ Moving 100,30 out of bounds +++++")
#print (aMover.moveAbsoluteXY(100,30))
#print (aMover.estimatedPosition())
#print ("++++ Done +++++")



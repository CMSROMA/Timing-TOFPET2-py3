from xyMover import XYMover
import time

print ("++++ Testing XYMover +++++")

aMover=XYMover(8820)#motor_0
aMover1=XYMover(8821)#motor_1
print (aMover.estimatedPosition())

#print ("++++ Moving to 25,25 +++++")
#print (aMover.moveAbsoluteXY(25,25))
#print (aMover.estimatedPosition())

#approximately the center
print (aMover.moveAbsoluteXY(18,22))
print (aMover.estimatedPosition())
print (aMover1.moveAbsoluteXY(24,24))
print (aMover1.estimatedPosition())
#position after end of run
print (aMover.moveAbsoluteXY(20,48))
print (aMover.estimatedPosition())
print (aMover1.moveAbsoluteXY(24,1))
print (aMover1.estimatedPosition())
#
print ("++++ Done +++++")

#print ("++++ Moving 100,30 out of bounds +++++")
#print (aMover.moveAbsoluteXY(100,30))
#print (aMover.estimatedPosition())
#print ("++++ Done +++++")



from morse.builder import *

robot1 = ATRV()
motion = MotionVW()
robot1.append(motion)

#robot1.translate(y=2)

sick = Sick()
sick.properties(laser_range = 10)
sick.properties(scan_window = 90)
sick.properties(Visible_arc = True)
robot1.append(sick)

pose = Pose()
pose.translate(z = -0.10)
pose.add_stream('socket')
robot1.append(pose)

mov = Keyboard()
mov.properties(Speed = 2)
robot1.append(mov)

camera = VideoCamera()
camera.translate(z = 1)
robot1.append(camera)

motion.add_interface('socket')
pose.add_interface('socket')
sick.add_interface('socket')

env = Environment('indoors-1/boxes')
env.set_camera_location([10, -10, 12])
env.set_camera_rotation([1.0470, 0, 0.7854])
env.select_display_camera(camera)

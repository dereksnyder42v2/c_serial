#!/usr/bin/python3

"""
Usage:
    $ ./picampic.py <picture-name.jpeg> <jpeg image quality>
"""

import sys
import time
import picamera

picname = sys.argv[1]
#print("Writing image to", picname)
qty = int(sys.argv[2]) #jpeg image quality; lower means smaller filesize!

camera = picamera.PiCamera()
camera.resolution = (1024, 768)
#camera.start_preview() #uncomment if you want to get a quick peek prior to snapping a picture
# Camera warm-up time ~2 seconds
time.sleep(2)
camera.capture(picname, quality=qty) #quality only works for jpgs
"""
camera.capture(picname+'.jpeg', format='jpeg')
time.sleep(2)

camera.capture(picname+'.png', format='png')
time.sleep(2)

camera.capture(picname+'.gif', format='gif')
time.sleep(2)

camera.capture(picname+'.bmp', format='bmp')
"""

#print("Done")
quit(0)

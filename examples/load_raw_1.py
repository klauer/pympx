from __future__ import print_function
import time
import os
import sys
import numpy as np
import matplotlib.pyplot as plt
import multiprocessing

# sys.path.insert(0, '../build/lib.linux-x86_64-2.7')
import pympx

class Log(object):
    def write(self, text):
        print(text.strip())

buf = Log()
logger = pympx.MpxFileLogger(buf)
mpx = pympx.MpxModule(0, 3, 0, pympx.MPIX_ROWS, 0, logger)

with open('test_1.raw', 'rb') as inputf:
    for i, (frame, lost_rows) in enumerate(mpx.read_frames(inputf)):
        print('Frame %d (lost_rows=%s)' % (i, lost_rows))
        print('nonzero pixels: %d' % len(frame[np.where(frame > 0)]))
        frame[np.where(frame > 0)] = 1
        plt.imshow(frame.reshape(512, 512), cmap='Greys')
        plt.colorbar()
        plt.show()

# distutils: language = c++
# distutils: libraries = mpxhwrelaxd boost_thread boost_system boost_chrono pthread rt tiff
# distutils: library_dirs = .
# distutils: include_dirs = sophy/lib/common/

import cython
from cpython.ref cimport PyObject

# std::string
from libcpp.string cimport string
# memoryviews http://docs.cython.org/src/userguide/memoryviews.html
from cython cimport view

import numpy as np
cimport numpy as np
cimport classes

cdef class MpxLogger:
    cdef classes.PyMpxLogger *thisptr
    def __cinit__(self):
        self.thisptr = NULL

    def __init__(self):
        self.thisptr = new classes.PyMpxLogger()

    def __dealloc__(self):
        if self.thisptr != NULL:
            del self.thisptr


cdef class MpxStdoutLogger(MpxLogger):
    def __init__(self):
        self.thisptr = new classes.PyMpxStdoutLogger()


cdef class MpxFileLogger(MpxLogger):
    def __init__(self, object file_like):
        self.thisptr = new classes.PyMpxFileLogger(file_like)


cdef class MpxModule:
    cdef classes.MpxModule *thisptr
    hdr_dtype = np.dtype([('lost_rows', '<i'),
                        ('frame_size', '<I'),
                        # frame data
                        ])

    def __cinit__(self, int id, int mpx_type, int ndevices, int nrows, int first_chipnr, MpxLogger log):
        cdef classes.PyMpxLogger *log_parm = NULL
        if log is not None:
            log_parm = log.thisptr

        self.thisptr = new classes.MpxModule(id, mpx_type, ndevices, nrows, first_chipnr, log_parm)

    @cython.boundscheck(False)
    def raw_to_bin(self, classes.u8[:] input, classes.i16[:] output):
        temp_ = view.array(shape=(output.shape[0], ), itemsize=2, format="<h")

        # TODO in-place reorder
        cdef classes.i16[:] temp = temp_

        with nogil:
            self.thisptr.parStream2Data(&input[0], &temp[0])

            classes.reorderPixels(&temp[0], &output[0])

    def read_frames(self, inputf, convert_to_bin=True):
        frame = 0

        while True:
            header = np.fromfile(inputf, dtype=self.hdr_dtype, count=1)
            if not header:
                break

            # TODO header each frame?
            lost_rows = header['lost_rows']
            n_bytes = header['frame_size']
            data = np.fromfile(inputf, dtype='B', count=n_bytes)

            if len(data) != n_bytes:
                break

            n_words = n_bytes / 2

            if convert_to_bin:
                data.setflags(write=1, align=1)
                output_data = np.zeros(n_words, dtype='int16')
                output_data.setflags(write=1, align=1)
                self.raw_to_bin(data, output_data)
                yield output_data, lost_rows
            else:
                yield data, lost_rows

    def __dealloc__(self):
        del self.thisptr


MPIX_MAX_DEVS = classes.MPIX_MAX_DEVS
MPIX_ROWS = classes.MPIX_ROWS
MPIX_COLS = classes.MPIX_COLS
MPIX_PIXELS = classes.MPIX_PIXELS
MPIX_PIXEL_BITS = classes.MPIX_PIXEL_BITS
MPIX_ROW_BYTES = classes.MPIX_ROW_BYTES
MPIX_FRAME_BYTES = classes.MPIX_FRAME_BYTES
MPIX_FSR_BITS = classes.MPIX_FSR_BITS
MPIX_FSR_BYTES = classes.MPIX_FSR_BYTES
MPIXD_PORT = classes.MPIXD_PORT

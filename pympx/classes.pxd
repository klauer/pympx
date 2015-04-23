from cpython.ref cimport PyObject
from libcpp.string cimport string

from libc.stdint cimport (int8_t, uint8_t, int16_t, uint16_t, int32_t,
                          uint32_t, int64_t, uint64_t)


cdef extern from "asitypes.h":
    ctypedef int8_t i8
    ctypedef uint8_t u8
    ctypedef int16_t i16
    ctypedef uint16_t u16
    ctypedef int32_t i32
    ctypedef uint32_t u32
    ctypedef int64_t i64
    ctypedef uint64_t u64

cdef extern from "Relaxdhw.h":
    cdef int MPIX_MAX_DEVS
    cdef int MPIX_ROWS
    cdef int MPIX_COLS
    cdef int MPIX_PIXELS
    cdef int MPIX_PIXEL_BITS
    cdef int MPIX_ROW_BYTES
    cdef int MPIX_FRAME_BYTES
    cdef int MPIX_FSR_BITS
    cdef int MPIX_FSR_BYTES
    cdef int MPIXD_PORT

cdef extern from "mpxlogger.h":
    cdef enum MpxLogLevel:
        MPX_TRACE
        MPX_DEBUG
        MPX_INFO
        MPX_WARN
        MPX_ERROR
        MPX_FATAL

cdef extern from "raw.cpp":
    void reorderPixels(i16 *pInput, i16 *pOutput) nogil

cdef extern from "pympxlogger.h":
    cdef cppclass PyMpxLogger:
        pass

    cdef cppclass PyMpxStdoutLogger(PyMpxLogger):
        pass

    cdef cppclass PyMpxFileLogger(PyMpxLogger):
        PyMpxFileLogger(object file_like)


cdef extern from "mpxmodule.h":
    cdef cppclass MpxLogger:
        trim(string)

    cdef cppclass MpxModule:
        MpxModule(int, int, int, int, int, PyMpxLogger*)

        void parStream2Data(u8 *stream, i16 *data) nogil

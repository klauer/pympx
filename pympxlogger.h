#include <mpxlogger.h>
#include <cstdio>
#include <iostream>
#include <Python.h>


class PyMpxLogger : public MpxLogger {
public:
    PyMpxLogger() : MpxLogger() {
    }

    virtual void log(MpxLogLevel level, std::string message) {
    }

    virtual ~PyMpxLogger() {
    }
};

class PyMpxStdoutLogger : public PyMpxLogger {
public:
    PyMpxStdoutLogger() : PyMpxLogger() {
    }

    void log(MpxLogLevel level, std::string message) {
        std::cout << "[Timepix(" << level << ")] " << message << std::endl;
    }

    ~PyMpxStdoutLogger() {
    }
};

class PyMpxFileLogger : public PyMpxLogger {
private:
    PyObject *fp;

public:
    PyMpxFileLogger(PyObject *log_f) : PyMpxLogger() {
        fp = (PyObject*)log_f;
        Py_XINCREF(fp);
    }

    void log(MpxLogLevel level, std::string message) {
        if (!fp) {
            return;
        }
       
        if (!PyEval_ThreadsInitialized()) {
            PyEval_InitThreads();
        }
        
        PyGILState_STATE gstate;
        gstate = PyGILState_Ensure();

        std::stringstream stream;
        stream << "[" << level << "] " << message << std::endl;
        const std::string msg = stream.str();
        
        // PyObject_CallMethod(fp, "write", (char*)msg.c_str());

        PyObject *py_write = PyString_FromString("write");
        PyObject *py_msg = PyString_FromString(msg.c_str());

        PyObject *ret = PyObject_CallMethodObjArgs(fp, py_write, py_msg, NULL);

        if (NULL == ret) {
            PyErr_Clear();
            // TODO: exception not raised properly and interferes with subsequent calls

            // PyObject *type = NULL, *value = NULL, *traceback = NULL;
            // PyErr_Fetch(&type, &value, &traceback);
            // PyErr_NormalizeException(&type, &value, &traceback);
            // 
            // PyThreadState *ts = PyThreadState_Get();
            // PyThreadState_SetAsyncExc(ts->thread_id, value);
        }

        Py_XDECREF(py_write);
        Py_XDECREF(py_msg);
        Py_XDECREF(ret);

        PyGILState_Release(gstate);
    }

    ~PyMpxFileLogger() {
        Py_XDECREF(fp);
    }
};


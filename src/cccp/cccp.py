"""
In this file we are callin the cpp lib libspm.so
and we are executing the ccccp() function.
"""
import ctypes
import os
import sys

def open_libspm() :
    """
    This function opens the libspm.so library.
    """
    libspm = ctypes.CDLL(os.path.join(os.path.dirname(__file__), "../../bin/libspm.so"))
    return libspm

libspm = open_libspm()
# call the cccp() function
libspm.cccp()





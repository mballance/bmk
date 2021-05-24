'''
Created on May 20, 2021

@author: mballance
'''

import hvlrpc
from ctypes import *
from posix import nice

@hvlrpc.api_imp
class BmkSemihost(object):
    
    def __init__(self):
        self.coreid = None
        pass
    
    @hvlrpc.func
    def bmk_info_low_v(self, fmt : str, ap : hvlrpc.va_list):
        if self.coreid is not None:
            print(self.coreid + ": " + self.format(fmt, ap))
        else:
            print(self.format(fmt, ap))
    
    @hvlrpc.func
    def bmk_info_mid_v(self, fmt : str, ap : hvlrpc.va_list):
        if self.coreid is not None:
            print(self.coreid + ": " + self.format(fmt, ap))
        else:
            print(self.format(fmt, ap))
    
    @hvlrpc.func
    def bmk_info_high_v(self, fmt : str, ap : hvlrpc.va_list):
        if self.coreid is not None:
            print(self.coreid + ": " + self.format(fmt, ap))
        else:
            print(self.format(fmt, ap))
    
    @hvlrpc.func
    def bmk_error_v(self, fmt : str, ap : hvlrpc.va_list):
        if self.coreid is not None:
            print("[Error] " + self.coreid + ": " + self.format(fmt, ap))
        else:
            print("[Error] " + self.format(fmt, ap))
    
    @hvlrpc.func
    def bmk_fatal_v(self, fmt : str, ap : hvlrpc.va_list):
        if self.coreid is not None:
            print("[Fatal] " + self.coreid + ": " + self.format(fmt, ap))
        else:
            print("[Fatal] " + self.format(fmt, ap))
    
    def format(self, fmt, ap : hvlrpc.va_list) -> str:
        params = []

        final_fmt = ""        
        i=0
        while i < len(fmt):
            ni = fmt.find('%', i)
            
            if ni == -1:
                final_fmt += fmt[i:]
                break
            else:
                final_fmt += fmt[i:ni]
                
            i = ni
            
            # TODO: skip width characters
            final_fmt += fmt[i]
            i += 1
            
            fc = fmt[i]

            sz_64 = False                
#            if fc == 'l' and (i+1) < :
#                "%lld"
                
            if fc in ['x', 'X', 'u']:
                final_fmt += "x"
                if sz_64:
                    params.append(ap.uint64())
                else:
                    params.append(ap.uint32())
            elif fc == 'd':
                final_fmt += "d"
                if sz_64:
                    params.append(ap.int64())
                else:
                    params.append(ap.int32())
            elif fc == 's':
                final_fmt += "s"
                params.append(ap.str())
            elif fc == 'p':
                final_fmt += "08x"
                params.append(ap.ptr())
            i += 1

        return final_fmt % tuple(params)
    
    
'''
Created on May 20, 2021

@author: mballance
'''

import hvlrpc
from ctypes import *

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
        
        i=0
        while True:
            i = fmt.find('%', i)
            
            if i == -1:
                break
            
            # TODO: skip width characters
            i += 1
            
            fc = fmt[i]


            sz_64 = False                
#            if fc == 'l' and (i+1) < :
#                "%lld"
                
            if fc in ['x', 'X', 'u']:
                if sz_64:
                    params.append(ap.uint64())
                else:
                    params.append(ap.uint32())
            elif fc == 'd':
                if sz_64:
                    params.append(ap.int64())
                else:
                    params.append(ap.int32())
            elif fc == 's':
                params.append(ap.str())
                    
        return fmt % tuple(params)
    
    
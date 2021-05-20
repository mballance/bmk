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
        print("bmk_info_low_v")
        pass
    
    @hvlrpc.func
    def bmk_info_mid_v(self, fmt : str, ap : hvlrpc.va_list):
        print("bmk_info_mid_v")
        pass
    
    @hvlrpc.func
    def bmk_info_high_v(self, fmt : str, ap : hvlrpc.va_list):
        pass
    
    @hvlrpc.func
    def bmk_error_v(self, fmt : str, ap : hvlrpc.va_list):
        pass
    
    @hvlrpc.func
    def bmk_fatal_v(self, fmt : str, ap : hvlrpc.va_list):
        pass
    
    
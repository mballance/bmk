'''
Created on May 20, 2021

@author: mballance
'''
import argparse
import os


def mkfile(args):
    mkdv_dir = os.path.dirname(os.path.abspath(__file__))
    print(os.path.join(mkdv_dir, "share", "mkfiles", "dv.mk"))

def get_parser():
    parser = argparse.ArgumentParser()
    subparsers = parser.add_subparsers()
    subparsers.required = True
    subparsers.dest = 'command'
    
    mkfile_cmd = subparsers.add_parser('mkfile',
        help='Returns path to bmk.mk')
    mkfile_cmd.set_defaults(func=mkfile)
    
    return parser

def main():
    parser = get_parser()
    
    args = parser.parse_args()
    
    args.func(args)
    
    pass

if __name__ == "__main__":
    main()

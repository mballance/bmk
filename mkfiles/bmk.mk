
BMK_MKFILES_DIR := $(dir $(lastword $(MAKEFILE_LIST)))
BMK_DIR := $(abspath $(BMK_MKFILES_DIR)/..)

BMK := $(BMK_DIR)
export BMK



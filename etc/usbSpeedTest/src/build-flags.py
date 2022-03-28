"""
@file build-flags.py
@author Daniel Starke
@copyright Copyright 2022 Daniel Starke
@date 2022-03-28
@version 2022-03-28
"""

import platform

target = platform.system()

if __name__ == '__main__':
	if target == "Linux" or target == "Darwin":
		print("-lm")
	elif target == "Windows":
		# assuming Cygwin or MinGW
		print("-lm -lwinmm")

if __name__ == 'SCons.Script':
	Import("env")
	env.Append(LINKFLAGS=["-s"])
	if target == "Windows":
		env.Append(LINKFLAGS=["-static"])

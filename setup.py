
import os
from setuptools import setup

#data_files = []
#directories = glob.glob('src/share')

setup(
  name = "bmk",
  packages=['bmk'],
  package_dir = {'' : 'src'},
  package_data = {'bmk' : ['share/*']},
  author = "Matthew Ballance",
  author_email = "matt.ballance@gmail.com",
  description = ("Provides a core classes for use by memory-oriented BFMs"),
  license = "Apache 2.0",
  keywords = ["Python", "CocoTB", "embedded software"],
  url = "https://github.com/mballance/bmk",
  setup_requires=[
    'setuptools_scm',
  ],
  install_requires=[
      "pyhvl-rpc",
  ],
)


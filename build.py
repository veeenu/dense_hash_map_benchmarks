import os
import glob
import shutil
import subprocess

BUILD_CONFIG='Release'

cmake_args = dict(
  nt=['-DCMAKE_BUILD_TYPE=' + BUILD_CONFIG,  '../'],
  posix=['-DCMAKE_BUILD_TYPE=' + BUILD_CONFIG, '../'],
)

if __name__ == '__main__':
  if not os.path.exists('build'):
    os.mkdir('build')
  os.chdir('build')
  subprocess.run(['cmake', *cmake_args[os.name]])
  subprocess.run(['cmake', '--build', '.', '--config', BUILD_CONFIG])
  os.chdir('..')
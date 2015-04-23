from distutils.core import setup
from Cython.Build import cythonize

# libmpx is built with boost 1.52
libraries = ['mpxhwrelaxd', 'boost_thread', 'boost_system', 'boost_chrono', 'pthread', 'rt', 'tiff']
library_dirs = ['/usr/local/lib', '.']

sophy_include = './sophy/lib/common/'

setup(name='tpx_raw',
      version='0.1',
      description='TimePix raw file handling extension',
      author='K Lauer',
      author_email='klauer@bnl.gov',
      url='',
      long_description=''' ''',
      ext_modules=cythonize(['pympx.pyx'],
                            language='c++',
                            include_path=[sophy_include],
                            ),
      )

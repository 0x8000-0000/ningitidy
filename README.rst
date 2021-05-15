*********
NinGiTidy
*********

Caching accelerator for linters based on Ninja dependencies and Git status.


Prerequisites
=============

Conan package manager
C++ compiler


Building
========

.. code-block:: shell

   $ mkdir ningitidy.build
   $ cd ningitidy.build
   $ conan install ../ningitidy --profile=clang.11.dbg  -s compiler.cppstd=20 # --build=missing
   $ conan build ../ningitidy


License
=======

Apache 2.0

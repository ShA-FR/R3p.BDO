R3p.BDO
=======

.. image:: https://img.shields.io/github/issues/r3peat/R3p.BDO.svg
  :alt: Issues on Github
  :target: https://github.com/r3peat/R3p.BDO/issues

.. image:: https://img.shields.io/github/issues-pr/r3peat/R3p.BDO.svg
  :alt: Pull Request opened on Github
  :target: https://github.com/r3peat/R3p.BDO/issues

.. image:: https://img.shields.io/github/release/r3peat/R3p.BDO.svg
  :alt: Release version on Github
  :target: https://github.com/r3peat/R3p.BDO/releases/latest

.. image:: https://img.shields.io/github/release-date/r3peat/R3p.BDO.svg
  :alt: Release date on Github
  :target: https://github.com/r3peat/R3p.BDO/releases/latest

+--------------+--------------------------+---------------------------+---------------------------+--------------------------+--------------------------+--------------------------+
| Branch name  | R3p.bdo                  | R3p.bdo.GUIloader         | R3p.bdo.settings          | R3p.bdo.ui               | R3p.injector             | x64fw2                   |
+==============+==========================+===========================+===========================+==========================+==========================+==========================+
| master       | TODO: appveyor SVG       | TODO: appveyor SVG        | TODO: appveyor SVG        | TODO: appveyor SVG       | TODO: appveyor SVG       | TODO: appveyor SVG       |
+--------------+--------------------------+---------------------------+---------------------------+--------------------------+--------------------------+--------------------------+


Description
-----------

I decided to publish the source of this so people can learn from it or revamp it.
This project was created long long ago so please don't judge me cause of design flaws and performance issues. Just had no time to proper update it.
The Bot/Hack is a combination of external C# and an internal C++ dll, they communicate through a named pipe.
All dependend projects + dll project + loader project are inside "Imports" folder.


How to compile sources?
-----------------------

- Recreate classes with ftp credentials because are removed from **r3p.bdo.testconsole** + **r3p.bdo.guiloader**
- To make this work use ur **own ftp** with ur **own credentials** or **just disable these features**


!Required NuGet Packages!
~~~~~~~~~~~~~~~~~~~~~~~~~

- FluentFTP
- Microsoft.WindowsAPICodePack.Core
- Microsoft.WindowsAPICodePack.Shell
- SharpDX
- SharpDX.Direct2D1
- SharpDX.DXGI
- SharpDX.Mathematics


BlackDesertx64 Dumps
--------------------

- [x] _`Download v771`: https://mega.nz/#!IeJTmCBC!XB4WWKXsGiSRIxREaOVEG3nMfAld7dUyjeTl5XejwJc
- [x] _`Download v768`: https://mega.nz/#!nCRjQRoR!TkARcgyJgieS5ziiEQjddNMl3qIzZex6L8S7_m2cBBg
- [x] Download v760 : use v759
- [x] _`Download v759`: https://mega.nz/#!pfxChYKL!aq0DobI13uJjyAcONapmwUZnRiJfEU9YJE5lSZL6zzc
- [x] _`Download v753`: https://mega.nz/#!lHJEyRrS!IluBtUDZgEJ1LXwhWb3svGzC7vyU-HAsxDBlIUHPXRA


Contributing
~~~~~~~~~~~~

We welcome contributions to R3p.bdo community! These are the many ways you can help:

* Submit patches and features
* Make *new updates to improve main libraries or GUI*
* Report bugs 
* And Donate!

Please read our **documentation** to get started. Also note that this project
is released with a code-of-conduct_ , please make sure to review and follow it.

.. image:: https://www.paypalobjects.com/en_US/i/btn/btn_donateCC_LG.gif
  :alt: Donate Paypal
  :target: https://www.paypal.com/cgi-bin/webscr?cmd=_s-xclick&hosted_button_id=FZG2ELLF6RD46


.. |r3pbdo_master_lin| image:: https://travis-ci.org/r3peat/R3p.BDO.svg?branch=master
.. |r3pbdo_master_win| image:: https://ci.appveyor.com/api/projects/status/f4orjhi6vjgsxxq9/branch/master?svg=true
.. _code-of-conduct: https://github.com/r3peat/R3p.BDO/blob/master/CODE_OF_CONDUCT.rst

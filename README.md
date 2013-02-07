onyx-test
=========

Test e-ink fb. Test some framebuffer operations on onyx boox touch (i62/i62hd).

Current Progress
----------------
* PASS Modify code(see [1]). Comment out some functions.
* PASS Compile.
* FAIL Run on a device. Flushing screen is OK, drawing rectangle is OK, displaying picture is wrong. Device stalled( not response to touching and pressing, have to reboot) after tests, ssh still worked.


Reference Link
--------------
1. https://community.freescale.com/docs/DOC-93622

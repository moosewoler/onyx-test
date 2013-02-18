onyx-test
=========

Test e-ink fb. Test some framebuffer operations on onyx boox touch (i62/i62hd).

TODO list
---------
* How to operate framebuffer device directly in a user application without disturbing the system(user interface).
* How to add A2 waveform support into system infrastructure? This is the most ideal solution to make the user experience better. It is certainly not as a easy task as spliting a paper. 

Current Progress2013-02-18
--------------------------
* test_data_type(). test data size.
* test_draw_spot(). draw dot lines using A2 waveform.
* test_draw_pic(). copy a bulk of image data to framebuffer, update with A2 waveform.
* test_animate(). test animation using A2 waveform. very acceptable!
* It looks like some task has drew back the system performence after exiting the test program.
* fix previous problems.

2013-02-07
----------
* PASS Modify code(see [1]). Comment out some functions.
* PASS Compile.
* FAIL Run on a device. Flushing screen is OK, drawing rectangle is OK, displaying picture is wrong. Device stalled( not response to touching and pressing, have to reboot) after tests, ssh still worked.


Reference Link
--------------
1. https://community.freescale.com/docs/DOC-93622

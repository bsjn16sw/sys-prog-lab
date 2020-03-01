# System Programming Lab
6 system programming projects.<br>
These projects were main assignments of <i>'System Programming Laboratory'</i> class whose original name was <i>'Software Experiment 2'</i> in 2017. I revised these codes of 2017 and re-uploaded on this repository. The objects of this repository are 1) to review my old code by myself, 2) to remind contents of the class, and 3) to be references for other students who study system programming.

## Environment
I made and checked the code on Ubuntu 16.04 LTS.
<img src="/img/ubuntu.png" alt="Ubuntu">

## Table of contents
### PA0: Making own "string_sw.h"
#### Overview
In "string_sw.h", there are 19 functions for string manipulation. You can check information of these functions except `int2str()` in <a href="http://man7.org/linux/man-pages/dir_section_3.html">Linux man page</a>. Information of `int2str()` is below:<br><br>

`char *int2str(char *dest, int num);`<br>
<i>Convert integer to string.</i><br>
Get integer `num` and convert it to string. Then save in `dest` and return it. But if `dest` is NULL, dynamically allocate the memory and save string to there. Then return the address of it. If it is not available to dynamically allocate some memory space, return NULL.<br>

#### How to check
I already made `test.c` and `makefile` for verification. You can verify if "string_sw.h" works properly. Just enter `make` and `./test` in shell like below.
<img src="/img/pa0.png" alt="Verification of PA0">
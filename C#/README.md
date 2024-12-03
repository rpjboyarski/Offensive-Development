This is a simple loader designed to emulate the shellcode loading routine used by the Lazarus APT. I ran a version of this with better OPSEC for a year and a half with basically zero detections against AV, which is insane considering how stupid simple this is. Now that I'm a few generations ahead of this, I'm releasing this loader since I don't really have a use for it anymore.

Do not use the Makefile manually. This is because the shellcode needs to be patched in at compile time using **build.py**. For example, **python3 build.py -f example_calc_shellcode**.

This technique will not work for enormous shellcodes (we will run out of stack space). However, anything other than Sliver should be fine here. The build script automatically un-patches upon compilation completing, so don't worry about messing with it manually.

There are a number of semi-intentional OPSEC failures here to provide small areas for improvement. However, as it stands, this already is able to fly through defender when paired with a PowerShell download cradle to run it fileless.
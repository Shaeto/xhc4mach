# XHC xHB03/xHB04 MPG ipc driver for Mach4 CNC motion control software

## Short description
This software works as MPG for Mach4 using capabilities provided by Mach4IPC.dll

It supports XHC xHB03/xHB04 USB (wired and wireless) pendants and can handle multiple connected mpgs simultaneously

## How to use
 * Build xhc4mach project using MSVC Community 2017 or download xhc4mach.exe from download page
 * Put executable to Mach4 installation folder (C:\Mach4 for example)
 * Start Mach4
 * Start xhc4mach
 * Plug in XHC MPG(s)
 * Enable machine, jog it using MPG

Please avoid to use incremental jogging mode for now

**Application is very raw ! Please, be very careful and keep hand on E-Stop button!**

Disclaimer & Limitation of Liability:
I AM NOT LIABLE FOR ANY LOSS OR DAMAGE ARISING DIRECTLY OR INDIRECTLY (INCLUDING SPECIAL, INCIDENTAL OR CONSEQUENTIAL LOSS OR DAMAGE) FROM THE USERS'S USE OF THE APPLICATION.

Links:
[XHC xHB03/xHB04 HID PROTOCOL](https://bitbucket.org/moonglow/openxhc/raw/d91d201083120eecc5688ef0e1fb7eee501254c9/xhc_mpg_hid_format.pdf)

LICENSE: (CC BY-SA 4.0)[https://creativecommons.org/licenses/by-sa/4.0/]

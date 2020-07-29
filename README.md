# sdbox for Amiga

### A parallel to sd-card project for the Amiga

This code uses the code from the amiga-par-to-spi-adapter https://github.com/niklasekstrom/amiga-par-to-spi-adapter written by Niklas Ekström. KiCad design, Schematics and files for DesignSpark Mechanical v4 is made by me. Design of the 3d-printable sdbox case is made by me and Tobias Niva (niva3d, http://www.niva3d.com/). It is a collaborative work done by us three, Enjoy!

***

<a href="images/sdbox_rev_1a_pic1.png">
<img src="images/sdbox_rev_1a_pic1.png" width="260" height="270">
</a>
<a href="images/pcb_pic1.jpg">
<img src="images/pcb_pic1.jpg" width="260" height="195">
</a>
<a href="images/sd_box_pic1.jpg">
<img src="images/sd_box_pic1.jpg" width="260" height="195">
</a>
<a href="images/sdbox_front.jpg">
<img src="images/sdbox_front.jpg" width="260" height="195">
</a>
<a href="images/sdbox_backside_pic1.jpg">
<img src="images/sdbox_backside_pic1.jpg" width="260" height="195">
</a>
<a href="images/sdbox_backside_pic2.jpg">
<img src="images/sdbox_backside_pic2.jpg" width="260" height="195">
</a>

***

What is the sdbox for Amiga? 

It is a small device that you can plug into your Amiga's parallel port and get access to a sd-card formatted with e.g. a windows FAT file system. It uses an Arduino Nano V3 and a common cheap MicroSD Card Adapter that has a voltage level shifter on board. These small devices are soldered onto a small PCB together with a male d-sub db25 connector that fits into the Amiga parallel port (The A1000 needs a proper gender changer, be aware). The device must be powered externally either through the usb-connector on the Nano or via the separate barrel jack connector on the main PCB. A power supply with 1A DC capacity should suffice. `5V` via USB or `7-12V` via the barrel jack power connector that goes to the VIN of the Nano V3. The driver should be AmigaOS system friendly but takes exclusive access over the parallel port so don't expect to run other things on this port together with the sdbox with some kind of breakout cable. It should work from 1.3 and upwards, verified working on 1.3, 3.1 and 3.1.4 so far.

The schematics, BOM, kicad drawings and the 3d-printable case source and stl files can all be found in the hardware folder.

PCBs can be order directly via link below or you can generate your own gerbers from the KiCad files and send them to your favorite boardhouse mfg.

https://aisler.net/p/YVXJBQHM

***
Ok enough talk!, show us some videos:
<br />

Copying from Amiga to sd-card<br />
https://i.imgur.com/um15pal.mp4

Copying from sd-card to the Amiga (cpu fastrom activated, KS ROM loaded into Fast RAM)<br />
https://i.imgur.com/G6VRPpx.mp4

Copying from sd-card to the Amiga  (without fastrom activated) <br />
https://i.imgur.com/ADebdlJ.mp4

***

<a href="images/sd_box_pic2.jpg">
<img src="images/sd_box_pic2.jpg" width="260" height="195">
</a>
<a href="images/sd_box_pic3.jpg">
<img src="images/sd_box_pic3.jpg" width="260" height="195">
</a>
<a href="images/sd_box_pic4.jpg">
<img src="images/sd_box_pic4.jpg" width="260" height="195">
</a>

***


<a href="images/sdbox_3d_model_pic1.jpg">
<img src="images/sdbox_3d_model_pic1.jpg" width="260" height="229">
</a>
<a href="images/vFinal-4view.png">
<img src="images/vFinal-4view.png" width="256" height="205">
</a>
<br />

More rendered pictures and concepts by `niva3d` under the `images/_renders` directory, made in Blender, check it out!

***

### 3d-printing

Check the README in the `hardware/DSM` folder


***

### Howto build the spisd.device driver

Check the README in the sd folder.

For the lazy: Binaries available now in a zip file under the `release` tab above.

***

### Caveats

One little caveat to keep in mind is that we do no CRC-verification of data being copied so please verify your files afterwards manually if you are going to copy important files over this way like taking a backup or something. Another caveat is that not all sd-cards might work with the sdbox so aim for a cheap 4/8 GB card and format it with a smaller partition `below 4GB` with FAT is a good starting point. If you see an error message saying `bad number` when trying to mount `SD0` it may be an indication that your particular sd-card simply doesn't work with the sdbox for some reason.

Everything done here is provide `as is`, don't blame us if your Amiga CIAs dies or your files get corrupted using the sdbox. I don't think it will happen though. Be careful though when plugging the sdbox in or out of the Amiga, always do that with the Amiga turned off.

    Some advice from Tom Handley / 18 April 93 (taken from pario on Aminet)

    NEVER connect, disconnect, or switch, parallel port lines with the power on!  
    ALWAYS shut the power off first!  In most cases, you should use the following sequence:

       Power up:   - Turn on your Amiga
                   - Turn on the external device

       Power down: - Turn off the external device
                   - Turn off your Amiga
                   
    If you have been switching devices with the power on you have been lucky...
    Not following the above precaution, and static electricity, are the two main
    causes of user-induced 8520 failures.
***

The sequence I use when powering the sdbox via USB is this:

* Plug the micro sd-card into the MicroSD card slot of the sdbox
* With the Amiga turned off plug the sdbox into the Amiga 
* Connect the +5V USB power cable into the sdbox, the two LEDs will light up with a steady shine. Don't worry that the activity LED light turns on, it's on when the SEL/CS line goes low (Zero). It will turn off as soon as the Amiga is turned on.
* Turn on the Amiga
* In wb mount the card
* When done, turn off the Amiga, the sdbox Activity LED will light up again as a reminder to unplug the USB power cable
* Unplug the sdbox from the Amiga if you want to remove it.

This has worked without any issues for me.

Hopefully powering via the barrel jack and using U3 chip (Solid State Relay) will work and automatically handle the Power cycling following the Amiga's on/off.

__UPDATE: Powering via the barrel jack is now verified and working.__ see last section in this readme

***

### Kudos

Many thanks to Mike Stirling for writing the original k1208-drivers and making them open source, we built further on his code. It was a good starting point. Thanks Mike and kipper2k and others involved in k1208.

***

### Performance 

What about performance you may ask? 

Well, Niklas managed to write the drivers with some real clever assembler to make it go at 2E-speed (theoretical 350 KB/s, IRL around 225-280 KB/s) if you have some kind of Accelerator installed in your A500. 
Copying a 23 MB file from the SD-card to the compact flash in the HC508 took 98 seconds, giving a throughput of 225 KB/s. We think this is a good result given the fact of the slow clocking of the CIAs (E-clock). It will also work with a standard 68k CPU at 7 MHz although slower.

With my TF534 and 3.1 rom loaded into Fast RAM via the command `cpu fastrom` (requires a MMU) I get 280 KB/s in SysInfo, and without fastrom 250 KB/s. It is a little slower copying to the Amiga than from the Amiga.

<a href="images/screenshots/sdcard_sysinfo_speed.jpg">
<img src="images/screenshots/sdcard_sysinfo_speed.jpg" width="303" height="227">
</a>
<a href="images/screenshots/amiga_parallel_port_2E_speed.jpg">
<img src="images/screenshots/amiga_parallel_port_2E_speed.jpg" width="290" height="188">
</a>


Speed definition of 2E here: <br />
https://lallafa.de/blog/2015/09/amiga-parallel-port-how-fast-can-you-go/

***

### Howto build the sdbox

Start with removing the angled pins from the MicroSD Card Adapter and replace with straight pins like shown on pictures below:

<a href="images/build/sdbox_build_pic1.jpg">
<img src="images/build/sdbox_build_pic1.jpg" width="252" height="189">
</a>
<a href="images/build/sdbox_build_pic2.jpg">
<img src="images/build/sdbox_build_pic2.jpg" width="252" height="189">
</a>
<a href="images/build/sdbox_build_pic3.jpg">
<img src="images/build/sdbox_build_pic3.jpg" width="189" height="252">
</a>
<a href="images/build/sdbox_build_pic4.jpg">
<img src="images/build/sdbox_build_pic4.jpg" width="189" height="252">
</a>
<a href="images/build/sdbox_build_pic5.jpg">
<img src="images/build/sdbox_build_pic5.jpg" width="252" height="189">
</a>
<a href="images/build/sdbox_build_pic6.jpg">
<img src="images/build/sdbox_build_pic6.jpg" width="252" height="189">
</a>

***

Now we start soldering on the surface mount parts, the passives, also known as the birdseed :) Start with Q1 and Q2, they are tiny so you probably need som sort of magnification. I use a lamp with a built-in magnifier glass, it works well, I do not have a scope. Use a fine tip with your solder iron...

***

<a href="images/build/sdbox_build_pic7.jpg">
<img src="images/build/sdbox_build_pic7.jpg" width="252" height="189">
</a>
<a href="images/build/sdbox_build_pic8.jpg">
<img src="images/build/sdbox_build_pic8.jpg" width="252" height="189">
</a>
<a href="images/build/sdbox_build_pic10.jpg">
<img src="images/build/sdbox_build_pic10.jpg" width="252" height="189">
</a>
<a href="images/build/sdbox_build_pic9.jpg">
<img src="images/build/sdbox_build_pic9.jpg" width="189" height="252">
</a>
<a href="images/build/sdbox_build_pic11.jpg">
<img src="images/build/sdbox_build_pic11.jpg" width="189" height="252">
</a>

***

Now with the surface mount stuff done we do the through-hole stuff, solder on two cut-down pins as leg support for the MicroSD Card Adapter, the adapter will rest on these and make it in level and the same height as the Nano.

***

<a href="images/build/sdbox_build_pic12.jpg">
<img src="images/build/sdbox_build_pic12.jpg" width="252" height="189">
</a>
<a href="images/build/sdbox_build_pic13.jpg">
<img src="images/build/sdbox_build_pic13.jpg" width="252" height="189">
</a>
<a href="images/build/sdbox_build_pic14.jpg">
<img src="images/build/sdbox_build_pic14.jpg" width="252" height="189">
</a>
<a href="images/build/sdbox_build_pic15.jpg">
<img src="images/build/sdbox_build_pic15.jpg" width="252" height="189">
</a>

***

Now, with this type of connector the metal plate comes loose when you unscrew the connector nuts. We need to remove them because we don't want to remove them on the Amiga side. Use some super glue and a vise or similar to press them together a couple of minutes until the glue bites. Be quick if you use super glue! Then put the screws back while soldering, we want the metal pieces in there as a support taking some force off the pins when plugging the device in and out of the Amiga.

***
<a href="images/build/sdbox_build_pic16.jpg">
<img src="images/build/sdbox_build_pic16.jpg" width="252" height="189">
</a>
<a href="images/build/sdbox_build_pic17.jpg">
<img src="images/build/sdbox_build_pic17.jpg" width="252" height="189">
</a>
<a href="images/build/sdbox_build_pic18.jpg">
<img src="images/build/sdbox_build_pic18.jpg" width="252" height="189">
</a>
<a href="images/build/sdbox_build_pic19.jpg">
<img src="images/build/sdbox_build_pic19.jpg" width="252" height="189">
</a>

***

Now solder the MicroSD Card Adapter and the Nano down to the PCB as well.

***

<a href="images/build/sdbox_build_pic20.jpg">
<img src="images/build/sdbox_build_pic20.jpg" width="252" height="189">
</a>
<a href="images/build/sdbox_build_pic21.jpg">
<img src="images/build/sdbox_build_pic21.jpg" width="252" height="189">
</a>

***

Almost done!, time to put the LEDs in. To get the correct LED height for the sdbox 3d-printable case, I used a leftover trial print of the bottom part and drilled a hole through it to give me access to tack the LEDs down, with the iron, while holding the LEDs in place pressing on the legs gently. Cut the legs like in the picture below so that you can remove the pcb from the case and solder and trim them better afterwards. Job done!

***

<a href="images/build/sdbox_build_pic22.jpg">
<img src="images/build/sdbox_build_pic22.jpg" width="252" height="189">
</a>
<a href="images/build/sdbox_build_pic23.jpg">
<img src="images/build/sdbox_build_pic23.jpg" width="189" height="252">
</a>
<a href="images/build/sdbox_build_pic24.jpg">
<img src="images/build/sdbox_build_pic24.jpg" width="189" height="252">
</a>
<a href="images/build/sdbox_build_pic25.jpg">
<img src="images/build/sdbox_build_pic25.jpg" width="189" height="252">
</a>
<a href="images/build/sdbox_build_pic26.jpg">
<img src="images/build/sdbox_build_pic26.jpg" width="252" height="189">
</a>
<a href="images/build/sdbox_build_pic27.jpg">
<img src="images/build/sdbox_build_pic27.jpg" width="252" height="189">
</a>

***
### Update: Powering via the barrel jack

So I finally got parts to try out powering the device via the optional barrel jack. It seems to work just fine. I used a `9V DC 1A`, center pin positive, adapter.

Solder the barrel jack and the solid state relay (PVG612S) to the PCB. I made a small parallel extension adapter in order to let me easily measure the control current draw of the `PVG612S`.

***

<a href="images/build/sdbox_build_pic28.jpg">
<img src="images/build/sdbox_build_pic28.jpg" width="252" height="189">
</a>
<a href="images/build/sdbox_build_pic29.jpg">
<img src="images/build/sdbox_build_pic29.jpg" width="252" height="189">
</a>
<a href="images/build/sdbox_build_pic30.jpg">
<img src="images/build/sdbox_build_pic30.jpg" width="252" height="189">
</a>

***

Video clip below, as you can see powering of the sdbox is now controlled by the Amiga's on/off and you can just leave the sdbox always connected via the barrel jack. The control current draw for the solid state relay is around `9.7 mA` which is just below the `10 mA` max recommended draw from the Amigas parallel port pin 14. Great!

https://i.imgur.com/eSkcy3X.mp4

First checking the `9V` adapter and verifying it is indeed center pin positive and outputs around 9V. It shows around `9.3V`. The current draw I measured to around `9.7 mA` with the `330 Ohm` on R5 in series with the Amigas internal `47 Ohm`. Voltage input to `PVG612S` is around `1.2V`, perfect! The `5V` on the Nano V3 shows `5.03V`, Spot On!

***

<a href="images/build/sdbox_build_pic31.jpg">
<img src="images/build/sdbox_build_pic31.jpg" width="252" height="189">
</a>
<a href="images/build/sdbox_build_pic32.jpg">
<img src="images/build/sdbox_build_pic32.jpg" width="252" height="189">
</a>
<a href="images/build/sdbox_build_pic33.jpg">
<img src="images/build/sdbox_build_pic33.jpg" width="252" height="189">
</a>
<a href="images/build/sdbox_build_pic34.jpg">
<img src="images/build/sdbox_build_pic34.jpg" width="252" height="189">
</a>
<a href="images/build/sdbox_build_pic35.jpg">
<img src="images/build/sdbox_build_pic35.jpg" width="252" height="189">
</a>

***

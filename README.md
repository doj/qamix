# QAmix on https://github.com/doj/qamix

fork of QAMix 0.0.7e to bring to modern QT. This is based on the last
released version from http://alsamodular.sourceforge.net/

Send comments to Dirk Jagdmann <doj@cubic.org>

# original README file

## QAMix/KAMix version 0.0.7 by Matthias Nagorni

Note: KAMix is the KDE version of QAMix. It is compiled from the same source
with make -f make_kamix. The xml directories for KAMix are /usr/share/kamix
and ~/.kamix.

QAMix is a configurable mixer for ALSA. The interface is created while the
application parses an XML file. The GUI description depends on the
soundcard. Therefore QAMix derives the filename from the ALSA driver name.
It first searches for the GUI description in the directory .qamix in your
home directory. If no card specific file exists, QAMix looks for
"default.xml" in the same directory. If this does not exist either, the
directory /usr/share/qamix (specified as QAMIX_SHARE_DIR in mixer.h)
is searched. You can provide a filename for the GUI description with
the -g parameter. The soundcard device you would like to control is provided
with the -d parameter. The default device is hw:0.

There are three GUI description files included in this tarball:
deflt_mixer.xml    A minimal mixer interface that should work
                   with most AC97 compatible cards.
deflt_sblive.xml   A GUI interface for the SB Live card.
demo_mixer.xml     Identical to deflt_mixer.xml but with additional
                   MIDI bindings for the MIDI demo with AlsaModularSynth.

While the GUI description is parsed, the application tries to create a GUI
control for each element (tag: `<element>`) found in the description. The
ALSA CTL name for this element is provided with the tag `<alsaname>`. With the
optional tag `<mixname>` you can provide a differnt label for the control
(e.g. if the ALSA CTL name is too long).

QAMix can be completely controlled via MIDI. Elements can have one or
several MIDI tags. See below for more on this. A funny demo for this feature
can be run with AlsaModularSynth:

1. Start ams with the qamix_demo.ams (included in this tarball)
2. Start qamix -g demo_mixer.xml
3. Unlock all "Lock" switches in QAMix
4. Use either KAconnect or aconnect to connect the two readable MIDI ports
   of AlsModularSynth with the writeable port of QAMix.

## The GUI description format

The desciption starts with the tag `<mixer>` and ends with the corresponding
closing tag `</mixer>`. In the menu "View" of QAMix, the user can choose one
of three different display modes of the mixer: Full, Master only and Simple.
Correspondingly there are three sections in the XML file specifying the
look of the respective views. The global structure of the GUI description is
therefore:

```
<mixer>
  <full>
    ...Elements...
  </full>
  <master>
    ...Elements...
  </master>
  <simple>
    ...Elements...
  </simple>
</mixer>
```

Elements can either be put directly between these tags or they can be placed
inside a GUI container, like e.g. `<tab>` or `<vgroup>`.

If you want to create your own GUI description file, you can have QAMix
print out a default file that contains all CTL elements found for your device:

```
qamix -p > my_mixer.xml
```

This is a convenient starting point. All you have to do is to add tab and
group tags and sort the elements in the order you would like them to appear
in the GUI.

## Elements

The control elements of the mixer.

Example:
```
  <element>
    <alsaname> Master Playback Volume </alsaname>
    <mixname> Master </mixname>
  </element>
```

The tag `<alsaname>` is required. The parser tries to match this with the
names of the CTL elements provided by ALSA. If a corresponding ALSA CTL
element is found, the mixer control is created. If no tag `<mixname>` is
given, the alsaname is used for the label of the control.
The tag `<inverse/>` is useful for boolean controls like e.g. "Mute".
With this tag you can determine whether a checked checkbox means "on" or
"off".
The tag `<single/>` is valid only for sliders. It will generate one single
slider for parameters belonging to different channels. The values for
this parameter will then be equal for all channels. This is especially
usefull for the "Master only" view.

Elements can have an attribute "index". This is used when there are several
elements that have identical names.

Example:
```
  <element>
    <alsaname> DAC </alsaname>
    <mixname> DAC </mixname>
  </element>
  <element index = "1">
    <alsaname> DAC </alsaname>
    <mixname> DAC </mixname>
  </element>
  <element index = "2">
    <alsaname> DAC </alsaname>
    <mixname> DAC </mixname>
  </element>
```

## GUI container

Mixer elements can be grouped in tabs, vertical and horizontal groups.
There can be several layers of groups, but only one layer of tabs.
The name attribute of a tab tag is required. The tag `<space/>`
creates an empty widget that can be used as spacer in groups.

Example:
```
  <tab name="Playback">
    <vgroup>
      <element>
        <alsaname> Master Playback Volume </alsaname>
        <mixname> Master </mixname>
      </element>
      <element>
        <alsaname> Master Playback Switch </alsaname>
        <mixname> Mute </mixname>
        <inverse />
      </element>
    </vgroup>
  </tab>
```
There are three different group tags:

- `<vgroup>` Vertical group
- `<hgroup>` Horizontal group
- `<group>`  Horizontal group with named frame

vgroup and hgroup both take the attributes frame and width. frame can be
either "plain", "raised" or "sunken". width is an integer specifying the
with of the frame.

The group tag has only one attribute which is its name.

## Slider Numbers

You can specify whether and how the program should display current slider
values on top of each slider. The tag sliderNumbers is valid for all
slider elements following the tag.

```
<sliderNumbers style="off" />          No numbers
<sliderNumbers style="simple" />       Simple numbers without frame
<sliderNumbers style="simple100" />    Simple numbers normalized to 100
<sliderNumbers style="frame" />        Numbers in backlit frame
<sliderNumbers style="frame100" />     As above with normalized numbers
```

## Stretch Factors

All flavours of group tags, element tags and space tags can have the
additional attribute stretch. This specifies the relative sizes in the
parent container.

Example:
```
<vgroup stretch="3">
  <element stretch="2">
    ...
  </element>
  <element stretch="1">
    ...
  </element>
</vgroup>
<vgroup stretch="1">
  ...
</vgroup>
```

In this example, the second vgroup will horizontally occupy a third of the
space of the first one (provided that the minimum width of its child
widgets is small enough). The first element will try to take double the
space as the second one.

## MIDI

One or several MIDI tags can be placed inside of an element description.

Example: MIDI binding for the Modulation Wheel of MIDI channel 0

```
<midi index="0" type="10" ch="0" param="1" sign="1" />
```

All 5 attributes are required.

index: Index of the control (e.g. stereo sliders have index 0 and 1)
type: Type of the event: 10 = controller
                         13 = pitchbend
                          6 = note on
                          7 = note off
ch: MIDI channel
param: Controller number (for controller events)
       note (for note events)
       0 (for pitchbend events)
sign: 1 or -1.

Example: Crossfade between two sliders of a stereo control (e.g. master
volume). This is a virtual pan control.

```
<midi index="0" type="10" ch="0" param="1" sign="1" />
<midi index="1" type="10" ch="0" param="1" sign="-1" />
```

If the --channel parameter is >= 0 then the individual channel settings will
be overridden by the global channel parameter.

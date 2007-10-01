ShapeFusion 0.4
http://shapefusion.sourceforge.net

An editor for Marathon II, Marathon Infinity and AlephOne scenarios

Copyright 2000-2007 Tito Dal Canton and Etienne Samson


What is ShapeFusion?
--------------------
ShapeFusion is a scenario editor for the Bungie games "Marathon II: Durandal"
and "Marathon Infinity". It's useful also for the AlephOne engine, the Open
Source evolution of the original games (I consider it part of the AlephOne
effort, actually). A Marathon scenario is made of several files; ShapeFusion
is devoted to editing the "Shapes" file, which stores most of the game
graphics and animation data. It's *not* a tool for editing maps. In case you
ever used Bungie tools for editing Marathon scenarios, it's enough to say
that ShapeFusion is meant as a replacement for Anvil, or better for its
Shapes editor. Anvil has always been a buggy, incomplete, closed-source and
Mac-only editor: I hope things will be better with ShapeFusion. Thanks to
Etienne Samson, since version 0.4 ShapeFusion can open Sounds files too;
the Sounds editor is in progress.

As for the license, ShapeFusion is free software, released under the terms of
the GNU General Public License. For more information see the COPYING file
that comes with ShapeFusion.

History
-------
ShapeFusion was born in 2000 as a MacOS Classic application. Working releases
were produced, but then I moved to other projects and the development ended
while ShapeFusion was still far from being a complete editor.

With the death of the Classic platform and the spread of AlephOne on different
operating systems I became more and more attracted by the idea of porting
ShapeFusion to other systems too. This couldn't be done easily with the
original MacOS code, so eventually I started to rewrite everything using the
portable wxWidgets toolkit. I also had the opportunity to rework the user
interface, since I never liked the Anvil approach that I was trying to follow
in the original ShapeFusion. Etienne joined the project at this stage.

The new editor is not yet complete, but it's a great improvement at least on
the technical side (stability, code organization, portability). Adding new
features and fixing existing ones will be much easier with respect to the old
version.

Supported platforms
-------------------
ShapeFusion is being developed and constantly tested both on Linux and MacOS
X. Thanks to wxWidgets it should compile and work also on Windows and maybe
other systems, but I've never tried it, although I had reports of success.
ShapeFusion could *in theory* compile and work also on MacOS Classic, but I
have no plans on this side. If you build ShapeFusion on systems different
from Linux and MacOS X, please let me know (screenshots and binary builds
are appreciated!).

Building
--------
You'll need wxWidgets (at least version 2.6.1) to compile ShapeFusion. Version
2.8 is strongly recommended and will become the required one in next releases.

On Linux systems you can test the presence of wxWidgets by running the command
'wx-config --version'. If that command is available and gives you the right
version, everything should work; otherwise install wxWidgets following your
Linux distribution rules. To compile ShapeFusion, just do a 'make' inside the
source directory. When compilation is complete, simply run the 'shapefusion'
executable (you can put it anywhere in your PATH).

On MacOS X you should be able to use the provided Xcode project in the
osx-build directory. You'll need both the MacOS X developer tools and
wxWidgets. Note that we are releasing MacOS X binary builds along with each
source release: they should work out-of-the-box without the need to install
anything else. In case you're wondering why they are so large in size, it is
because they carry the whole wxWidgets code linked statically.

Overview of the Shapes editor window
------------------------------------
When you open a Shapes file, the window shows its collection slots on the left.
In standard Marathon Shapes there are always 32 of them; however, ShapeFusion
tries to be flexible and can open "extended" Shapes files with more
collections, like Damage Inc ones. Each slot can be opened revealing two
sub-slots, the 8 bit and true color versions. They represent two independent
versions of the same collection: the first will be used if the game is played
on low-color displays, the second is for modern high-color displays (including
the OpenGL mode in AlephOne). Each sub-slot can finally be opened to reveal
its real content: bitmaps, color tables, frames and sequences. The contrast
with Anvil is clear. Anvil tries to obfuscate some aspects of the Shapes file
organization, but I feel that this creates a lot of confusion in the user. In
my opinion, it makes more sense to treat Shapes files as they really are:
archives of collections. The ShapeFusion window tries to reflect this idea.

Bitmaps, color tables and frames are displayed as lists of preview thumbnails.
Selecting one of them will pop up an edit panel for that item. Sequences are
listed by their name since they can't be easily represented with thumbnails;
however, selecting a sequence will pop up an edit panel like for other items.

The logic behind ShapeFusion is that each collection is made of four sets of
entities: color tables, bitmaps, frames and sequences. If you follow this
order, entities coming after reference entities coming before: a sequence
references a set of frames, a frame references a bitmap, a bitmap naturally
references a color table since it's indexed. So editing a sequence involves
picking frames from the collection frame set, and editing a frame involves
selecting a bitmap from the collection bitmap set. Get it? ;-) Of course you
don't have to follow this flow, expecially if you are only adjusting some
settings.

The View menu contains display settings such as thumbnail size. Nothing here
affects what's inside the Shapes file.

Overview of the Sounds editor window
------------------------------------
TODO

Notes
-----
Please be careful when editing your scenario with ShapeFusion. We had no big
problems so far, but it's still under development and it may corrupt your
files, so be sure to keep backup copies. You shouldn't use versions before
0.3, as they are affected by a bug regarding frames minimum light intensity
value. If you think your files got corrupted by ShapeFusion, please let us
know what happened.

Object indexes start from 0, not 1. So if you have 12 bitmaps, the first is
bitmap 0 (not 1) and the last is bitmap 11 (not 12).

Bitmaps are exported as 8-bit Windows bitmap (.bmp) files. They get the color
table that is currently used for viewing, as selected in the View menu.

When you import a bitmap, ShapeFusion tries to map its colors to the collection
color table as best as it can. Although the algorithm often yelds good results,
you can help it a lot by converting the bitmap to indexed before (using
PhotoShop or Gimp, for example) so that it contains just colors belonging to the
collection color table.

In ShapeFusion, a frame can be referenced by many sequences. This may sound
strange at first, but in Shapes file format logic it's perfectly legal and I
think it makes sense too. However it can create a bit of confusion if you
forget it. Keep in mind that a famous Anvil bug is based on this very
confusion. But in Anvil it's a bug, in ShapeFusion it's a feature ;-)

If you open an "original" Shapes file (e.g. from Marathon II) and then save it
without touching anything, it won't probably have exactly the same size. In
other words, it won't be exactly the same file. However I've never been able
to tell the difference, apart from looking at the size. The original engines,
AlephOne and ShapeFusion itself seem to behave perfectly with both files. The
size mismatch has probably to do with data structure alignments: ShapeFusion
ignores alignments when writing files, it just packs structure after structure
and so produces generally smaller files.

Shapes files are known to rarely contain strange data, like negative sequence
ticks_per_frame. Maybe this comes from some broken Shapes editor, but it could
be just Anvil. ShapeFusion doesn't handle this invalid data very well, but I
plan to implement some consistency checks in the future.

Changelog
---------
0.4
	Switched to multidocument wxWidgets framework (major code rewrite).
	Implemented a preliminary Sounds editor.
	Implemented the color table editor.
	Improved the color quantization algorithm used when importing bitmaps.
	Bitmap masks can be exported to separate bmp files.
	Bitmaps are now exported to 8-bit bmp files.
	Lots of small changes and fixes.

0.3
	Improved functionality of the sequence editor.
	Implemented bitmap and frame deletion.
	Frame and sequence user interface cleanup.
	One can now edit frame scale factors.
	Fixed a serious bug regarding frames minimum light intensity value.
	Assorted changes and fixes.

0.2
	Work done on the color table section and on color table exporting features.
	Improved color reduction when importing RGB bitmaps.
	Implemented "Save all bitmaps".
	Implemented preliminary version of a true sequence editor.
	Implemented sequence deleting.
	Fixed sequence name charset issue that prevented some names from being displayed.
	BitmapBrowser and FrameBrowser speed optimizations.
	Assorted changes and fixes.

0.1
	First release of the new version using wxWidgets.


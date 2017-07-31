# On Writing A Class File Parser

I have an end-goal for a project that I think may be worthy of a significant
time investment. Part of that project is analyzing compiled bytecode generated
from languages like Java and Python. To start, I'm writing my own Java class
file parser. Here are some things I've learned while getting through the first
~320 bytes or so.

## Starting out

Reading binary files is really quite easy. For no particular reason, I chose to
write my class file parser in C. I'm not particularly good with C, but I do know
that it does quite well with handling bytes. The basic structure for the program
is really just "read this many bytes, then read this many bytes, then read N
bytes based on the previous bytes." This is all documented quite well by
[Oracle](https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html), which
is very handy. To start, I just got a basic program to read four bytes and print
out "CAFEBABE".

** CODE SNIPPET OR SOMETHING? **

## Gaining speed

I was able to get through a decent chunk of the class file
(mainly the header and constant pool) just by copying the pseudo-struct style of
the documentation and then reading the correct amount of bytes. This felt really
good! I was able to read a class file's constant pool and spit out some
information about what was in the class file:

```
magic: CAFEBABE
minor_version: 0
major_version: 52
constant_pool_count: 29
constant_pool[1]: Methodref| tag: 10; class_index: 6; name_and_type_index: 15
constant_pool[2]: Fieldref| tag: 9; class_index: 16; name_and_type_index: 17
constant_pool[3]: String| tag: 8; string_index: 18
constant_pool[4]: Methodref| tag: 10; class_index: 19; name_and_type_index: 20
constant_pool[5]: Class| tag: 7; name_index: 21
constant_pool[6]: Class| tag: 7; name_index: 22
constant_pool[7]: Utf8| tag: 1; length: 6; bytes: <init>
constant_pool[8]: Utf8| tag: 1; length: 3; bytes: ()V
constant_pool[9]: Utf8| tag: 1; length: 4; bytes: Code
constant_pool[10]: Utf8| tag: 1; length: 15; bytes: LineNumberTable
constant_pool[11]: Utf8| tag: 1; length: 4; bytes: main
constant_pool[12]: Utf8| tag: 1; length: 22; bytes: ([Ljava/lang/String;)V
constant_pool[13]: Utf8| tag: 1; length: 10; bytes: SourceFile
constant_pool[14]: Utf8| tag: 1; length: 8; bytes: Foo.java
constant_pool[15]: NameAndType| tag: 12; name_index: 7; descriptor_index: 8
constant_pool[16]: Class| tag: 7; name_index: 23
constant_pool[17]: NameAndType| tag: 12; name_index: 24; descriptor_index: 25
constant_pool[18]: Utf8| tag: 1; length: 13; bytes: Hello, world!
constant_pool[19]: Class| tag: 7; name_index: 26
constant_pool[20]: NameAndType| tag: 12; name_index: 27; descriptor_index: 28
constant_pool[21]: Utf8| tag: 1; length: 3; bytes: Foo
constant_pool[22]: Utf8| tag: 1; length: 16; bytes: java/lang/Object
constant_pool[23]: Utf8| tag: 1; length: 16; bytes: java/lang/System
constant_pool[24]: Utf8| tag: 1; length: 3; bytes: out
constant_pool[25]: Utf8| tag: 1; length: 21; bytes: Ljava/io/PrintStream;
constant_pool[26]: Utf8| tag: 1; length: 19; bytes: java/io/PrintStream
constant_pool[27]: Utf8| tag: 1; length: 7; bytes: println
constant_pool[28]: Utf8| tag: 1; length: 21; bytes: (Ljava/lang/String;)V
```

(I know - the tag isn't actually necessary, it's just a sanity check.)

## Feeling some accomplishment

How cool! If you ignore the first bit before the `:`, and you squint really hard
from across the room, you might almost think you're looking at output from
`javap -verbose`! Look at me, I might as well work at Oracle.

Now at this point in the real world, the coffee shop I was at was closing in
about an hour. I figured, "Hey, it took me a bit to get to this point but I'm a
good way through this whole file! I should be able to finish it before they
close!" So I got cracking. Next up, a few more bytes lying around in the class
file handling some flags and class stuff. There was some space for interfaces
and fields, but my "Hello World" doesn't use either of those, so I just had to
read the size (zero) and skip over them. Then I hit methods, which my program
_does_ have a couple of.

"Easy, just power through these - they're not that different from handling the
constant pool, surely!"

## Facing unrealistic expectations

That was roughly two weeks ago, at the time of this writing. I could parse the
header of the methods, which are just more indices into the constant pool table,
for the most part. But then I saw that methods (as well as fields, which I
didn't need to deal with) include a field of "type" `attribute_info`. So I
started looking at those and scrolled through the 24 subsections of the
attributes section.

Wow - there's a lot of stuff there - and I need to deal with (as long as the
rest of my boilerplate parsing code is functioning correctly) at least three of
them!

# History #
_cormo_ is heavily based on [litesql](http://sourceforge.net/apps/trac/litesql/), as I tried to use that a few months ago and adjust it to fit my needs. While the structure of _cormo_ still resembles _litesql_ a LOT, the entire code has been rewritten by now.

_litesql_ has quite a few features that _cormo_ still lacks, such as support for MySQL and SQLite, and Many-To-Many relationships, which hopefully will be added soon. At the same time _cormo_ tries to add some new features and different approaches, such as modelling in Python, which is potentially more powerful than XML. It also tries to be as conform with the Google C++ Style Guide as it can be.

# Google C++ Style Guide #
_cormo_ itself and the files it generates try to follow the guide as much as possible. Just things like maximum line length can't always be a priority when the file is generated automatically. However, even copyright and license header information can be passed to the generator, to make sure every file gets them. Proper project-scope `#define` guards are also used in generated header files.

One of the biggest (possibly the only) breaks with the Google C++ Style Guide is the use of exceptions, which after a few attempts without exceptions just seemed much, much easier and cleaner. But perhaps this can be changed again with the help of someone who has a good idea how to do it. :)
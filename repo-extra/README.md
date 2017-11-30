This repo contains the entire source code for each step of
[tgc-tutorial](https://github.com/snaptoken/tgc-tutorial). Each step is
represented by a commit. Each step name (found in the upper-right corner of
each step diff in the [tutorial](http://viewsourcecode.org/snaptoken/tgc)) is
a ref to that step's commit.

If you want to compare your version of `tgc.c` with the version in this repo
for a particular step, say `paused`, you could do it like this:

    $ git clone https://github.com/snaptoken/tgc-src
    $ cd tgc-src
    $ git checkout paused
    $ git diff --no-index -b ../path/to/your/tgc.c tgc.c

`--no-index` lets you use `git diff` as an ordinary diff tool, and `-b` ignores
differences in whitespace, which is important if you use a different indent
style than the one in the tutorial.


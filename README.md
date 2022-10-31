# ruby-edlib

[![Gem Version](https://badge.fury.io/rb/edlib.svg)](https://badge.fury.io/rb/edlib)
[![test](https://github.com/kojix2/ruby-edlib/actions/workflows/ci.yml/badge.svg)](https://github.com/kojix2/ruby-edlib/actions/workflows/ci.yml)
[![DOI](https://zenodo.org/badge/559318048.svg)](https://zenodo.org/badge/latestdoi/559318048)

[Edlib](https://github.com/Martinsos/edlib) - A lightweight and super fast C/C++ library for sequence alignment using edit distance

## Installation

```
gem install edlib
```

The Gem compiles the edlib source code inside the gem at installation. If you want to use the latest edlib, see Development.

## Usage

```ruby
require "edlib"

a = Edlib::Aligner.new(mode: :hw, task: :path)
a.align("AACG", "TCAACCTG")

# {
#  :edit_distance   => 1,
#  :alphabet_length => 4,
#  :locations       => [[2, 4], [2, 5]],
#  :alignment       => [0, 0, 0, 1],
#  :cigar           => "3=1I"
# }
```

|keyword argument     |description|
|---------------------|-----------------------------------------------------------------------------|
|k                    |edit distance is not larger than k [-1]                                             |
|mode                 |global (NW) , prefix (SHW) , infix (HW) ["NW"]                                      |
|task                 |DISTANCE, LOC, PATH ["DISTANCE"]                                                    |
|additional_equalities|List of pairs of characters, where each pair defines two characters as equal. [NULL]|

## Development

Pull requests welcome!

```sh
git clone https://github.com/kojix2/ruby-edlib # Please fork repo
cd ruby-edlib
bundle install
bundle exec rake compile
bundle exec rake test
```

Use latest edlib

```sh
git clone https://github.com/kojix2/ruby-edlib
cd ruby-edlib
bundle install
bundle exec rake edlib:update # Download latest edlib.h and edlib.cpp
bundle exec rake compile
bundle exec rake test
```

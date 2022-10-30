# ruby-edlib

* [Edlib](https://github.com/Martinsos/edlib) - A lightweight and super fast C/C++ library for sequence alignment using edit distance

## Installation

```
sudo apt install edlib-dev
gem install edlib
```

## API

```ruby
require "edlib"

# Aligner wraps EdlibAlignConfig
a = Edlib::Aligner.new(mode: :hw, task: :path)
a.align("AACG", "TCAACCTG")
# => {:edit_distance=>1, :alphabet_length=>4, :locations=>[[2, 4], [2, 5]], :alignment=>[0, 0, 0, 1], :cigar=>"3=1I"}
```

## Development

* Pull requests welcome

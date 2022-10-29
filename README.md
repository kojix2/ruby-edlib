# ruby-edlib

## Installation

```
sudo apt install edlib-dev
gem install edlib
```

## API Future Plan

```ruby
require "edlib"

# Aligner wraps EdlibAlignConfig
e = Edlib::Aligner.new
e = Edlib::Aligner.new(...)

r = e.align(...) # hash
```

## Development

edlib.h api

```
enum EdlibAlignMode
enum EdlibAlignTask
enum EdlibCigarFormat

struct EdlibEqualityPair
struct EdlibAlignConfig

edlibNewAlignConfig
edlibDefaultAlignConfig

struct EdlibAlignRsult

edlibFreeAlignResult
edlibAlign
edlibAlignmentToCigar
```

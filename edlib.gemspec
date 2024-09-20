# frozen_string_literal: true

require_relative 'lib/edlib/version'

Gem::Specification.new do |spec|
  spec.name          = 'edlib'
  spec.version       = Edlib::VERSION
  spec.authors       = ['kojix2']
  spec.email         = ['2xijok@gmail.com']

  spec.summary       = 'ruby-edlib is a wrapper for edlib.'
  spec.description   = 'Lightweight, super fast C/C++ library for sequence alignment using edit (Levenshtein) distance.'
  spec.homepage      = 'https://github.com/kojix2/ruby-edlib'
  spec.license       = 'MIT'
  spec.required_ruby_version = '>= 2.7.0'

  spec.files         = Dir['*.{md,txt}', '{lib,ext}/**/*.{c,cpp,h,rb}']
  spec.require_paths = ['lib']
  spec.extensions    = ['ext/edlib/extconf.rb']
end

require_relative 'edlib/edlib'

module Edlib
  class Align
    def initialize(*args)
      if args.empty?
        initialize_with_defaults
      else
        raise NotImplementedError
      end
    end
  end
end
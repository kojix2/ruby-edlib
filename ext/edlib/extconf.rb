# frozen_string_literal: true

require 'mkmf'

have_library "edlib"
create_makefile('edlib/edlibext')

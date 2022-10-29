require "mkmf"

dir_config('edlib')
if have_header('edlib.h') and have_library('edlib')
  create_makefile('edlib/edlib')
end

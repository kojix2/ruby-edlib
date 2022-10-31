require 'mkmf'

dir_config('edlib')
create_makefile('edlib/edlibext') if have_header('edlib.h') and have_library('edlib')

# frozen_string_literal: true

require 'bundler/gem_tasks'

namespace :edlib do
  desc 'Download latest edlib.h and edlib.cpp'
  task :update do
    require 'open-uri'
    base_url = 'https://raw.githubusercontent.com/Martinsos/edlib/master/'
    url_edlib_h = URI("#{base_url}edlib/include/edlib.h")
    url_edlib_cpp = URI("#{base_url}edlib/src/edlib.cpp")
    url_
    outdir = 'ext/edlib'
    URI.open(url_edlib_h) do |file|
      File.open("#{outdir}/edlib.h", 'w') do |out|
        out.write(file.read)
      end
    end
    warn "Saved #{outdir}/edlib.h"
    URI.open(url_edlib_cpp) do |file|
      File.open("#{outdir}/edlib.cpp", 'w') do |out|
        out.write(file.read)
      end
    end
    warn "Saved #{outdir}/edlib.cpp"
  end
end
require 'rake/testtask'

Rake::TestTask.new(:test) do |t|
  t.libs << 'test'
  t.libs << 'lib'
  t.test_files = FileList['test/**/*_test.rb']
end

require 'rake/extensiontask'

task build: :compile

Rake::ExtensionTask.new('edlibext') do |ext|
  ext.ext_dir = 'ext/edlib'
  ext.lib_dir = 'lib/edlib'
end

task default: %i[clobber compile test]

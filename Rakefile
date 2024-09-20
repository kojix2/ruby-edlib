# frozen_string_literal: true

require 'bundler/gem_tasks'

namespace :edlib do
  desc 'Download latest edlib.h and edlib.cpp'
  task :update do
    require 'open-uri'
    base_url = 'https://raw.githubusercontent.com/Martinsos/edlib/master/'
    urls = [
      "#{base_url}edlib/include/edlib.h",
      "#{base_url}edlib/src/edlib.cpp",
      "#{base_url}LICENSE"
    ]
    outdir = 'ext/edlib'
    urls.each do |url|
      name = File.basename(url)
      File.open("#{outdir}/#{name}", 'w') do |out|
        out.write(URI.parse(url).read)
      end
      warn "Saved #{outdir}/#{name}"
    end
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

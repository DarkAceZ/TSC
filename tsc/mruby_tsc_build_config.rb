# Build configuration file for mruby.

# Absolute path to the directory this file resides in,
# independent of any PWD or invocation stuff.
THIS_DIR = File.expand_path(File.dirname(__FILE__))
# Absolute path mruby resides in
MRUBY_DIR = File.join(THIS_DIR, "..", "mruby", "mruby")

config = lambda do |conf, root|
  #Standard things included with mruby
  conf.gembox "default"

  # Additional things
  conf.gem "#{THIS_DIR}/../mruby/mgems/mruby-sleep"         # Sleep
  conf.gem "#{THIS_DIR}/../mruby/mgems/mruby-pcre-regexp"   # PCRE Regular Expressions
  conf.gem "#{THIS_DIR}/../mruby/mgems/mruby-md5"           # MD5
  conf.gem "#{THIS_DIR}/../mruby/mgems/mruby-json"          # JSON
end

MRuby::Build.new do |conf|
  toolchain :gcc
  config.call(conf, root)
end

if ENV["CROSSCOMPILE_TARGET"] and !ENV["CROSSCOMPILE_TARGET"].empty?
  prefix = ENV["CROSSCOMPILE_TARGET"]

  MRuby::CrossBuild.new(prefix) do |conf|
    toolchain :gcc

    conf.cc do |cc|
      cc.command = ENV["CC"] || "#{prefix}-gcc"
    end

    conf.linker do |linker|
      linker.command = ENV["LD"] || "#{prefix}-gcc"
    end

    conf.archiver do |archiver|
      archiver.command = ENV["AR"] || "#{prefix}-ar"
    end

    config.call(conf, root)
  end
end

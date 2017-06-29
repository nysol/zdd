# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)

Gem::Specification.new do |spec|
  spec.name          = "nysol-zdd"
  spec.version       = "3.0.2"
  spec.authors       = ["nysol"]
  spec.email         = ["info@nysol.jp"]

  spec.summary       = %q{nysol zdd package}
  spec.description   = %q{nysol zdd package}
  spec.homepage      = "http://www.nysol.jp/"
	spec.extensions = ["ext/zdd_so/extconf.rb"]
  spec.files         = Dir.glob([
		"lib/nysol/zdd.rb",
		"ext/zdd_so/extconf.rb",
		"ext/zdd_so/zdd_so.cpp",
		"ext/zdd_so/lcm-vsop.cc",
		"ext/zdd_so/BDD.cc",
		"ext/zdd_so/BDDDG.cc",
		"ext/zdd_so/BDDHASH.cc",
		"ext/zdd_so/BtoI.cc",
		"ext/zdd_so/CtoI.cc",
		"ext/zdd_so/MLZBDDV.cc",
		"ext/zdd_so/SOP.cc",
		"ext/zdd_so/ZBDD.cc",
		"ext/zdd_so/ZBDDDG.cc",
		"ext/zdd_so/ZBDDHASH.cc",
		"ext/zdd_so/bddc.c",
		"ext/zdd_so/print.cc",
		"ext/zdd_so/table.cc",
		"ext/zdd_so/include/*",
		"ext/zdd_so/*.h"])


  spec.require_paths = ["lib"]

  spec.add_development_dependency "bundler", "~> 1.11"
  spec.add_development_dependency "rake", "~> 10.0"
  spec.add_development_dependency 'rake-compiler'

end


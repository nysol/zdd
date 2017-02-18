require "bundler/gem_tasks"
require "rake/extensiontask"


fList = [
	"ext/zdd_so/zdd_so.cpp",
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
	"ext/zdd_so/lcm-vsop.cc",
	"ext/zdd_so/vsop.h"
]

task "compile" => fList do
end

Rake::ExtensionTask.new "zdd_so" do |ext|
  ext.lib_dir = "lib/nysol"
end

file "ext/zdd_so/lcm-vsop.cc" => ["ext/zdd_so/ORG/lcm-vsop.ruby.sed","ext/zdd_so/ORG/lcm-vsop_add.cc","ext/zdd_so/ORG/SAPPOROBDD/src/BDDLCM/lcm.c"] do |t|
	f=""
	f << "sed -f #{t.prerequisites[0]} < #{t.prerequisites[2]} > xxcc;"
	f << "cat xxcc #{t.prerequisites[1]} > #{t.name}"
	system(f)
end

file "ext/zdd_so/zdd_so.cpp" => ["ext/zdd_so/ORG/zdd_so.cxx","ext/zdd_so/ORG/gensrc.rb"] do 
	system("cd ext/zdd_so/ORG && ./gensrc.rb")
end


file "ext/zdd_so/BDD.cc" => ["ext/zdd_so/ORG/SAPPOROBDD/src/BDD+/BDD.cc"] do |t|
	cp t.prerequisites.first, t.name
end

file "ext/zdd_so/BDDDG.cc" => ["ext/zdd_so/ORG/SAPPOROBDD/src/BDD+/BDDDG.cc"] do |t|
	cp t.prerequisites.first, t.name
end
file "ext/zdd_so/BDDHASH.cc" => ["ext/zdd_so/ORG/SAPPOROBDD/src/BDD+/BDDHASH.cc"] do |t|
	cp t.prerequisites.first, t.name
end
file "ext/zdd_so/BtoI.cc" => ["ext/zdd_so/ORG/SAPPOROBDD/src/BDD+/BtoI.cc"] do |t|
	cp t.prerequisites.first, t.name
end
file "ext/zdd_so/CtoI.cc" => ["ext/zdd_so/ORG/SAPPOROBDD/src/BDD+/CtoI.cc"] do |t|
	cp t.prerequisites.first, t.name
end
file "ext/zdd_so/MLZBDDV.cc" => ["ext/zdd_so/ORG/SAPPOROBDD/src/BDD+/MLZBDDV.cc"] do |t|
	cp t.prerequisites.first, t.name
end
file "ext/zdd_so/SOP.cc" => ["ext/zdd_so/ORG/SAPPOROBDD/src/BDD+/SOP.cc"] do |t|
	cp t.prerequisites.first, t.name
end
file "ext/zdd_so/ZBDD.cc" => ["ext/zdd_so/ORG/SAPPOROBDD/src/BDD+/ZBDD.cc"] do |t|
	cp t.prerequisites.first, t.name
end
file "ext/zdd_so/ZBDDDG.cc" => ["ext/zdd_so/ORG/SAPPOROBDD/src/BDD+/ZBDDDG.cc"] do |t|
	cp t.prerequisites.first, t.name
end
file "ext/zdd_so/ZBDDHASH.cc" => ["ext/zdd_so/ORG/SAPPOROBDD/src/BDD+/ZBDDHASH.cc"] do |t|
	cp t.prerequisites.first, t.name
end
file "ext/zdd_so/bddc.c" => ["ext/zdd_so/ORG/SAPPOROBDD/src/BDDc/bddc.c"] do |t|
	cp t.prerequisites.first, t.name
end
file "ext/zdd_so/print.cc" => ["ext/zdd_so/ORG/print.cc"] do |t|
	cp t.prerequisites.first, t.name
end
file "ext/zdd_so/table.cc" => ["ext/zdd_so/ORG/SAPPOROBDD/app/VSOP/table.cc"] do |t|
	cp t.prerequisites.first, t.name
end
file "ext/zdd_so/vsop.h" => ["ext/zdd_so/ORG/vsop.h"] do |t|
	cp t.prerequisites.first, t.name
end

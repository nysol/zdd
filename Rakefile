require "bundler/gem_tasks"
require "rake/extensiontask"

iDic = "ext/zdd_so/include/"
fList = [
	iDic,
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
	"ext/zdd_so/vsop.h",
	"ext/zdd_so/BDD.h",
	"ext/zdd_so/BDDDG.h",
	"ext/zdd_so/BtoI.h",
	"ext/zdd_so/CtoI.h",
	"ext/zdd_so/MLZBDDV.h",
	"ext/zdd_so/SOP.h",
	"ext/zdd_so/ZBDD.h",
	"ext/zdd_so/ZBDDDG.h",
	"ext/zdd_so/bddc.h",
	"ext/zdd_so/include/aheap.c",
	"ext/zdd_so/include/aheap.h",
	"ext/zdd_so/include/base.c",
	"ext/zdd_so/include/base.h",
	"ext/zdd_so/include/itemset.c",
	"ext/zdd_so/include/itemset.h",
	"ext/zdd_so/include/problem.c",
	"ext/zdd_so/include/problem.h",
	"ext/zdd_so/include/queue.c",
	"ext/zdd_so/include/queue.h",
	"ext/zdd_so/include/sgraph.c",
	"ext/zdd_so/include/sgraph.h",
	"ext/zdd_so/include/stdlib2.c",
	"ext/zdd_so/include/stdlib2.h",
	"ext/zdd_so/include/trsact.c",
	"ext/zdd_so/include/trsact.h",
	"ext/zdd_so/include/vec.c",
	"ext/zdd_so/include/vec.h"

]

directory iDic


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

file "ext/zdd_so/include/aheap.c" => ["ext/zdd_so/ORG/SAPPOROBDD/src/BDDLCM/aheap.c"] do |t|
	cp t.prerequisites.first, t.name
end
file "ext/zdd_so/include/aheap.h" => ["ext/zdd_so/ORG/SAPPOROBDD/src/BDDLCM/aheap.h"] do |t|
	cp t.prerequisites.first, t.name
end

file "ext/zdd_so/include/base.c" => ["ext/zdd_so/ORG/SAPPOROBDD/src/BDDLCM/base.c"] do |t|
	cp t.prerequisites.first, t.name
end
file "ext/zdd_so/include/base.h" => ["ext/zdd_so/ORG/SAPPOROBDD/src/BDDLCM/base.h"] do |t|
	cp t.prerequisites.first, t.name
end

file "ext/zdd_so/include/itemset.c" => ["ext/zdd_so/ORG/SAPPOROBDD/src/BDDLCM/itemset.c"] do |t|
	cp t.prerequisites.first, t.name
end
file "ext/zdd_so/include/itemset.h" => ["ext/zdd_so/ORG/SAPPOROBDD/src/BDDLCM/itemset.h"] do |t|
	cp t.prerequisites.first, t.name
end

file "ext/zdd_so/include/problem.c" => ["ext/zdd_so/ORG/SAPPOROBDD/src/BDDLCM/problem.c"] do |t|
	cp t.prerequisites.first, t.name
end
file "ext/zdd_so/include/problem.h" => ["ext/zdd_so/ORG/SAPPOROBDD/src/BDDLCM/problem.h"] do |t|
	cp t.prerequisites.first, t.name
end

file "ext/zdd_so/include/queue.c" => ["ext/zdd_so/ORG/SAPPOROBDD/src/BDDLCM/queue.c"] do |t|
	cp t.prerequisites.first, t.name
end
file "ext/zdd_so/include/queue.h" => ["ext/zdd_so/ORG/SAPPOROBDD/src/BDDLCM/queue.h"] do |t|
	cp t.prerequisites.first, t.name
end

file "ext/zdd_so/include/sgraph.c" => ["ext/zdd_so/ORG/SAPPOROBDD/src/BDDLCM/sgraph.c"] do |t|
	cp t.prerequisites.first, t.name
end
file "ext/zdd_so/include/sgraph.h" => ["ext/zdd_so/ORG/SAPPOROBDD/src/BDDLCM/sgraph.h"] do |t|
	cp t.prerequisites.first, t.name
end

file "ext/zdd_so/include/stdlib2.c" => ["ext/zdd_so/ORG/SAPPOROBDD/src/BDDLCM/stdlib2.c"] do |t|
	cp t.prerequisites.first, t.name
end
file "ext/zdd_so/include/stdlib2.h" => ["ext/zdd_so/ORG/SAPPOROBDD/src/BDDLCM/stdlib2.h"] do |t|
	cp t.prerequisites.first, t.name
end

file "ext/zdd_so/include/trsact.c" => ["ext/zdd_so/ORG/SAPPOROBDD/src/BDDLCM/trsact.c"] do |t|
	cp t.prerequisites.first, t.name
end
file "ext/zdd_so/include/trsact.h" => ["ext/zdd_so/ORG/SAPPOROBDD/src/BDDLCM/trsact.h"] do |t|
	cp t.prerequisites.first, t.name
end

file "ext/zdd_so/include/vec.c" => ["ext/zdd_so/ORG/SAPPOROBDD/src/BDDLCM/vec.c"] do |t|
	cp t.prerequisites.first, t.name
end
file "ext/zdd_so/include/vec.h" => ["ext/zdd_so/ORG/SAPPOROBDD/src/BDDLCM/vec.h"] do |t|
	cp t.prerequisites.first, t.name
end

rule /ext\/zdd_so\/[^\/]*\.h/ =>'%{^ext/zdd_so,ext/zdd_so/ORG/SAPPOROBDD/include}X.h' do |t|
	cp t.source , t.name
end



task "compile" => fList do
end
task "build" => fList do
end

#task "clean" => fList do
#end


Rake::ExtensionTask.new "zdd_so" do |ext|
  ext.lib_dir = "lib/nysol"
end


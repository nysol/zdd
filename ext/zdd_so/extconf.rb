require 'mkmf'
cp = "$(srcdir)"
$CFLAGS = " -O3 -Wall -I. -I#{cp}/include -DB_STATIC -D_NO_MAIN_ -DLINE -fPIC -Wno-error=format-security -Wno-narrowing"
$CPPFLAGS = " -O3 -Wall -I. -I#{cp}/include  -DB_STATIC -D_NO_MAIN_ -DLINE -fPIC -Wno-error=format-security -Wno-narrowing"
$CXXFLAGS = " -O3 -Wall -I. -I#{cp}/include  -DB_STATIC -D_NO_MAIN_ -DLINE -fPIC -Wno-error=format-security -Wno-narrowing"


$LOCAL_LIBS += " -lstdc++"

if RUBY_VERSION >= '2.0.0' then
	if Gem::Platform::local.os =~ /darwin/ then
	$CFLAGS += " -Wno-error=unused-command-line-argument-hard-error-in-future"
	$CPPFLAGS += " -Wno-error=unused-command-line-argument-hard-error-in-future"
	$LOCAL_LIBS += " -Wno-error=unused-command-line-argument-hard-error-in-future"
	end
end


if RUBY_VERSION < '1.9.0'
	$CFLAGS += " -o $@"
end


create_makefile("nysol/zdd_so")


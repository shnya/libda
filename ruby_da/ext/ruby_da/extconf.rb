require "mkmf"
RbConfig::CONFIG["CPP"] = "g++ -E"
$LDFLAGS += " -lstdc++"

dir_config('da')
if have_header('double_array.hpp')
   create_makefile("ruby_da/ruby_da")
end

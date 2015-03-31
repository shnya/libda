# coding: utf-8
lib = File.expand_path('../lib', __FILE__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)
require 'ruby_da/version'

Gem::Specification.new do |spec|
  spec.name          = "ruby_da"
  spec.version       = RubyDa::VERSION
  spec.authors       = ["Masahiko Higashiyama"]
  spec.email         = ["shnya@shnya.jp"]

  spec.summary       = %q{Double Array Implementation}
  spec.description   = %q{Double Array Implementation}
  spec.homepage      = "https://github.com/shnya/libda"
  spec.license       = "MIT"

  spec.files         = `git ls-files -z`.split("\x0").reject { |f| f.match(%r{^(test|spec|features)/}) }
  spec.bindir        = "exe"
  spec.executables   = spec.files.grep(%r{^exe/}) { |f| File.basename(f) }
  spec.require_paths = ["lib"]
  spec.extensions    = ["ext/ruby_da/extconf.rb"]

  spec.add_development_dependency "bundler", "~> 1.9"
  spec.add_development_dependency "rake", "~> 10.0"
  spec.add_development_dependency "rake-compiler"
  spec.add_development_dependency "minitest"
end

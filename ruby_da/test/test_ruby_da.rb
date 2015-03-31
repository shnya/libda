# -*- coding: utf-8 -*-
require 'minitest_helper'

class TestRubyDa < Minitest::Test
  def test_that_it_has_a_version_number
    refute_nil ::RubyDa::VERSION
  end

  def test_insert
    trie = RubyDa::Trie.new
    trie.build(["日本", "日本の夜明け", "日本の夜明け前"])
    puts trie.common_prefix_search("日本の夜明けは明るい")
  end
end

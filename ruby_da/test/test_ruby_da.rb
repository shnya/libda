# -*- coding: utf-8 -*-
require 'minitest_helper'

class TestRubyDa < Minitest::Test
  def test_that_it_has_a_version_number
    refute_nil ::RubyDa::VERSION
  end

  def test_insert
    trie = RubyDa::Trie.new
    trie.insert("日本", 1)
    trie.insert("日本の夜明け", 2)
    trie.insert("日本の夜明け前", 2)
    puts trie.common_prefix_search("日本の夜明けは明るい")
  end
end

#include "ruby_da.h"
#include "double_array.hpp"
using namespace std;

void wrap_RubyDa_free(DoubleArray* ptr){
  ptr->~DoubleArray();
  ruby_xfree(ptr);
}


VALUE wrap_RubyDa_allocate(VALUE self) {
  void* p = ruby_xmalloc(sizeof(DoubleArray));
  return Data_Wrap_Struct(self, NULL, wrap_RubyDa_free, p);
}


VALUE wrap_RubyDa_initialize(VALUE self) {
  DoubleArray* p;
  Data_Get_Struct(self, DoubleArray, p);
  new (p) DoubleArray();

  return Qnil;
}

VALUE wrap_RubyDa_common_prefix_search(VALUE self, VALUE str) {
  DoubleArray* p;
  Data_Get_Struct(self, DoubleArray, p);
  vector<string> result;
  p->common_prefix_search(StringValueCStr(str), result);
  VALUE rb_ary = rb_ary_new();
  for(vector<string>::iterator itr = result.begin();
      itr != result.end(); ++itr){
    rb_ary_push(rb_ary, rb_str_new2(itr->c_str()));
  }
  return rb_ary;
}

VALUE wrap_RubyDa_contains(VALUE self, VALUE str) {
  DoubleArray* p;
  Data_Get_Struct(self, DoubleArray, p);
  vector<string> result;
  bool is_contains = static_cast<int>(p->contains(StringValueCStr(str)));
  if(is_contains){
    return Qtrue;
  }else{
    return Qfalse;
  }
}

VALUE wrap_RubyDa_extract_all_matched(VALUE self, VALUE str) {
  DoubleArray* p;
  Data_Get_Struct(self, DoubleArray, p);
  vector<string> result;
  p->extract_all_matched(StringValueCStr(str), result);
  VALUE rb_ary = rb_ary_new();
  for(vector<string>::iterator itr = result.begin();
      itr != result.end(); ++itr){
    rb_ary_push(rb_ary, rb_str_new2(itr->c_str()));
  }
  return rb_ary;
}


VALUE wrap_RubyDa_insert(VALUE self, VALUE str, VALUE id){
  DoubleArray* p;
  Data_Get_Struct(self, DoubleArray, p);
  bool is_success = p->insert(StringValueCStr(str), NUM2INT(id));
  if(is_success){
    return Qtrue;
  }else{
    return Qfalse;
  }
}

VALUE wrap_RubyDa_erase(VALUE self, VALUE str){
  DoubleArray* p;
  Data_Get_Struct(self, DoubleArray, p);
  bool is_success = p->erase(StringValueCStr(str));
  if(is_success){
    return Qtrue;
  }else{
    return Qfalse;
  }
}


extern "C" {
  void Init_ruby_da(void) {
    VALUE rb_RubyDa = rb_define_module("RubyDa");
    VALUE klass = rb_define_class_under(rb_RubyDa, "Trie", rb_cObject);

    rb_define_alloc_func(klass, wrap_RubyDa_allocate);
    rb_define_private_method(klass, "initialize", (VALUE(*)(...))wrap_RubyDa_initialize, 0);
    rb_define_method(klass, "common_prefix_search", (VALUE(*)(...))wrap_RubyDa_common_prefix_search, 1);
    rb_define_method(klass, "contains", (VALUE(*)(...))wrap_RubyDa_contains, 1);
    rb_define_method(klass, "extract_all_matched", (VALUE(*)(...))wrap_RubyDa_extract_all_matched, 1);
    rb_define_method(klass, "insert", (VALUE(*)(...))wrap_RubyDa_insert, 2);
    rb_define_method(klass, "erase", (VALUE(*)(...))wrap_RubyDa_erase, 1);
  }
}

#include "ruby_da.h"
#include <ruby/encoding.h>
#include "double_array.hpp"
using namespace std;

static int UTF8_ENCODE_ID = rb_enc_find_index("UTF-8");
static int ASCII_ENCODE_ID = rb_enc_find_index("US-ASCII");

static void wrap_RubyDa_free(DoubleArray* ptr){
  ptr->~DoubleArray();
  ruby_xfree(ptr);
}


static VALUE wrap_RubyDa_allocate(VALUE self) {
  void* p = ruby_xmalloc(sizeof(DoubleArray));
  return Data_Wrap_Struct(self, NULL, wrap_RubyDa_free, p);
}


static VALUE wrap_RubyDa_initialize(VALUE self) {
  DoubleArray* p;
  Data_Get_Struct(self, DoubleArray, p);
  new (p) DoubleArray();

  return Qnil;
}

static VALUE vector_to_array(vector<string> vec) {
  VALUE rb_ary = rb_ary_new();
  for(vector<string>::iterator itr = vec.begin(); itr != vec.end(); ++itr){
    VALUE rstr = rb_str_new2(itr->c_str());
    rb_enc_associate_index(rstr, UTF8_ENCODE_ID);
    rb_ary_push(rb_ary, rb_str_new2(itr->c_str()));
  }
  return rb_ary;
}

static DoubleArray* get_ptr(VALUE self){
  DoubleArray* p; 
  Data_Get_Struct(self, DoubleArray, p);
  return p;
}

static void check_utf8_encoding(VALUE str){
  int encoding = rb_enc_get_index(str);
  if(encoding != UTF8_ENCODE_ID && encoding != ASCII_ENCODE_ID){
    rb_raise(rb_eRuntimeError, "invalid parameter, input string is not utf-8");
  }
}

static VALUE wrap_RubyDa_common_prefix_search(VALUE self, VALUE str) {
  check_utf8_encoding(str);
  DoubleArray* p = get_ptr(self);
  vector<string> result;
  p->common_prefix_search(StringValueCStr(str), result);
  return vector_to_array(result);
}

static VALUE wrap_RubyDa_contains(VALUE self, VALUE str) {
  check_utf8_encoding(str);
  DoubleArray* p = get_ptr(self);
  vector<string> result;
  bool is_contains = static_cast<int>(p->contains(StringValueCStr(str)));
  return is_contains ? Qtrue : Qfalse;
}

static VALUE wrap_RubyDa_extract_all_matched(VALUE self, VALUE str) {
  check_utf8_encoding(str);
  DoubleArray* p = get_ptr(self);
  vector<string> result;
  p->extract_all_matched(StringValueCStr(str), result);
  return vector_to_array(result);
}


static VALUE wrap_RubyDa_insert(int argc, VALUE* argv, VALUE self) {
  if(argc == 0){
    rb_raise(rb_eRuntimeError, "insert need at least 1 argument");
  }
  VALUE str = argv[0];
  check_utf8_encoding(str);
  int id = -1;
  if(argc == 2){
    id = NUM2INT(argv[1]);
  }
  DoubleArray* p = get_ptr(self);
  bool is_success = p->insert(StringValueCStr(str), id);
  return is_success ? Qtrue : Qfalse;
}

static VALUE wrap_RubyDa_build(VALUE self, VALUE ary) {
  DoubleArray* p = get_ptr(self);
  unsigned long ary_len = RARRAY_LEN(ary);
  vector<string> inputs;
  for(unsigned long i = 0; i < ary_len; i++){
    VALUE entry = rb_ary_entry(ary, i);
    check_utf8_encoding(entry);
    inputs.push_back(StringValueCStr(entry));
  }
  bool is_success = p->build(inputs);
  return is_success ? Qtrue : Qfalse;
}

static VALUE wrap_RubyDa_erase(VALUE self, VALUE str) {
  check_utf8_encoding(str);
  DoubleArray* p = get_ptr(self);
  bool is_success = p->erase(StringValueCStr(str));
  return is_success ? Qtrue : Qfalse;
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
    rb_define_method(klass, "insert", (VALUE(*)(...))wrap_RubyDa_insert, -1);
    rb_define_method(klass, "build", (VALUE(*)(...))wrap_RubyDa_build, 1);
    rb_define_method(klass, "erase", (VALUE(*)(...))wrap_RubyDa_erase, 1);
  }
}

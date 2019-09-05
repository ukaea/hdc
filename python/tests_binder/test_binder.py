#! /usr/bin/env python3
from pyhdc import HDC
import pytest

from  test_module_triple_int_interface import triple_int
from  test_module_add_double_interface import add_double
from  test_module_modify_string_interface import modify_string
from  test_module_merge_hdc_trees_interface import merge_hdc_trees
from  function_add_double_interface import function_add_double

def test_triple_int():
    int_in = 22
    int_out = triple_int(int_in)
    assert 3*int_in == int_out
    assert type(int_in) == type(int_out)

def test_add_double():
    a = 22.2
    b = 33.3
    c = add_double(a,b)
    assert a+b == c
    assert type(a) == type(c)

def test_add_double_function():
    a = 22.2
    b = 33.3
    c = function_add_double(a,b)
    assert a+b == c
    assert type(a) == type(c)

def test_modify_string():
    str_in = "hura"
    str_out = modify_string(str_in)
    expected = "{}_{}".format(str_in,str_in)
    assert str_out == expected

def test_merge_hdc_trees():
    tree1 = HDC()
    tree1["aaa/bbb"] = "some string"
    tree1["aaa/double"] = 22.22

    tree2 = HDC()
    tree2["aaa/double2"] = 33.333

    tree_out = merge_hdc_trees(tree1,tree2)

    tree_expected = HDC()
    tree_expected["tree1"] = tree1
    tree_expected["tree2"] = tree2

    assert tree_expected.dumps() == tree_out.dumps()

if __name__ == "__main__":
    pytest.main()

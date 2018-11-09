%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% test constructor %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
obj = HDC();
assert(class(obj) == "HDC")

s = 1.23;
obj = HDC(s);
assert(obj.get_data() == s)

s = int8(-8);
obj = HDC(s);
assert(obj.get_data() == s)
assert(class(obj.get_data()) == class(s))

s = 1:5;
obj = HDC(s);
assert(obj.get_data() == s)

s = reshape(1:10,2,5);
obj = HDC(s);
assert(obj.get_data() == s)

s = "aaa"
obj.set_data(s);
assert(obj.get_data() == s)

s = 'bbb'
obj = HDC(s);
assert(obj.get_data() == s)



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% test set / get %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
s = 1.23;
obj = HDC(s);
assert(obj.get_data() == s)


s = 1:5;
obj.set_data(s);
assert(obj.get_data() == s)

s = reshape(1:10,2,5);
obj.set_data(s);
assert(obj.get_data() == s)

s = "aaa"
obj.set_data(s);
assert(obj.get_data() == s)

s = 'bbb'
obj.set_data(s);
assert(obj.get_data() == s)



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% test node manipulation %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
tree = HDC();
s = 999;
child = HDC(s);
path = "aaa/bbb";

% test add
tree.add(path,child);
child2 = tree.get(path);
assert(child2.get_data() == s)
assert(tree.at(path) == s)

%test set
r = 666;
child = HDC(r);
tree.set(path,child);
assert(child2.get_data() == r)
assert(tree.at(path) == r)

%test list
list = HDC();
s = 1.23456;
s2 = 2.22222;
ch = HDC(s);
ch2 = HDC(s2);
list.append(ch);
list.append(ch);
list.append(ch);
ch_ = list.get(1);
assert(list.get(3).get_data() == s)
list.insert(3,ch2);
assert(list.get(3).get_data() == s2)
assert(list.get(4).get_data() == s)

%test add, set and keys
tree = HDC();
s = 1.23456;
s2 = 2.22222;
ch = HDC(s);
ch2 = HDC(s2);
tree.add_child("aaa",ch)
assert(tree.shape() == 1)
tree.add_child("bbb",ch)
assert(tree.shape() == 2)
assert(tree.keys() == {'aaa';'bbb'}


%test set/at
tree = HDC()
tree.set("aaa/bbb",3.141592)
a = tree.at("aaa/bbb")
assert(a,3.141592)
assert(class(a),"double")
c = tree.at("aaa")
assert(class(c),"HDC")
tree.set("bbb",45645656456)
assert(tree.exists("bbb"))
tree.delete_child("bbb");
assert(~tree.exists("bbb"))



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% TODO %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% json / HDF5
%l = HDC.load_json("tree.json","");
% more sizes / types + class strings, errors when type not known, objects / cell arrays???
% rename this, to test_*, prepare installation similarly to python
% remove absolute paths from includes
